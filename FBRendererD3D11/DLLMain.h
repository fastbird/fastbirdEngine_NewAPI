#pragma once
#if !defined(FBPlugin)
#define FBPlugin __declspec(dllimport)
#endif
namespace fastbird {
	class IRenderer;
}
extern "C"{
	FBPlugin fastbird::IRenderer* CreateRenderEngine();
	FBPlugin void DeleteRenderEngine(fastbird::IRenderer* renderer);
}