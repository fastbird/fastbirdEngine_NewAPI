/*
 -----------------------------------------------------------------------------
 This source file is part of fastbird engine
 For the latest info, see http://www.jungwan.net/
 
 Copyright (c) 2013-2015 Jungwan Byun
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
*/

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
	std::cerr << buffer;
#else
	std::cerr << str;
#endif
}
