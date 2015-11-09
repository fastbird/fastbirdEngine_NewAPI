#include "stdafx.h"
#include "ResourceBinder.h"
#include "RendererD3D11.h"

namespace fastbird{
	void ResourceBinder::SetIndexBuffer(IndexBufferD3D11* indexBuffer){
		RendererD3D11::GetInstance().SetIndexBuffer(indexBuffer);
	}
}