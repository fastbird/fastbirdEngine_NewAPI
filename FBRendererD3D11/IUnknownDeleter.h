#pragma once
#include <D3D11.h>
namespace fastbird{
	struct IUnknownDeleter{
		void operator()(IUnknown* p);
	};
}