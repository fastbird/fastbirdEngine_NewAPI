#include "stdafx.h"
#include "AsyncObjects.h"
using namespace fastbird;

//---------------------------------------------------------------------------
FB_CRITICAL_SECTION::FB_CRITICAL_SECTION()
{
}

FB_CRITICAL_SECTION::~FB_CRITICAL_SECTION()
{
}

void FB_CRITICAL_SECTION::Lock()
{
	mMutex.lock();
}

void FB_CRITICAL_SECTION::Unlock()
{
	mMutex.unlock();
}

//---------------------------------------------------------------------------
LOCK_CRITICAL_SECTION::LOCK_CRITICAL_SECTION(FB_CRITICAL_SECTION* cs)
	:mCS(cs)
{
	mCS->Lock();
}
LOCK_CRITICAL_SECTION::LOCK_CRITICAL_SECTION(FB_CRITICAL_SECTION& cs)
	:mCS(&cs)
{
	mCS->Lock();
}
LOCK_CRITICAL_SECTION::~LOCK_CRITICAL_SECTION()
{
	mCS->Unlock();
}

//---------------------------------------------------------------------------
FB_READ_WRITE_CS::FB_READ_WRITE_CS()
	: mNumReaders(0)
{
	mReaderCleared.notify_all();
}
FB_READ_WRITE_CS::~FB_READ_WRITE_CS()
{
}
void FB_READ_WRITE_CS::EnterReader()
{
	std::lock_guard<std::mutex> lk(mWriteMutex);	
	LOCK_CRITICAL_SECTION lockReadCount(mReaderCountCS);
	++mNumReaders;	
}
void FB_READ_WRITE_CS::LeaveReader()
{
	LOCK_CRITICAL_SECTION lockReadCount(mReaderCountCS);	
	--mNumReaders;
	if (mNumReaders == 0){
		mReaderCleared.notify_all();
	}
}
void FB_READ_WRITE_CS::EnterWriter()
{
	mPendingLock = std::unique_lock<std::mutex>(mWriteMutex);	 
	mReaderCleared.wait(mPendingLock, [&]{return mNumReaders == 0; });
}
void FB_READ_WRITE_CS::LeaveWriter()
{
	mPendingLock.unlock();
}

//---------------------------------------------------------------------------
READ_LOCK::READ_LOCK(FB_READ_WRITE_CS& lock)
	: mReadWrite(&lock)
{
	mReadWrite->EnterReader();
}
READ_LOCK::~READ_LOCK()
{
	mReadWrite->LeaveReader();
}

//---------------------------------------------------------------------------
WRITE_LOCK::WRITE_LOCK(FB_READ_WRITE_CS& lock)
	: mReadWrite(&lock)
{
	mReadWrite->EnterWriter();
}
WRITE_LOCK::~WRITE_LOCK()
{
	mReadWrite->LeaveWriter();
}

#if defined(_PLATFORM_WINDOWS_)
class SyncEventWin : public SyncEvent
{
protected:
	HANDLE EventHandle;

public:
	SyncEventWin(bool ManualReset, char* Name)
	{
		
		EventHandle = CreateEventA(NULL, ManualReset, 0, Name);
		assert(EventHandle);
	}

	~SyncEventWin()
	{
		CloseHandle(EventHandle);
	}

	void Trigger()
	{
		SetEvent(EventHandle);
	}

	void Reset()
	{
		ResetEvent(EventHandle);
	}

	bool Wait(DWORD WaitTime)
	{
		return WaitForSingleObject(EventHandle, WaitTime) == WAIT_OBJECT_0;
	}

	void Lock()
	{
		WaitForSingleObject(EventHandle, INFINITE);
	}
};
#else
#endif

SyncEventPtr fastbird::CreateSyncEvent(bool ManualReset, char* Name)
{
#if defined(_PLATFORM_WINDOWS_)
	return SyncEventPtr(new SyncEventWin(ManualReset, Name), [](SyncEventWin* obj){ delete obj; });
#else
	assert(0 && " Not implemented.");
	return 0;
#endif
}