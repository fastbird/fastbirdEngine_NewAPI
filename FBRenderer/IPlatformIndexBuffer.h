#pragma once
#include "RendererStructs.h"
namespace fastbird
{
	enum INDEXBUFFER_FORMAT
	{
		INDEXBUFFER_FORMAT_16BIT,
		INDEXBUFFER_FORMAT_32BIT,
	};
	DECLARE_SMART_PTR(IPlatformIndexBuffer);
	class IPlatformIndexBuffer
	{
	public:		
		virtual bool IsReady() const = 0;		
		virtual void Bind() = 0;
		virtual MapData Map(MAP_TYPE type, UINT subResource, MAP_FLAG flag) = 0;
		virtual void Unmap() = 0;

	protected:
		~IPlatformIndexBuffer(){}
	};
}