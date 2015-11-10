#include "stdafx.h"
#include "VertexBufferD3D11.h"
#include "ResourceBinder.h"
#include "ResourceMapper.h"

namespace fastbird
{
	VertexBufferD3D11::VertexBufferD3D11(ID3D11Buffer* vertexBuffer)
		: mVertexBuffer(vertexBuffer)
	{
	}

	VertexBufferD3D11::~VertexBufferD3D11()
	{
		SAFE_RELEASE(mVertexBuffer);
	}

	void VertexBufferD3D11::Bind(){
		ResourceBinder::Bind(this);
	}

	bool VertexBufferD3D11::IsReady() const
	{
		return mVertexBuffer != 0;
	}

	ID3D11Buffer* VertexBufferD3D11::GetHardwareBuffer() const
	{
		return mVertexBuffer;
	}

	MapData VertexBufferD3D11::Map(MAP_TYPE type, UINT subResource, MAP_FLAG flag){

	}

	void VertexBufferD3D11::Unmap(){

	}
}