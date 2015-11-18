#pragma comment(lib, "libogg.lib")
#pragma comment(lib, "libtheora.lib")

#include "FBDebugLib/Logger.h"
#include "FBStringLib/StringLib.h"

namespace fastbird{
	void Error(const char* szFmt, ...);
	void Log(const char* szFmt, ...);
}