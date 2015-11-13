#pragma once
#include "FBCommonHeaders/platform.h"
namespace fastbird {
	class IPlatformRenderer;
}
extern "C"{
	FB_PLUGIN_DLL_PUBLIC fastbird::IPlatformRenderer* CreateRenderEngine();
	FB_PLUGIN_DLL_PUBLIC void DeleteRenderEngine(fastbird::IPlatformRenderer* renderer);
}