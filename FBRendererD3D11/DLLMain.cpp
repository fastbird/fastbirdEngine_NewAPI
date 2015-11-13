#include "stdafx.h"
#include "DLLMain.h"
#include "RendererD3D11.h"

fastbird::IPlatformRenderer* CreateRenderEngine(){
	// Calling serveral times is fine.
	fastbird::Logger::Init("RendererD3D11.log");
	return fastbird::RendererD3D11::Create();
}

void DeleteRenderEngine(fastbird::IPlatformRenderer* renderer){
	fastbird::RendererD3D11::Delete();
}