#pragma once
#include "FBCommonHeaders/Types.h"
#include "RendererEnums.h"
#include "RendererStructs.h"
namespace fastbird
{
	DECLARE_SMART_PTR(IPlatformTexture);
	class IPlatformTexture
	{
	public:
		virtual const char* GetPath() const = 0;
		virtual const Vec2ITuple GetSize() const = 0;
		virtual PIXEL_FORMAT GetPixelFormat() const = 0;		
		virtual bool IsReady() const = 0;
		virtual void Bind(BINDING_SHADER shader, int slot) const = 0;
		virtual void Unbind() const = 0;
		virtual MapData Map(UINT subResource, MAP_TYPE type, MAP_FLAG flag) const = 0;
		virtual void Unmap(UINT subResource) const = 0;
		virtual void CopyToStaging(IPlatformTexture* dst, UINT dstSubresource, 
			UINT dstX, UINT dstY, UINT dstZ, UINT srcSubresource, Box3D* srcBox) const = 0;
		virtual void SaveToFile(const char* filename) const = 0;
		virtual void GenerateMips() = 0;
		virtual void SetDebugName(const char*) = 0;

	protected:
		~IPlatformTexture(){}
	};
}