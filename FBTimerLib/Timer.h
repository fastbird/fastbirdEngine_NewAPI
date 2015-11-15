#pragma once
#include "FBCommonHeaders/platform.h"
#include <memory>
#include <chrono>
namespace fastbird
{
	DECLARE_SMART_PTR(Timer);
	class FB_DLL_PUBLIC Timer
	{
		DECLARE_PIMPL(Timer);
		Timer();

	public:
		// unsigned int : safe for 828 'days' at 60 frames/sec
		// unsigned long long : safe for 9749040289 'years' at 60 frames/sec
		typedef unsigned int FRAME_PRECISION;
		typedef Real TIME_PRECISION;

		static TimerPtr Create();
		static TimerPtr GetMainTimer();		
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
	/// only declaration. you need to define it.
	extern Timer* gpTimer;
}