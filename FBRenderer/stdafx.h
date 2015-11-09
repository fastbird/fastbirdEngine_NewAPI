/**
\defgroup FBRenderer
Main hub module for rendering. 

Required libraries: \b FBDebugLib, \b FBMath, \b FBSystemLib, \b FBMemoryManagerLib, \b FBStringMathLib, \b FBStringLib
Required modules: \b FBColladaImporter, \b FBRenderableFactory
Optional modules: \b FBRendererD3D11
*/

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
#include "FBMath/Math.h"
#include "FBDebugLib/Logger.h"
#include "FBStringLib/StringLib.h"
#include "FBCommonHeaders/String.h"

// convenient functions
namespace fastbird{
	void Error(LPCTSTR format, ...);
	void Log(LPCTSTR format, ...);
}