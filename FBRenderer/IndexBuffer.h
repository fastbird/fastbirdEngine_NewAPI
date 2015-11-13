#pragma once
#include "FBCommonHeaders/Types.h"
#include "RendererStructs.h"
#include "IPlatformIndexBuffer.h"
namespace fastbird{	
	DECLARE_SMART_PTR(IndexBuffer);
	class FB_DLL_PUBLIC IndexBuffer{
		DECLARE_PIMPL_NON_COPYABLE(IndexBuffer);
		IndexBuffer(unsigned numIndices, INDEXBUFFER_FORMAT format);

	public:
		static IndexBufferPtr Create(unsigned numIndices, INDEXBUFFER_FORMAT format);

		bool IsReady() const;
		void Bind() const;
		MapData Map(MAP_TYPE type, UINT subResource, MAP_FLAG flag) const;
		void Unmap() const;
		unsigned GetNumIndices() const;
		INDEXBUFFER_FORMAT GetFormat() const;
		void SetPlatformBuffer(IPlatformIndexBufferPtr buffer);
		IPlatformIndexBufferPtr GetPlatformBuffer();
	};
}