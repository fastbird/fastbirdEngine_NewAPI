#include "FBCommonHeaders/platform.h"
#if defined(_PLATFORM_WINDOWS_)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define FB_DLL_FILEMONITOR __declspec(dllexport)
#define FB_DLL_TIMER __declspec(dllimport)
#define FB_DLL_FILESYSTEM __declspec(dllimport)
#else
#endif

#include <fstream>
#include <set>
#include <algorithm>

#include "FBCommonHeaders/Types.h"
#include "FBThreadLib/threads.h"
#include "FBDebugLib/DebugLib.h"
#include "FBStringLib/StringLib.h"
#include "FBTimer/Timer.h"
#include "FBFileSystem/FileSystem.h"