#pragma once
#include <atomic>
#include "threads.h"
namespace fastbird
{
DECLARE_SMART_PTR(SyncEvent);
DECLARE_SMART_PTR(Task);
class Task
{
    friend class TaskScheduler;
    friend class WorkerThread;

protected:
	Task(bool _WaitEvent = false, ThreadSafeCounter* _ExecCounter = NULL);
	virtual ~Task();
    void Trigger(TaskScheduler* Scheduler);
	virtual int GetDependencies(TaskPtr Dependencies[]);

    // Called by the scheduler when the task is fully executed.
    virtual void OnExecuted();

    unsigned mTaskID;              // Internal task-id, used for task hashing.
    TaskPtr mHashNext;            // Internal hashmap next element pointer.
	unsigned mMyHash;	
	ThreadSafeCounter* mExecCounter;	// Pointer to a variable that gets decremented when execution is done.
	ThreadSafeCounter mSyncCounter;  // Used to wait for subtasks to complete.
	SyncEventPtr mWaitEvent;       // Event used to wait for a task to complete.
	std::atomic<bool> mExecuted;          // Is this task executed?
	std::atomic<bool> mIsHashed;          // Is this task in the dependencies hashmap?
    bool mScheduled : 1;         // Is this task scheduled?
    bool mIsDependency : 1;      // Is this task a dependency for another task?
	bool mTriggered : 1;
    

public:

    virtual void Execute(TaskScheduler* Scheduler)=0;

	bool IsExecuted() const;
	bool IsTriggered() const;

	// wait to finish
	void Sync();
	// to re add.
	virtual void Reset();
};

}