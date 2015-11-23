#pragma once
#include "FBCommonHeaders/Types.h"
#include <thread>
namespace fastbird
{

//---------------------------------------------------------------------------
struct ThreadInfo
{
	char ThreadName[128];
	std::thread::id mThreadID;

	ThreadInfo()
	{
	}
};

extern __declspec(thread) ThreadInfo* GThreadDesc;
void SwitchThread();
void SetThreadName(DWORD ThreadID, const char* ThreadName);

//---------------------------------------------------------------------------
class ThreadSafeCounter
{
	DECLARE_PIMPL_NON_COPYABLE(ThreadSafeCounter);

public:
	ThreadSafeCounter();
	ThreadSafeCounter(int _Value);

	int operator *() const;
	int operator ++();
	int operator --();
	int operator +=(int Amount);
	int operator -=(int Amount);
};


//---------------------------------------------------------------------------
class Thread;
DECLARE_SMART_PTR_STRUCT(ThreadHandle);
struct ThreadHandle
{
private:
	DECLARE_PIMPL_NON_COPYABLE(ThreadHandle);

public:
	ThreadHandle(Thread* Thread, int StackSize, char* ThreadName);	
	~ThreadHandle();	
	bool IsValid();
	HANDLE GetNativeHandle();
	void Join();
};

//---------------------------------------------------------------------------
DECLARE_SMART_PTR(Thread);
class Thread
{
	DECLARE_PIMPL_NON_COPYABLE(Thread);

public:
    ThreadInfo* mThreadDesc;
	static void RegisterThread(char* ThreadName);

	Thread();    

    void CreateThread(int StackSize, char* ThreadName);
    void RegisterThread();    

	void StartRun();
	void EndRun();

	bool IsForceExit();
	void ForceExit(bool Wait);
	bool IsRunning();
	void Join();

    // Interface
    virtual bool Init() { return true; }
    virtual bool Run() = 0;
    virtual void Exit() {}
};
}