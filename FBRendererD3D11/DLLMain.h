#pragma once
#include "FBCommonHeaders/platform.h"
namespace fastbird {
	class IRenderer;
}
extern "C"{
	FB_PLUGIN_DLL_PUBLIC fastbird::IRenderer* CreateRenderEngine();
	FB_PLUGIN_DLL_PUBLIC void DeleteRenderEngine(fastbird::IRenderer* renderer);
}