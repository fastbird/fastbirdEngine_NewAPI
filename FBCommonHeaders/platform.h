/**
\defgroup FBCommonHeaders
A group containing header files only.
*/
#if defined(_WIN32)
	#define _PLATFORM_WINDOWS_
#elif defined(__APPLE__)
	#include "TargetConditionals.h"
	#if TARGET_IPHONE_SIMULATOR
		#error "Platform is not supported"
	#elif TARGET_OS_IPHONE
		#error "Platform is not supported"
	#elif TARGET_OS_MAC
		#define _PLATFORM_MAC_
	#else
	#   error "Unknown Apple platform"
	#endif
#else
	#error "Platform is not supported"
#endif
