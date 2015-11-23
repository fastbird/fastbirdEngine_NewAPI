#include "FBCommonHeaders/platform.h"
#if defined(_PLATFORM_WINDOWS_)
#define FB_DLL_PHYSICS __declspec(dllexport)
#else
#endif

#ifdef _DEBUG
#pragma comment(lib, "BulletCollision_vs2010_debug.lib")
#pragma comment(lib, "BulletDynamics_vs2010_debug.lib")
#pragma comment(lib, "LinearMath_vs2010_debug.lib")
#else
#pragma comment(lib, "BulletCollision_vs2010.lib")
#pragma comment(lib, "BulletDynamics_vs2010.lib")
#pragma comment(lib, "LinearMath_vs2010.lib")
#endif

#include <iostream>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btTransform.h>

#include "mathConv.h"
#include "FBDebugLib/Logger.h"
#include "FBMemoryManagerLib/MemoryManager.h"
#include "FBMathLib/Math.h"
#include "FBStringLib/StringLib.h"
#include "FBStringLib/StringConverter.h"
#include "FBStringMathLib/StringMathConverter.h"
#include "FBCommonHeaders/VectorMap.h"
#include "TinyXmlLib/tinyxml2.h"