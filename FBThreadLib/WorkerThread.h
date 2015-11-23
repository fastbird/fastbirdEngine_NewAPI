#pragma once
#include "FBCommonHeaders/Types.h"
#include "threads.h"
#include "AsyncObjects.h"
namespace fastbird
{
class TaskScheduler;
DECLARE_SMART_PTR(Task);
DECLARE_SMART_PTR(SyncEvent);
class WorkerThread: public Thread
{
protected:
    TaskPtr mCurrentTask;          // Current executing task.
	SyncEventPtr mTasksEvent;      // Event used when waiting for a task.
    TaskScheduler* mScheduler;          // Scheduler owning this worker thread.

	FB_CRITICAL_SECTION mTaskCS;

public:
    WorkerThread(TaskScheduler* scheduler);
	~WorkerThread();

    void SetTask(TaskPtr t);
	virtual bool Run();
};

}