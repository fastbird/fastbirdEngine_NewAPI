#pragma once
#include "../FBCommonHeaders/FBString.h"
#include <fstream>
#include <memory>
namespace fastbird{
	/** Collection of debug featreus. */
	class Debug{
	public:
		enum FileHandling{
			OverWrite,
			BackUp,
		};
		/** Initialize the log file.
		Prepare the log file. Logs received before the initializing will be sent to
		the debug output widow rather than recorded into the log file.
		@param filepath The new log file path. ex)error.log
		@param handling \b Decide handling method for old log files. Old log files will be preserved
		as the number of \b numKeeping by appending sequential numbers at the end of file name.
		@param numKeeping Decide how many backup will be preserved. If the number of old logs 
		exceeded this amount, the oldest one will be deleted.
		*/		
		static void Init(LPCTSTR filepath, FileHandling handling, unsigned numKeeping);

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