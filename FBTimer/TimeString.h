#pragma once
#include "FBCommonHeaders/platform.h"
#include <string>
namespace fastbird
{
	class FB_DLL_PUBLIC TimeString{
	public:
		static std::string GetTimeString();
		static std::string GetTimeStringForFileName();
	};
	
}