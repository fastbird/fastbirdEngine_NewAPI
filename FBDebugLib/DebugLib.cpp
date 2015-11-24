#include "DebugLib.h"
#include "FBCommonHeaders/platform.h"
#if defined(_PLATFORM_WINDOWS_)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#endif
namespace fastbird{
	void FBDebugBreak(){
#if defined(_DEBUG) && defined(_PLATFORM_WINDOWS_)
		DebugBreak();
#else
#endif
	}
}