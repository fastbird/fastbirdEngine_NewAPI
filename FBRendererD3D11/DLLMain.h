#pragma once
namespace fastbird {
	class IRenderer;
}
extern "C"{
	FBRendererD3D11DLL fastbird::IRenderer* CreateRenderEngine();
	FBRendererD3D11DLL void DeleteRenderEngine(fastbird::IRenderer* renderer);
}