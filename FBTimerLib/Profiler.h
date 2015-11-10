#pragma once
#include <stack>
#include "FBCommonHeaders/Types.h"
#include "Timer.h"
namespace fastbird
{
	class Profiler
	{
		static int indent;
		typedef std::stack<std::string> MSG_STACK;
		static  MSG_STACK msgs;
		std::string mName;
		INT64 mStartTick;
		Timer::TIME_PRECISION* mAccumulator;		

	public:
		Profiler(const char* name);
		Profiler(const char* name, Timer::TIME_PRECISION* accumulator);
		~Profiler();

		void SetAccumulator(Timer::TIME_PRECISION* p);
		Timer::TIME_PRECISION GetDt();
		void Reset();	
	};

	class ProfilerSimple
	{
		const char* mName;
		INT64 mStartTick;
		Timer::TIME_PRECISION mPrevDT;

	public:
		ProfilerSimple(const char* name);
		Timer::TIME_PRECISION GetDT();
		const char* GetName() const;
		void Reset();		
	};
}