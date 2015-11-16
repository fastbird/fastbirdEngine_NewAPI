/**
\defgroup FBSceneManager
Scene manage the objects visible on the render target.

Required libraries: 
Required modules: 
Plugin: 
*/

// Happens when a interface can have not exposed symbols.
// But they are all privates so it is fine.
#pragma warning (disable : 4251)
#define FBDLLProject
#include "FBCommonHeaders/platform.h"
#if defined(_PLATFORM_WINDOWS_)
#else
#include "PrefixHeader.pch"
#endif

#include <memory>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

#include "FBDebugLib/Logger.h"