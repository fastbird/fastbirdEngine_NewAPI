#pragma once
#ifndef _IVertexBuffer_header_included__
#define _IVertexBuffer_header_included__
#include "RendererStructs.h"
namespace fastbird
{
	DECLARE_SMART_PTR(IVertexBuffer);
	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() {}
		virtual bool IsReady() const = 0;		
		virtual void Bind() = 0;
		virtual MapData Map(MAP_TYPE type, UINT subResource, MAP_FLAG flag) = 0;
		virtual void Unmap() = 0;		
	};
}

#endif //_IVertexBuffer_header_included__