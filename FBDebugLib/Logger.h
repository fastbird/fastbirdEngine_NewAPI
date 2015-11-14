/**
\file FBDebug.h
Debug utility.
\author Jungwan Byun
\defgroup FBDebugLib
A debug library
*/
#pragma once
#include "FBCommonHeaders/String.h"
#include <fstream>
#include <memory>
namespace fastbird{
	/** Collection of debug featreus.
	\ingroup FBDebug
	*/
	class Logger{
	public:
		/** Initialize the log file.
		Prepare the log file. Logs received before the initializing will be sent to
		the debug output widow rather than recorded into the log file.
		@param filepath The new log file path. ex)error.log		
		*/		
		static void Init(const char* filepath);
		static void Init(const WCHAR* filepath);

		/** Close the log file
		Logs received after Debug is released, will be sent to the debug output.
		*/
		static void Release();

		/** Output to the log file created by \b CreateLogFile(). */		
		static void Log(const char* str, ...);		

		/** Output to the log file specified as a parameter. */
		static void Log(std::ofstream& file, const char* str);

		/** Output to the debug window.	*/
		static void Output(const char* str, ...);
	};
}

#define FB_DEFAULT_LOG_ARG "%s: %s\n", __FUNCTION__
#define FB_DEFAULT_LOG_ARG_NO_LINE "%s: %s", __FUNCTION__
#define FB_ERROR_LOG_ARG "%s: (error)%s\n", __FUNCTION__
#define FB_ERROR_LOG_ARG_NO_LINE "%s: (error)%s", __FUNCTION__
