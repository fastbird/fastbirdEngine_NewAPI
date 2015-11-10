#include "Timer.h"

using namespace std::chrono;
namespace fastbird
{
	Timer* gpTimer = 0;

	Timer::Timer()
		: mFrames(0)
		, mPaused(false)
	{
		Reset();
	}

	Timer::~Timer()
	{
	}

	void Timer::Tick()
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

	void Timer::Reset()
	{
		mPreviousTimePoint = high_resolution_clock::now();
		mTime = 0;
		mDeltaTime = 0;
		mTimeNotPausable = 0;
		mDeltaTimeNotPausable = 0;
	}

	Timer::TIME_PRECISION Timer::GetDeltaTime() const
	{
		return mDeltaTime < 0.4f ? mDeltaTime : 0.4f;
	}

	Timer::TIME_PRECISION Timer::GetDeltaTimeNotPausable() const
	{
		return mDeltaTimeNotPausable < 0.4f ? mDeltaTimeNotPausable : 0.4f;
	}

	Timer::TIME_PRECISION Timer::GetTime() const
	{
		return mTime;
	}

	INT64 Timer::GetTickCount()
	{
		return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
	}

	void Timer::Pause()
	{
		mPaused = true;
		mDeltaTime = 0.f;
	}

	void Timer::Resume()
	{
		mPaused = false;
	}
}