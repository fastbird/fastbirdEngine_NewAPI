#pragma once
#include "FBCommonHeaders/Types.h"
#include "RendererStructs.h"
namespace fastbird{
	DECLARE_SMART_PTR(IPlatformVertexBuffer);
	DECLARE_SMART_PTR(VertexBuffer);
	class FB_DLL_PUBLIC VertexBuffer{
		DECLARE_PIMPL_NON_COPYABLE(VertexBuffer);
		VertexBuffer(unsigned stride, unsigned numVertices);

	public:
		static VertexBufferPtr Create(unsigned stride, unsigned numVertices);

		void Bind() const;
		MapData Map(MAP_TYPE type, UINT subResource, MAP_FLAG flag) const;
		void Unmap() const;
		unsigned GetStride() const;
		unsigned GetNumVertices() const;
		void SetPlatformBuffer(IPlatformVertexBufferPtr buffer);
		IPlatformVertexBufferPtr GetPlatformBuffer() const;
	};
}