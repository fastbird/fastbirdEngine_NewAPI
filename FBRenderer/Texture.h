#pragma once
#include "RendererEnums.h"
#include "RendererStructs.h"
namespace fastbird{
	DECLARE_SMART_PTR(ITexture);
	DECLARE_SMART_PTR(Texture);
	class FB_DLL_PUBLIC Texture
	{
		DECLARE_PIMPL(Texture);
		friend class RenderResourceFactory;
		Texture();
	
	public:		
		static unsigned sNextTextureID;
		~Texture();
		TexturePtr Clone() const;

		bool LoadFile(const char* filepath, bool async);

		size_t GetTextureID() const;
		void SetName(const char* filepath);
		const char* GetName() const;		
		void SetType(TEXTURE_TYPE type);
		TEXTURE_TYPE GetType() const;
		unsigned GetWidth() const;
		unsigned GetHeight() const;
		PIXEL_FORMAT GetFormat() const;
		Vec2I GetSize();
		void SetSlot(int slot);
		int GetSlot() const;
		void SetShaderStage(BINDING_SHADER shader);
		BINDING_SHADER GetShaderStage() const;
		void SetAdamTexture(TexturePtr adam);
		TexturePtr GetAdamTexture() const;
		void SetDebugName(const char* name);

		bool IsReady() const;
		void Bind();
		void Unbind();		
		MapData Map(UINT subResource, MAP_TYPE type, MAP_FLAG flag);
		void Unmap(UINT subResource);
		void CopyToStaging(TexturePtr dst, UINT dstSubresource, UINT dstX, UINT dstY, UINT dstZ,
			UINT srcSubresource, Box3D* srcBox);
		void SaveToFile(const char* filename);
		void GenerateMips();
		void SetPlatformTexture(ITexturePtr platformTexture);
		ITexturePtr GetPlatformTexture() const;
	};
}