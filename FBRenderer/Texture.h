#pragma once
#include "RendererEnums.h"
#include "RendererStructs.h"
namespace fastbird{
	DECLARE_SMART_PTR(Texture);
	class FBRendererDLL Texture
	{
		DECLARE_PIMPL(Texture);
		friend class RenderResourceFactory;
		Texture();
	
	public:		
		static unsigned sNextTextureID;
		~Texture();
		
		bool LoadFile(const char* filepath, bool async);

		size_t GetTextureID() const;
		void SetName(const char* filepath);
		const char* GetName() const;
		Vec2I GetSize();
		void SetType(TEXTURE_TYPE type);
		TEXTURE_TYPE GetType() const;
		MapData Map(UINT subResource, MAP_TYPE type, MAP_FLAG flag);
		void Unmap(UINT subResource);
		void SetAdamTexture(Texture* adam);
		Texture* GetAdamTexture() const;
	};
}