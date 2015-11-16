#pragma once
#include <stack>
#include "FBCommonHeaders/platform.h"
#include "Timer.h"
namespace fastbird
{
	class FB_DLL_PUBLIC Profiler
	{
		static int indent;
		typedef std::stack<std::string> MSG_STACK;
		static  MSG_STACK msgs;
		std::string mName;
		INT64 mStartTick;
		TIME_PRECISION* mAccumulator;		

	public:
		Profiler(const char* name);
		Profiler(const char* name, TIME_PRECISION* accumulator);
		~Profiler();

		void SetAccumulator(TIME_PRECISION* p);
		TIME_PRECISION GetDt();
		void Reset();	
	};

	class FB_DLL_PUBLIC ProfilerSimple
	{
		const char* mName;
		INT64 mStartTick;
		TIME_PRECISION mPrevDT;

	public:
		ProfilerSimple(const char* name);
		TIME_PRECISION GetDT();
		const char* GetName() const;
		void Reset();		
	};
}