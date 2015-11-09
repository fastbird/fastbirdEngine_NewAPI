#include "FBDebug.h"
#include "FBCommonHeaders/platform.h"
#include "FBStringLib/FBStringLib.h"
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

void Debug::Init(LPCTSTR filepath){		
	sLogFile.open(filepath);
	auto errStream = std::_tcerr.rdbuf(sLogFile.rdbuf());
	if (!sOriginalErrorStream){
		sOriginalErrorStream = errStream;
	}
	sInitialized = true;
}

void Debug::Release(){
	sInitialized = false;
	if (sOriginalErrorStream){
		std::_tcerr.rdbuf(sOriginalErrorStream);
		sOriginalErrorStream = 0;
	}
	sLogFile.close();	
}

void Debug::Log(LPCTSTR str){
	if (sLogFile.is_open()){
		sLogFile << str;
		sLogFile.flush();
	}
	else{	
		// fallback
		Output(str);
	}
}
void Debug::Log(std::ofstream& file, LPCTSTR str){
	if (file.is_open()){
		file << str;
	}
	else{
		// fallback
		Output(str);
	}
}

void Debug::Output(LPCTSTR str){
#if defined(_PLATFORM_WINDOWS_)
	OutputDebugString(str);	
#else
	std::cerr << str;
#endif
}
