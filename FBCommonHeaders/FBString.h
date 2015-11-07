#pragma once
#include <vector>
#include <string>
#include <tchar.h>
#if !defined(_MAC)
	typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character
#else
	// some Macintosh compilers don't define wchar_t in a convenient location, or define it as a char
	typedef unsigned short WCHAR;    // wc,   16-bit UNICODE character
#endif

typedef std::vector<std::string> StringVector;
typedef std::vector<std::wstring> WStringVector;

#if defined(UNICODE)
	typedef wchar_t TCHAR;
	typedef const WCHAR* LPCTSTR;
	typedef std::vector<std::wstring> TStringVector;
	typedef std::wstring TString;
	#define _tstrlen wcslen
	#define _tisspace iswspace
	#define _tisdigit iswdigit
	#define _ttolower towlower
	#define _ttoupper towupper
	#define _tvsprintf vswprintf
	#define _tsprintf swprintf
	#define _tofstream wofstream
	#define _tgeneric_string generic_wstring
	#define __T(x)      L ## x
#else
	typedef const char* LPCTSTR;
	typedef char TCHAR;
	typedef std::vector<std::string> TStringVector;
	#define _tstrlen strlen	
	#define _tisspace isspace
	#define _tisdigit isdigit
	#define _ttolower tolower
	#define _ttoupper toupper
	#define _tvsprintf vsprintf
	#define _tsprintf sprintf
	#define _tofstream ofstream
	#define _tgeneric_string generic_string
	#define __T(x)      x
#endif

#define _T(x)       __T(x)
#define _TEXT(x)    __T(x)
