#include "stdafx.h"
#include "Timer.h"

using namespace std::chrono;
namespace fastbird
{
	Timer* gpTimer = 0;
	class Timer::Impl{
	public:
		std::chrono::high_resolution_clock::time_point mPreviousTimePoint;
		FRAME_PRECISION mFrames;
		TIME_PRECISION mTime;
		TIME_PRECISION mTimeNotPausable;
		TIME_PRECISION mDeltaTime;
		TIME_PRECISION mDeltaTimeNotPausable;
		bool mPaused;

		//---------------------------------------------------------------------------
		Impl()
			: mFrames(0)
			, mPaused(false){
			Reset();
		}

		void Tick()
		{
			auto now = high_resolution_clock::now();
			std::chrono::duration<TIME_PRECISION> diff = now - mPreviousTimePoint;
			mPreviousTimePoint = now;
			mTimeNotPausable += diff.count();
			mDeltaTimeNotPausable = diff.count();
			mTimeNotPausable += mDeltaTimeNotPausable;

			++mFrames;
			if (mPaused)
				return;

			TIME_PRECISION previousTime = mTime;
			mTime += mDeltaTimeNotPausable;
			mDeltaTime = mDeltaTimeNotPausable;
		}

		void Reset()
		{
			mPreviousTimePoint = high_resolution_clock::now();
			mTime = 0;
			mDeltaTime = 0;
			mTimeNotPausable = 0;
			mDeltaTimeNotPausable = 0;
		}

		TIME_PRECISION GetDeltaTime() const
		{
			return mDeltaTime < 0.4f ? mDeltaTime : 0.4f;
		}

		TIME_PRECISION GetDeltaTimeNotPausable() const
		{
			return mDeltaTimeNotPausable < 0.4f ? mDeltaTimeNotPausable : 0.4f;
		}

		TIME_PRECISION GetTime() const
		{
			return mTime;
		}

		FRAME_PRECISION GetFrame() const{
			return mFrames;
		}

		INT64 GetTickCount()
		{
			return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
		}

		void Pause()
		{
			mPaused = true;
			mDeltaTime = 0.f;
		}

		void Resume()
		{
			mPaused = false;
		}
	};

	//---------------------------------------------------------------------------
	TimerPtr sTimer;
	TimerPtr Timer::Create(){
		auto timer = TimerPtr(new Timer, [](Timer* obj){ delete obj; });
		if (!sTimer){
			sTimer = timer;
		}
		return timer;
	}

	TimerPtr Timer::GetMainTimer(){
		if (!sTimer){
			Create();
		}
		return sTimer;
	}

	Timer::Timer()
		: mImpl(new Impl)
	{
	}

	Timer::~Timer()
	{
	}

	void Timer::Tick()
	{
		mImpl->Tick();
	}

	void Timer::Reset()
	{
		mImpl->Reset();
	}

	TIME_PRECISION Timer::GetDeltaTime() const
	{
		return mImpl->GetDeltaTime();
	}

	TIME_PRECISION Timer::GetDeltaTimeNotPausable() const
	{
		return mImpl->GetDeltaTimeNotPausable();
	}

	TIME_PRECISION Timer::GetTime() const
	{
		return mImpl->GetTime();
	}

	Timer::FRAME_PRECISION Timer::GetFrame() const{
		return mImpl->GetFrame();
	}

	INT64 Timer::GetTickCount()
	{
		return mImpl->GetTickCount();
	}

	void Timer::Pause()
	{
		mImpl->Pause();
	}

	void Timer::Resume()
	{
		mImpl->Resume();
	}
}