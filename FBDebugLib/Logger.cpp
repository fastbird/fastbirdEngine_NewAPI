#include "Logger.h"
#include "FBCommonHeaders/platform.h"
#if defined(_PLATFORM_WINDOWS_)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#endif

#include <iostream>

using namespace fastbird;

static int sInitialized = false;
static std::_tofstream sLogFile;
static std::_tstreambuf* sOriginalErrorStream = 0;

void Logger::Init(LPCTSTR filepath){		
	sLogFile.open(filepath);
	auto errStream = std::_tcerr.rdbuf(sLogFile.rdbuf());
	if (!sOriginalErrorStream){
		sOriginalErrorStream = errStream;
	}
	sInitialized = true;
}

void Logger::Release(){
	sInitialized = false;
	if (sOriginalErrorStream){
		std::_tcerr.rdbuf(sOriginalErrorStream);
		sOriginalErrorStream = 0;
	}
	sLogFile.close();	
}

void Logger::Log(LPCTSTR str, ...){
	static const unsigned BUFFER_SIZE = 2048;	
	if (!str) return;
	auto length = _tstrlen(str);
	if (length == 0) return;
	if (length >= BUFFER_SIZE){
		std::_tcerr << _T("Log message is too long to print and truncated. Maximum 2047 characters are supported.\n");
	}
	TCHAR buffer[BUFFER_SIZE];
	va_list args;
	va_start(args, str);
	_tvsnprintf(buffer, BUFFER_SIZE, str, args);
	va_end(args);

	if (sLogFile.is_open()){
		sLogFile << buffer;
		sLogFile.flush();
	}
	else{
		// fallback
		Output(buffer);
	}
}

void Logger::Log(std::ofstream& file, LPCTSTR str){
	if (file.is_open()){
		file << str;
	}
	else{
		// fallback
		Output(str);
	}
}

void Logger::Output(LPCTSTR str, ...){
	static const unsigned BUFFER_SIZE = 2048;
	if (!str) return;
	auto length = _tstrlen(str);
	if (length == 0) return;
	if (length >= BUFFER_SIZE){
		std::_tcerr << _T("Log message is too long to print and truncated. Maximum 2047 characters are supported.\n");
	}
	TCHAR buffer[BUFFER_SIZE];
	va_list args;
	va_start(args, str);
	_tvsnprintf(buffer, BUFFER_SIZE, str, args);
	va_end(args);
#if defined(_PLATFORM_WINDOWS_)
	OutputDebugString(buffer);
#else
	std::cerr << str;
#endif
}
