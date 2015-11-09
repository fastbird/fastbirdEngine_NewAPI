#pragma once

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#ifdef _DEBUG
#pragma comment(lib, "d3dx11d.lib")
#else
#pragma comment(lib, "d3dx11d.lib")
#endif

#include <D3DX11.h>