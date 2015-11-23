#pragma once
#include <mutex>
#include <atomic>
#include <condition_variable>
namespace fastbird
{
	//---------------------------------------------------------------------------
	struct FB_CRITICAL_SECTION
	{
	private:
		std::mutex mMutex;
	public:
		FB_CRITICAL_SECTION();
		~FB_CRITICAL_SECTION();
		void Lock();
		void Unlock();	
	};

	//---------------------------------------------------------------------------
	struct LOCK_CRITICAL_SECTION
	{
		FB_CRITICAL_SECTION* mCS;

		LOCK_CRITICAL_SECTION(FB_CRITICAL_SECTION* cs);
		LOCK_CRITICAL_SECTION(FB_CRITICAL_SECTION& cs);
		~LOCK_CRITICAL_SECTION();
	};

	//---------------------------------------------------------------------------
	struct FB_READ_WRITE_CS
	{
	private:
		std::mutex mWriteMutex;		
		std::condition_variable mReaderCleared;
		std::unique_lock<std::mutex> mPendingLock;
		FB_CRITICAL_SECTION mReaderCountCS;
		volatile int mNumReaders;

	public:
		FB_READ_WRITE_CS();
		~FB_READ_WRITE_CS();
		void EnterReader();
		void LeaveReader();
		void EnterWriter();
		void LeaveWriter();
	};

	//-----------------------------------------------------------------------
	struct READ_LOCK
	{
		FB_READ_WRITE_CS* mReadWrite;

		READ_LOCK(FB_READ_WRITE_CS& lock);
		~READ_LOCK();
	};

	struct WRITE_LOCK
	{
		FB_READ_WRITE_CS* mReadWrite;

		WRITE_LOCK(FB_READ_WRITE_CS& lock);
		~WRITE_LOCK();
	};

	DECLARE_SMART_PTR(SyncEvent);
	class SyncEvent
	{
	public:
		virtual ~SyncEvent() {}				
		virtual void Trigger() = 0;
		virtual void Reset() = 0;
		virtual void Lock() = 0;
		virtual bool Wait(DWORD WaitTime = 0xffffffff) = 0;
	};

	SyncEventPtr CreateSyncEvent(bool ManualReset = FALSE, char* Name = NULL);
}

