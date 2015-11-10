#pragma once
#include "FBCommonHeaders/Types.h"
#include <memory>
#include <chrono>
namespace fastbird
{
	DECLARE_SMART_PTR(Timer);
	class Timer
	{
	public:
		// unsigned int : safe for 828 'days' at 60 frames/sec
		// unsigned long long : safe for 9749040289 'years' at 60 frames/sec
		typedef unsigned int FRAME_PRECISION; 
		typedef Real TIME_PRECISION;

	private:
		std::chrono::high_resolution_clock::time_point mPreviousTimePoint;
		FRAME_PRECISION mFrames;		
		TIME_PRECISION mTime;
		TIME_PRECISION mTimeNotPausable;
		TIME_PRECISION mDeltaTime;
		TIME_PRECISION mDeltaTimeNotPausable;

		bool mPaused;

	public:
		Timer();	
		~Timer();		

		void Tick();
		void Reset();
		TIME_PRECISION GetTime() const;
		TIME_PRECISION GetDeltaTime() const;
		TIME_PRECISION GetDeltaTimeNotPausable() const;		
		FRAME_PRECISION GetFrame() const;
		INT64 GetTickCount();

		void Pause();
		void Resume();
		bool IsPause() const;
		
	};
	extern Timer* gpTimer;
}