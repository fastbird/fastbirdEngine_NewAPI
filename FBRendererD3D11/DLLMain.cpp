#include "stdafx.h"
#include "DLLMain.h"
#include "RendererD3D11.h"

fastbird::IRendererPtr Create(){
	// Calling serveral times is fine.
	fastbird::Logger::Init(_T("RendererD3D11.log"));
	return fastbird::RendererD3D11::Create();
}