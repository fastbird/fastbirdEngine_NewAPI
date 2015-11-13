#pragma once
#include "RendererStructs.h"
namespace fastbird
{
	DECLARE_SMART_PTR(IPlatformVertexBuffer);
	class IPlatformVertexBuffer
	{
	public:		
		virtual bool IsReady() const = 0;		
		virtual void Bind() = 0;
		virtual MapData Map(MAP_TYPE type, UINT subResource, MAP_FLAG flag) = 0;
		virtual void Unmap() = 0;	

	protected:
		~IPlatformVertexBuffer() {}
	};
}