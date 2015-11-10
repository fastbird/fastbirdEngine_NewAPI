#pragma once
#include "FBCommonHeaders/Types.h"
#include "RendererEnums.h"
#include "RendererStructs.h"
namespace fastbird
{
	class Vec2I;
	DECLARE_SMART_PTR(ITexture);
	class ITexture
	{
	public:
		virtual ~ITexture(){}

		virtual ITexture* Clone() const = 0;
		virtual bool LoadFile(const char* filepath, bool async) = 0;
		virtual void SetName(const char* filepath) = 0;
		virtual const char* GetName() const = 0;
		virtual unsigned GetWidth() const = 0;
		virtual unsigned GetHeight() const = 0;
		virtual PIXEL_FORMAT GetFormat() const = 0;
		virtual void SetSlot(int slot) = 0;
		virtual int GetSlot() const = 0;
		virtual void SetType(TEXTURE_TYPE type) = 0;
		virtual TEXTURE_TYPE GetType() const = 0;
		virtual void SetShaderStage(BINDING_SHADER shader) = 0;
		virtual BINDING_SHADER GetShaderStage() const = 0;
		virtual bool IsReady() const = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual MapData Map(UINT subResource, MAP_TYPE type, MAP_FLAG flag) = 0;
		virtual void Unmap(UINT subResource) = 0;
		virtual void CopyToStaging(ITexture* dst, UINT dstSubresource, UINT dstX, UINT dstY, UINT dstZ,
			UINT srcSubresource, Box3D* srcBox) = 0;
		virtual void SaveToFile(const char* filename) = 0;
		virtual void GenerateMips() = 0;
		virtual void SetDebugName(const char*) = 0;
	};
}