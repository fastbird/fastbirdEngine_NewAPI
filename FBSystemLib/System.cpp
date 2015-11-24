#include "stdafx.h"
#include "System.h"

namespace fastbird{
	int GetNumProcessors(){
#if defined(_PLATFORM_WINDOWS_)
		SYSTEM_INFO SI;
		GetSystemInfo(&SI);
		return SI.dwNumberOfProcessors;
#else
		assert(0 && "Not implemented");
		return 4;
#endif
	}

	bool IsWindowForeground(HWindow window){
#if defined(_PLATFORM_WINDOWS_)
		return GetForegroundWindow() == (HWND)window;
#else
		assert(0 && "Not implemented");
		return true;
#endif
	}

	HWindow ForegroundWindow(){
#if defined(_PLATFORM_WINDOWS_)
		return (HWindow)GetForegroundWindow();
#else
		assert(0 && "Not implemented");
		return -1;
#endif
	}

	HWindow WindowFromMousePosition(){
#if defined(_PLATFORM_WINDOWS_)
		POINT pt;
		GetCursorPos(&pt);
		return (HWindow)WindowFromPoint(pt);
#else
		assert(0 && "Not implemented");
		return -1;
#endif
	}
}