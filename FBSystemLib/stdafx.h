/**
\defgroup FBSystemLib
Perform platform specific task like LoadModule. required(\b FBDebugLib)
*/

#include "FBCommonHeaders/platform.h"
#if defined(_PLATFORM_WINDOWS_)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#endif