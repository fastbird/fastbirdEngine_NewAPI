#include "FBCommonHeaders/platform.h"
#if defined (_PLATFORM_WINDOWS_)
#define FB_DLL_UI __declspec(dllexport)	
#define FB_DLL_CONSOLE __declspec(dllimport)
#define FB_DLL_TIMER __declspec(dllimport)
#define FB_DLL_RENDERER __declspec(dllimport)
#define FB_DLL_INPUTMANAGER __declspec(dllimport)
#define FB_DLL_LUA __declspec(dllimport)
#define FB_DLL_FILEMONITOR __declspec(dllimport)
#define FB_DLL_FILESYSTEM __declspec(dllimport)
#else
#endif

#include <assert.h>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <string>
#include <algorithm>
#include <functional>
#include <sstream>

#include "FBMathLib/Math.h"
#include "FBStringLib/StringLib.h"
#include "FBStringLib/StringConverter.h"
#include "FBStringMathLib/StringMathConverter.h"
#include "FBDebugLib/DebugLib.h"
#include "FBTimer/Timer.h"
#include "FBTimer/Profiler.h"
#include "TinyXmlLib/tinyxml2.h"
#include "FBRenderer/Renderer.h"
#include "FBRenderer/Material.h"
#include "FBRenderer/Font.h"
#include "FBInputManager/IInputConsumer.h"
#include "FBInputManager/TextManipulator.h"
#include "FBInputManager/InputManager.h"
#include "FBLua/LuaObject.h"
#include "FBMemoryManagerLib/MemoryManager.h"
#include "FBFileSystem/FileSystem.h"
#include "FBSystemLib/System.h"
#include "FBFileMonitor/FileMonitor.h"
#include "ComponentType.h"
#include "Align.h"

namespace fastbird{
	void Log(const char* fmt, ...);
	void Error(const char* fmt, ...);
}