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
}