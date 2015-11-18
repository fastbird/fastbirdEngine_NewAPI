#include "stdafx.h"

namespace fastbird{
	void Error(const char* szFmt, ...){
		char buf[2048];
		va_list args;
		va_start(args, szFmt);
		vsprintf_s(buf, 2048, szFmt, args);
		va_end(args);
		Logger::Log(FB_ERROR_LOG_ARG, buf);
	}

	void Log(const char* szFmt, ...){
		char buf[2048];
		va_list args;
		va_start(args, szFmt);
		vsprintf_s(buf, 2048, szFmt, args);
		va_end(args);
		Logger::Log(FB_DEFAULT_LOG_ARG, buf);
	}
}