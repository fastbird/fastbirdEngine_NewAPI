#include "stdafx.h"
#include "TaskScheduler.h"
#include "FBCommonHeaders/LockFreeQueue.h"
#include "FBCommonHeaders/SpinLock.h"
#include "AsyncObjects.h"
#include "WorkerThread.h"
#include "Task.h"
#include "FBCommonHeaders/Helpers.h"
#include "FBSystemLib/System.h"
#include <thread>
using namespace fastbird;

static const int MAX_NUM_QUEUE = 65536;
SyncEventPtr gScheduleSliceEvent = 0;
namespace fastbird
{
	TaskScheduler* gTaskScheduler = 0;

	void Scheduler()
	{
		while (!gTaskScheduler->_IsFinalized())
		{
			gTaskScheduler->_Schedule();
			gScheduleSliceEvent->Wait(66);
		}
	}
}

class TaskScheduler::Impl{
public:
	TaskScheduler* mSelf;
	std::thread mSchedulerThread;
	// Array of worker threads
	std::vector<WorkerThread*> mWorkerThreads;
	int mNumWorkerThreads;

	// Lock used to check if a scheduling slice is already running (never waits)
	SpinLock<false, false> mSchedulerLock;

	// Hash table of active tasks
	TaskPtr mActiveTasksMap[1024];

	// Internal queues
	LockFreeQueue<TaskPtr> mPendingTasksQueue;
	LockFreeQueue<TaskPtr> mReadyTasksQueue;
	LockFreeQueue<WorkerThread*> mIdleThreadsQueue;

	FB_CRITICAL_SECTION mQueueCS;
	bool mFinalize;

	// Used to maintain the number of scheduling slices requested
	volatile long mSchedulingSlices;


	//---------------------------------------------------------------------------
	Impl(TaskScheduler* self, int numThread)
		: mSelf(self)
		, mFinalize(false)
	{
		mSchedulingSlices = 0;
		for (int i = 0; i<ARRAYCOUNT(mActiveTasksMap); i++)
		{
			mActiveTasksMap[i] = NULL;
		}

		mNumWorkerThreads = (numThread == 0) ? GetNumProcessors() : numThread;
		assert(mNumWorkerThreads > 0);

		mWorkerThreads.assign(mNumWorkerThreads, 0);

		for (int i = 0; i<mNumWorkerThreads; i++)
		{
			mWorkerThreads[i] = new WorkerThread(mSelf);
			AddIdleWorker(mWorkerThreads[i]);
		}

		Logger::Log(FB_DEFAULT_LOG_ARG, FormatString("Task Scheduler initialized using %d worker threads\n", mNumWorkerThreads).c_str());		

		gScheduleSliceEvent = CreateSyncEvent(false);
		mSchedulerThread = std::thread(Scheduler);
	}

	~Impl(){
		mFinalize = true;
		std::this_thread::sleep_for(std::chrono::microseconds(500));
		for (int i = 0; i<mNumWorkerThreads; i++)
		{
			mWorkerThreads[i]->ForceExit(false);
			mWorkerThreads[i]->SetTask(0);
		}
		std::this_thread::sleep_for(std::chrono::microseconds(500));
		gScheduleSliceEvent->Trigger();
		mSchedulerThread.join();
		gScheduleSliceEvent = 0;
		for (int i = 0; i<mNumWorkerThreads; i++)
		{
			if (!mWorkerThreads[i]->IsRunning())
				delete mWorkerThreads[i];
		}
	}

	//---------------------------------------------------------------------------
	// Public
	//---------------------------------------------------------------------------
	void AddTask(TaskPtr NewTask){
		if (mFinalize)
			return;
		assert(NewTask);
		TaskPtr* Dependencies = NULL;
		int NumDependencies = NewTask->GetDependencies(Dependencies);

		// Recursivly add dependant tasks
		for (int i = 0; i<NumDependencies; i++)
		{
			Dependencies[i]->mIsDependency = true;
			AddTask(Dependencies[i]);
		}

		if (!NumDependencies && !NewTask->mIsDependency)
		{
			// The task is ready to execute; schedule it right away
			ScheduleTask(NewTask);
		}
		else
		{
			// Since the task has dependencies, we register it internally.
			AddPendingTask(NewTask);
			SchedulerSlice();
		}
	}

	//---------------------------------------------------------------------------
	// Called by WorkerThread or Task
	//---------------------------------------------------------------------------
	void AddIdleWorker(WorkerThread* w){
		if (mFinalize)
			return;
		LOCK_CRITICAL_SECTION lock(mQueueCS);
		mIdleThreadsQueue.Enq(w);
	}

	void AddPendingTask(TaskPtr t){
		if (mFinalize)
			return;
		LOCK_CRITICAL_SECTION lock(mQueueCS);
		mPendingTasksQueue.Enq(t);
	}

