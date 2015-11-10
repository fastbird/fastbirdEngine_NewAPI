#include "Profiler.h"
#include "FBCommonHeaders/platform.h"
#include <stdarg.h>
#include <fstream>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#endif

namespace fastbird
{
	static std::ofstream profileLogFile("fb_profile_result.log");
	int Profiler::indent = 0;
	Profiler::MSG_STACK Profiler::msgs;

	static void DebugOutput(const char* format, ...){
		va_list args;
		va_start(args, format);
		char buffer[2048];
		vsprintf_s(buffer, format, args);
		va_end(args);
#if defined(_WIN32)
		OutputDebugString(buffer);
#else
		assert(0 && "Not implemented");
#endif
	}

	Profiler::Profiler(const char* name, Timer::TIME_PRECISION* accumulator)
		: mName(name)
		, mAccumulator(accumulator)
	{
		mStartTick = gpTimer->GetTickCount();
		indent++;
	}

	void Profiler::SetAccumulator(Timer::TIME_PRECISION* p)
	{
		mAccumulator = p;
	}

	Profiler::~Profiler()
	{
		indent--;
		Timer::TIME_PRECISION elapsedTime = (gpTimer->GetTickCount() - mStartTick) / (Timer::TIME_PRECISION)std::milli::den;
		char sp[10];
		memset(sp, '\t', 10);
		if (indent > 9)
			sp[9] = 0;
		else
			sp[indent] = 0;
		if (indent != 0)
		{
			char buffer[255];
			sprintf_s(buffer, 255, "%s[Profiler] %s takes %f secs.", sp, mName.c_str(), elapsedTime);
			msgs.push(buffer);
		}
		else
		{
			DebugOutput("%s[Profiler] %s takes %f secs.", sp, mName.c_str(), elapsedTime);
			// print stack
			while (!msgs.empty())
			{
				DebugOutput(msgs.top().c_str());
				if (profileLogFile.is_open()){
					profileLogFile << msgs.top().c_str();
					profileLogFile.flush();

				}
				msgs.pop();
			}
		}
		if (mAccumulator)
			*mAccumulator += elapsedTime;		
	}

	Timer::TIME_PRECISION Profiler::GetDt(){
		return (gpTimer->GetTickCount() - mStartTick) / (Timer::TIME_PRECISION)std::milli::den;
	}

	void Profiler::Reset(){
		mStartTick = gpTimer->GetTickCount();
	}

	//-----------------------------------------------------------------------
	// Profile simple
	ProfilerSimple::ProfilerSimple(const char* name)
		:mName(name)
	{
		mStartTick = gpTimer->GetTickCount();
	}

	Timer::TIME_PRECISION ProfilerSimple::GetDT()
	{
		Timer::TIME_PRECISION dt = (gpTimer->GetTickCount() - mStartTick) / (Timer::TIME_PRECISION)std::milli::den;
		mPrevDT = (dt + mPrevDT) * .5;
		return mPrevDT;
	}

	const char* ProfilerSimple::GetName() const
	{
		return mName;
	}

	void ProfilerSimple::Reset()
	{
		mStartTick = gpTimer->GetTickCount();
	}
}