/**
\file FBDebug.h
Debug utility.
\author fastbird @ fastbird dev studio
\defgroup FBDebugLib
A debug library
*/
#pragma once
#include "../FBCommonHeaders/FBString.h"
#include <fstream>
#include <memory>
namespace fastbird{
	/** Collection of debug featreus.
	\ingroup FBDebug
	*/
	class Debug{
	public:
		/** Initialize the log file.
		Prepare the log file. Logs received before the initializing will be sent to
		the debug output widow rather than recorded into the log file.
		@param filepath The new log file path. ex)error.log		
		*/		
		static void Init(LPCTSTR filepath);

		/** Close the log file
		Logs received after Debug is released, will be sent to the debug output.
		*/
		static void Release();

		/** Output to the log file created by \b CreateLogFile(). */
		static void Log(LPCTSTR str);

		/** Output to the log file specified as a parameter. */
		static void Log(std::ofstream& file, LPCTSTR str);

		/** Output to the debug window.	*/
		static void Output(LPCTSTR str);
	};
}