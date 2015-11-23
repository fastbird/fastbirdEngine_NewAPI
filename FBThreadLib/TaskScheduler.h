#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird
{
DECLARE_SMART_PTR(Task);
DECLARE_SMART_PTR(TaskScheduler);
class TaskScheduler
{
	DECLARE_PIMPL_NON_COPYABLE(TaskScheduler);
	TaskScheduler(int NumThreads);

public:
	/** Create TaskSchedular
	\param numThread if 0, the number of thread will be the same as number of cpu cores.
	*/
	static TaskSchedulerPtr Create(int numThread);
	
	void AddTask(TaskPtr NewTask);
	bool _IsFinalized() const; // internal.
	void _Schedule(); // internal.

private:
	friend class Task;
	friend class WorkerThread;
	void AddIdleWorker(WorkerThread* w);
	void AddPendingTask(Task* t);
	void SchedulerSlice();
	TaskPtr GetNextReadyTask(WorkerThread* Thread);	
};

}