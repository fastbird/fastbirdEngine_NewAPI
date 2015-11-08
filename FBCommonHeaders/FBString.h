#pragma once
#include "platform.h"
#include <vector>
#include <string>

typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character
typedef std::vector<std::string> StringVector;
typedef std::vector<std::wstring> WStringVector;

#if defined(UNICODE)
	typedef wchar_t TCHAR;
	typedef const WCHAR* LPCTSTR;	
	typedef std::vector<std::wstring> TStringVector;
	typedef std::wstring TString;
	#define __T(x)      L ## x
	#define _tstrlen wcslen
	#define _tisspace iswspace
	#define _tisdigit iswdigit
	#define _ttolower towlower
	#define _ttoupper towupper
	#define _tvsprintf vswprintf
	#define _tsprintf swprintf
	#define _tofstream wofstream
	#define _tgeneric_string generic_wstring
	#define _tcout wcout
	#define _tcerr wcerr	
	#define _tofstream wofstream
	#define _tifstream wifstream
	#define _tstreambuf wstreambuf
#else
	typedef const char* LPCTSTR;
	typedef char TCHAR;
	typedef std::vector<std::string> TStringVector;
    typedef std::string TString;
	#define __T(x)      x
	#define _tstrlen strlen	
	#define _tisspace isspace
	#define _tisdigit isdigit
	#define _ttolower tolower
	#define _ttoupper toupper
	#define _tvsprintf vsprintf
	#define _tsprintf sprintf
	#define _tofstream ofstream
	#define _tgeneric_string generic_string
	#define _tcout cout
	#define _tcerr cerr	
	#define _tofstream ofstream
	#define _tifstream ifstream
	#define _tstreambuf streambuf
#endif

#if defined(_PLATFORM_MAC_)
static inline int sprintf(TCHAR* str, size_t size, LPCTSTR format, ...){
    int ret = -1;
    va_list args;
    va_start(args, format);
    ret = _tvsprintf(str, format, args);
    va_end(args);
    return ret;    
}
#endif

#define _T(x)       __T(x)
#define _TEXT(x)    __T(x)
