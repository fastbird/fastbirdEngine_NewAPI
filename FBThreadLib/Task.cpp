#include "stdafx.h"
#include "Task.h"
#include "TaskScheduler.h"
#include "threads.h"
#include "AsyncObjects.h"
namespace fastbird
{
static ThreadSafeCounter counter;
Task::Task(bool _WaitEvent, ThreadSafeCounter* _ExecCounter)
	: mScheduled(false)
	, mExecuted(false)
	, mIsDependency(false)
	, mIsHashed(false)
	, mExecCounter(_ExecCounter)
	, mSyncCounter(0)
	, mHashNext(NULL)
	, mMyHash(-1)
	, mTriggered(false)
{
    static volatile DWORD UniqueTaskID = 0;	
	mTaskID = ++counter;		
	if (_WaitEvent){
		mWaitEvent = CreateSyncEvent(true);
	}
}

Task::~Task()
{
}

void Task::OnExecuted()
{
    if(mExecCounter)
    {
		mExecCounter->operator--();
    }

    if(mWaitEvent)
    {
		mWaitEvent->Trigger();
    }
}

void Task::Trigger(TaskScheduler* pScheduler)
{
	mTriggered = true;
    Execute(pScheduler);

	mExecuted = true;

    if(!mIsHashed && IsExecuted())
    {
        OnExecuted();
    }
    else
    {
		pScheduler->AddPendingTask(this);
		pScheduler->SchedulerSlice();
    }
}

int Task::GetDependencies(TaskPtr Dependencies[])
{
	Dependencies = NULL;
	return 0;
}

void Task::Sync()
{
	if (mWaitEvent)
		mWaitEvent->Wait();	
}

void Task::Reset()
{
	Sync();
	mScheduled = false;
	mExecuted = false;
	mTriggered = false;
	if (mWaitEvent)
		mWaitEvent->Reset();
}

bool Task::IsExecuted() const
{
	return mExecuted && *mSyncCounter==0;
}

bool Task::IsTriggered() const{
	return mTriggered;
}

}