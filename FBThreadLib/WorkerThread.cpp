#include "stdafx.h"
#include "WorkerThread.h"
#include "Task.h"
#include "TaskScheduler.h"
namespace fastbird
{

WorkerThread::WorkerThread(TaskScheduler* scheduler)
	: mScheduler(scheduler)
{
    mTasksEvent = CreateSyncEvent();

    static DWORD ThreadID = 0;
    static char ThreadName[128];
    sprintf_s(ThreadName, "worker_thread_%d", ThreadID++);

    CreateThread(256 * 1024, ThreadName);
}

WorkerThread::~WorkerThread()
{
}

bool WorkerThread::Run()
{
	mTasksEvent->Wait();

    // Loop executing ready tasks
	while (mCurrentTask)
    {
        // Execute current task
		mCurrentTask->Trigger(mScheduler);

        // Try to get another task
		mCurrentTask = mScheduler->GetNextReadyTask(this);
    }
	mScheduler->AddIdleWorker(this);
	mScheduler->SchedulerSlice();

	return !IsForceExit();
}

void WorkerThread::SetTask(TaskPtr t)
{
	mCurrentTask = t;
	mTasksEvent->Trigger();
}

}