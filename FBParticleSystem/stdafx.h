#pragma warning (disable : 4251)

#if defined(_WIN32)

#define FB_DLL_PARTICLESYSTEM __declspec(dllexport)
#define FB_DLL_RENDERER __declspec(dllimport)
#define FB_DLL_SCENEMANAGER __declspec(dllimport)
#define FB_DLL_SCENEOBJECTFACTORY __declspec(dllimport)
#define FB_DLL_TIMER __declspec(dllimport)

#else

#endif

#include "FBMathLib/Math.h"
#include "FBCommonHeaders/VectorMap.h"
#include "FBDebugLib/Logger.h"
#include "FBStringLib/StringLib.h"
#include "FBStringLib/StringConverter.h"
#include "FBStringMathLib/StringMathConverter.h"
#include "TinyXmlLib/tinyxml2.h"
#include "FBTimer/Timer.h"

#include <map>