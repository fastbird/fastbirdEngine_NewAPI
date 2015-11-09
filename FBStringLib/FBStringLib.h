/**
\file FBStringLib.h
Provide string manipulators
\author Jungwan Byun
\defgroup FBStringLib
Provide string manipulators
*/
#pragma once
#include "FBCommonHeaders/FBString.h"
#include <vector>
namespace fastbird{
	///@addtogroup FBStringLib
	///@{

	/// Replace all occurences of \a target in \a s to \a replace
	void ReplaceCharacter(TCHAR* s, TCHAR target, TCHAR replace);	
	TCHAR* StripRight(TCHAR* s);
	TCHAR* StripLeft(TCHAR* s);
	TString StripBoth(LPCTSTR s);	
	LPCTSTR FindLastOf(LPCTSTR s, TCHAR ch);
	void StepToDigit(TCHAR** ppch);
	/** Compare two strings. This function ignores the case.
	@return 0 if two strings are same. 1 if \b lhs > \b rhs. -1 if \nlhs < \b rhs
	*/
	int StringCompareNoCase(const TString& lhs, const TString& rhs);
	/** Check whether the \b str is start with the \b pattern.
	@param lowerCase if true, the \b str will be lowered before checking.
	*/
	bool StartsWith(const TString& str, const TString& pattern, bool lowerCase = true);
	
	void ToLowerCase(TString& str);	
	void ToUpperCase(TString& str);
	
	bool IsNumeric(LPCTSTR str);
	LPCTSTR FormatString(LPCTSTR str, ...);	

	//-----------------------------------------------------------------------
	// UNICODE conversion
	enum EUnicodeByteOrder	{
		FB_LITTLE_ENDIAN,
		FB_BIG_ENDIAN,
	};

	/// This function will attempt to decode a UTF-8 encoded character in the buffer.
	/// If the encoding is invalid, the function returns -1.
	int DecodeUTF8(const unsigned char *encodedBuffer, unsigned int *outCharLength);

	/// This function will encode the value into the buffer.
	/// If the value is invalid, the function returns -1, else the encoded length.
	int EncodeUTF8(unsigned int value, unsigned char *outEncodedBuffer, unsigned int *outCharLength);

	/// This function will attempt to decode a UTF-16 encoded character in the buffer.
	/// If the encoding is invalid, the function returns -1.
	int DecodeUTF16(const unsigned char *encodedBuffer, unsigned int *outCharLength, EUnicodeByteOrder byteOrder = FB_LITTLE_ENDIAN);

	/// This function will encode the value into the buffer.
	/// If the value is invalid, the function returns -1, else the encoded length.
	int EncodeUTF16(unsigned int value, unsigned char *outEncodedBuffer, unsigned int *outCharLength, EUnicodeByteOrder byteOrder = FB_LITTLE_ENDIAN);

	/// return data is temporary data. save it to other memory if you need
	unsigned char* AnsiToUTF8(const char* source);

	/// return data is temporary data. save it to other memory if you need
	WCHAR* AnsiToWide(const char* source, int size);
	WCHAR* AnsiToWide(const char* source);
	WCHAR AnsiToWide(const char source);

	// return data is temporary data. save it to other memory if you need
	WCHAR* UTF8ToWide(const unsigned char* source);

	const char* WideToAnsi(const WCHAR* source);
	///@}
}