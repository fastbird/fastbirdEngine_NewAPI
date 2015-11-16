#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird
{
	std::string GetClipboardDataAsString(HWindow hwnd);
	void SetClipboardStringData(HWindow hwnd, const char* data);
}