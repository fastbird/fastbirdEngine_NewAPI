/**
\defgroup FBRendererD3D11
Direct3D 11 renderer

Required libraries: \b FBMemoryManager, \b FBDebugLib, \b FBMath
*/
#pragma once

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#ifdef _DEBUG
#pragma comment(lib, "d3dx11d.lib")
#else
#pragma comment(lib, "d3dx11d.lib")
#endif
#define NOMINMAX
#include <memory>
#include <iostream>
#include <D3DX11.h>

#include "FBMath/Math.h"
#include "FBMemoryManagerLib/MemoryManager.h"
#include "FBDebugLib/Logger.h"

// convenient functions
namespace fastbird{
	void Error(LPCTSTR format, ...);
	void Log(LPCTSTR format, ...);
}

#define SAFE_RELEASE(x) (x) ? (x)->Release() : 0; (x)=0