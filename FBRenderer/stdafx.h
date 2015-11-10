/**
\defgroup FBRenderer
Main hub module for rendering. 

Required libraries: \b FBDebugLib, \b FBMath, \b FBSystemLib, \b FBMemoryManagerLib, \b FBStringMathLib, \b FBStringLib
Required modules: \b FBColladaImporter, \b FBRenderableFactory
Optional modules: \b FBRendererD3D11
*/

// Happens when a interface can have not exposed symbols.
// But they are all privates so it is fine.
#pragma warning (disable : 4251)

#include "FBCommonHeaders/platform.h"
#if defined(_PLATFORM_WINDOWS_)
#define FBRendererDLL __declspec(dllexport)
#else
#include "PrefixHeader.pch"
#endif

#include <memory>
#include <algorithm>
#include <vector>
#include <map>
#include <assert.h>
#include <iostream>
#include <cstdarg>

#include "FBMemoryManagerLib/MemoryManager.h"
#include "FBMathLib/Math.h"
#include "FBDebugLib/Logger.h"
#include "FBStringLib/StringLib.h"
#include "FBCommonHeaders/String.h"