#include "stdafx.h"
namespace fastbird{
	void Error(LPCTSTR str, ...){
		static const unsigned BUFFER_SIZE = 2048;
		if (!str) return;
		auto length = _tstrlen(str);
		if (length == 0) return;
		if (length >= BUFFER_SIZE){
			std::_tcerr << _T("Log message is too long to print and truncated. Maximum 2047 characters are supported.\n");
		}

		TCHAR buffer[BUFFER_SIZE] = _T("(error) ");
		auto len = _tstrlen(buffer);
		va_list args;
		va_start(args, str);
		_tvsnprintf(buffer + len, BUFFER_SIZE, str, args);
		va_end(args);
		buffer[BUFFER_SIZE - 1] = 0;
		Logger::Log(buffer);
	}

	void Log(LPCTSTR str, ...){
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
		Logger::Log(buffer);
	}
}