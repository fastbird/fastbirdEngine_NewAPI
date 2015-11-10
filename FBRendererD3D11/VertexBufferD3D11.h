#pragma once
#ifndef _VertexBuffer_header_included_
#define _VertexBuffer_header_included_

#include "FBRenderer/IVertexBuffer.h"
namespace fastbird
{
	class VertexBufferD3D11 : public IVertexBuffer
	{
		ID3D11Buffer* mVertexBuffer;		

	public:
		VertexBufferD3D11(ID3D11Buffer* vertexBuffer);
		virtual ~VertexBufferD3D11();

		virtual void Bind();
		virtual bool IsReady() const;
		virtual MapData Map(MAP_TYPE type, UINT subResource, MAP_FLAG flag);
		virtual void Unmap();
		ID3D11Buffer* GetHardwareBuffer() const;
		
	};
}

#endif //_VertexBuffer_header_included_