#include "FBCommonHeaders/platform.h"
#if defined(_PLATFORM_WINDOWS_)
#define FB_DLL_ENGINEFACADE __declspec(dllexport)
#define FB_DLL_RENDERER __declspec(dllimport)
#define FB_DLL_LUA __declspec(dllimport)
#define FB_DLL_SCENEMANAGER __declspec(dllimport)
#define FB_DLL_SCENEOBJECTFACTORY __declspec(dllimport)
#else
#define FB_DLL_ENGINEFACADE 
#endif

#include "FBRenderer/Renderer.h"
#include "FBLua/LuaUtils.h"