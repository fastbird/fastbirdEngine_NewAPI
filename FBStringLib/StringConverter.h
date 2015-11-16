#pragma once
#include "FBCommonHeaders/String.h"
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	class StringConverter{
	public:
		static TString ToString(Real val, unsigned short precision = 6,
			unsigned short width = 0, char fill = ' ', 
			std::ios::fmtflags flags = std::ios::fmtflags(0));
		static TString ToString(int val, unsigned short width = 0,
			char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
		static TString ToString(size_t val, unsigned short width = 0, 
			char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));		
		static TString ToString(unsigned long val, unsigned short width = 0, 
			char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
		static TString ToStringK(unsigned val);		
		static TString ToString(long val, unsigned short width = 0, char fill = ' ',
			std::ios::fmtflags flags = std::ios::fmtflags(0));
		/** Converts a boolean to a String.
		\param \a yesNo If set to true, result is 'yes' or 'no' instead of 'true' or 'false'
		*/
		static TString ToString(bool val, bool yesNo = false);
		/// Should not contain a ' ' in the each string elements.
		TString StringConverter::ToString(const TStringVector& val);

		static Real ParseReal(const TString& val, Real defaultValue = 0);
		static int ParseInt(const TString& val, int defaultValue = 0);
		static unsigned int ParseUnsignedInt(const TString& val, unsigned int defaultValue = 0);
		static unsigned int ParseHexa(const TString& val, unsigned int defaultValue = 0);
		static long ParseLong(const TString& val, long defaultValue = 0);
		static unsigned long ParseUnsignedLong(const TString& val, unsigned long defaultValue = 0);
		static unsigned long long ParseUnsignedLongLong(const TString& val, unsigned long long defaultValue = 0);
		/** Converts a TString to a boolean.
		@remarks
		Returns true if case-insensitive match of the start of the string
		matches "true", "yes" or "1", false otherwise.
		*/
		static bool ParseBool(const TString& val, bool defaultValue = 0);

		/** Checks the TString is a valid number value. */
		static bool IsNumber(const TString& val);
		static TStringVector ParseStringVector(const TString& val);		
		
	};
}