	void SchedulerSlice(){
		if (mFinalize)
			return;
		if (mSchedulerLock.Lock())
		{
			TaskPtr CurTask;
			while (CurTask = mPendingTasksQueue.Deq())
			{
				//Log("Task %d Dequeued from pending", CurTask->mTaskID);
				if (CurTask->mScheduled)
				{
					if (!CurTask->mIsHashed)
					{
						// Fast path: the task is fully executed and not hashed
						if (CurTask->IsExecuted())
						{
							CurTask->OnExecuted();
							//Log("Task %d OnExcuted()", CurTask->mTaskID);
							CurTask = 0;
						}
						else
						{
							// Register task in dependencies hashmap
							assert(!CurTask->mHashNext);
							DWORD HashBin = (CurTask->mTaskID) &
								(ARRAYCOUNT(mActiveTasksMap) - 1);
							CurTask->mHashNext = mActiveTasksMap[HashBin];
							assert(mActiveTasksMap[HashBin] != CurTask);
							mActiveTasksMap[HashBin] = CurTask;
							CurTask->mIsHashed = true;
							CurTask->mMyHash = HashBin;
							//Log("Task %d Hashed()", CurTask->mTaskID);
						}
					}
				}
				else
				{
					TaskPtr* Dependencies = NULL;
					int NumDependencies = CurTask->GetDependencies(Dependencies);
					if (CurTask->mIsDependency || NumDependencies)
					{
						// Register task in dependencies hashmap
						assert(!CurTask->mHashNext);
						DWORD HashBin = (CurTask->mTaskID) &
							(ARRAYCOUNT(mActiveTasksMap) - 1);
						CurTask->mHashNext = mActiveTasksMap[HashBin];
						assert(mActiveTasksMap[HashBin] != CurTask);
						CurTask->mIsHashed = true;
						mActiveTasksMap[HashBin] = CurTask;
					}

					if (!NumDependencies)
					{
						// Schedule ready tasks
						ScheduleTask(CurTask);
					}
				}
			}

			// Schedule ready tasks with dependencies
			for (int i = 0; i < ARRAYCOUNT(mActiveTasksMap); i++)
			{
				TaskPtr CurTask = mActiveTasksMap[i];
				TaskPtr* PrevLink = &mActiveTasksMap[i];

				while (CurTask)
				{
					assert(CurTask->mIsHashed);
					bool UnHashTask = false;

					if (!CurTask->mScheduled)
					{
						bool TaskIsReady = true;
						TaskPtr* Dependencies = NULL;
						int NumDependencies = CurTask->GetDependencies(Dependencies);

						for (int j = 0; j < NumDependencies; j++)
						{
							TaskPtr ChildTask = Dependencies[j];

							DWORD HashBin = (ChildTask->mTaskID) &
								(ARRAYCOUNT(mActiveTasksMap) - 1);
							TaskPtr HashTask = mActiveTasksMap[HashBin];

							while (HashTask)
							{
								if ((HashTask == ChildTask) && !ChildTask->IsExecuted())
								{
									TaskIsReady = false;
									break;
								}

								HashTask = HashTask->mHashNext;
							}
						}

						if (TaskIsReady)
						{
							ScheduleTask(CurTask);
							UnHashTask = !CurTask->mIsDependency;
						}
					}
					else if (CurTask->IsExecuted())
					{
						CurTask->OnExecuted();
						//Log("Hashed Task %d OnExecuted()", CurTask->mTaskID);						
						UnHashTask = true;
					}

					if (UnHashTask)
					{
						*PrevLink = CurTask->mHashNext;
						CurTask->mIsHashed = false;
						//Log("Hashed Task %d is Unhashed()", CurTask->mTaskID);
					}
					else
					{
						PrevLink = &CurTask->mHashNext;
					}

					TaskPtr NextTask = CurTask->mHashNext;

					CurTask = NextTask;
				}
			}
			mSchedulerLock.Unlock();
		}// if mSchedulerLock
		else
		{
			gScheduleSliceEvent->Trigger();
		}
	}

	TaskPtr GetNextReadyTask(WorkerThread* Thread){
		if (mFinalize)
			return 0;
		LOCK_CRITICAL_SECTION lock(mQueueCS);

		auto ReadyTask = mReadyTasksQueue.Deq();
		if (ReadyTask)
		{
			return ReadyTask;
		}

		return NULL;
	}

	//---------------------------------------------------------------------------
	// Internal
	//---------------------------------------------------------------------------
	WorkerThread* GetNextIdleThread(TaskPtr t){
		if (mFinalize)
			return 0;
		LOCK_CRITICAL_SECTION lock(mQueueCS);

		return mIdleThreadsQueue.Deq();
	}

	void AddReadyTask(TaskPtr t){
		if (mFinalize)
			return;
		LOCK_CRITICAL_SECTION lock(mQueueCS);
		mReadyTasksQueue.Enq(t);
	}

	void ScheduleTask(TaskPtr t){
		if (mFinalize)
			return;
		t->mScheduled = true;
		WorkerThread* WorkerThread = GetNextIdleThread(t);
		if (WorkerThread)
		{
			WorkerThread->SetTask(t);
		}
		else
		{
			AddReadyTask(t);
		}
	}
};

TaskSchedulerPtr TaskScheduler::Create(int numThreads){
	return TaskSchedulerPtr(new TaskScheduler(numThreads), [](TaskScheduler* obj){ delete obj; });
}
TaskScheduler::TaskScheduler(int NumThreads)
	: mImpl(new Impl(this, NumThreads))
{
	gTaskScheduler = this;
}

bool TaskScheduler::_IsFinalized() const{
	return mImpl->mFinalize;
}

void TaskScheduler::_Schedule(){
	mImpl->SchedulerSlice();
}