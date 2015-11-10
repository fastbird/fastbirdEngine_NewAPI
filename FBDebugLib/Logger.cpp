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
static std::ofstream sLogFile;
static std::streambuf* sOriginalErrorStream = 0;

void Logger::Init(const char* filepath){		
	sLogFile.open(filepath);
	auto errStream = std::cerr.rdbuf(sLogFile.rdbuf());
	if (!sOriginalErrorStream){
		sOriginalErrorStream = errStream;
	}
	sInitialized = true;
}

void Logger::Init(const WCHAR* filepath){
	sLogFile.open(filepath);
	auto errStream = std::cerr.rdbuf(sLogFile.rdbuf());
	if (!sOriginalErrorStream){
		sOriginalErrorStream = errStream;
	}
	sInitialized = true;
}

void Logger::Release(){
	sInitialized = false;
	if (sOriginalErrorStream){
		std::cerr.rdbuf(sOriginalErrorStream);
		sOriginalErrorStream = 0;
	}
	sLogFile.close();	
}

void Logger::Log(const char* str, ...){
	static const unsigned BUFFER_SIZE = 2048;	
	if (!str) return;
	auto length = strlen(str);
	if (length == 0) return;
	if (length >= BUFFER_SIZE){
		std::cerr << "Log message is too long to print and truncated. Maximum 2047 characters are supported.\n";
	}
	char buffer[BUFFER_SIZE];
	va_list args;
	va_start(args, str);
	vsprintf_s(buffer, str, args);
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

void Logger::Log(std::ofstream& file, const char* str){
	if (file.is_open()){
		file << str;
	}
	else{
		// fallback
		Output(str);
	}
}

void Logger::Output(const char* str, ...){
	static const unsigned BUFFER_SIZE = 2048;
	if (!str) return;
	auto length = strlen(str);
	if (length == 0) return;
	if (length >= BUFFER_SIZE){
		std::cerr << "Log message is too long to print and truncated. Maximum 2047 characters are supported.\n";
	}
	char buffer[BUFFER_SIZE];
	va_list args;
	va_start(args, str);
	vsprintf_s(buffer, str, args);
	va_end(args);
#if defined(_PLATFORM_WINDOWS_)
	OutputDebugStringA(buffer);
#else
	std::cerr << str;
#endif
}
