#pragma once
#include "RendererEnums.h"
#include "RendererStructs.h"
namespace fastbird{
	DECLARE_SMART_PTR(IPlatformTexture);
	DECLARE_SMART_PTR(Texture);
	/** Texture.
	You can create a texture with the function Renderer::CreateTexture(...).
	When you create server textures with the same file name, the underying hardward
	texture resource will be shared among them.	
	*/
	class FB_DLL_PUBLIC Texture
	{
		DECLARE_PIMPL(Texture);
		friend class RenderResourceFactory;
		Texture();
		/** private operators.
		You can clone Textures efficiently with Texture:Clone(). GPU data
		will be shared as well as the underlying data like, texture type, size and path.
		If you modify the underlying data, for example setting the new texture file, 
		change binding slots and shader stages, only the instance you are handling
		will be affected.
		*/
		explicit Texture(const Texture&); // cloning
		Texture& operator=(const Texture& other);

	public:		
		static unsigned sNextTextureID;

		/** If you want this texture to be managed by the Renderer,
		do not call this function directly. Use Renderer::CreateTexture() instead.
		*/
		static TexturePtr Create();
		/** Usually, you don't need to clone on the texture you get from 
		Renderer::CreateTexture() since it is already cloned and you can make
		a change on binding shader stages or input slots as you with not polluting
		shared textures.
		Manually Cloned texture will not be managed by Renderer.
		*/
		TexturePtr Clone() const;
		~Texture();

		size_t GetTextureID() const;
		const char* GetName() const;		
		void SetType(TEXTURE_TYPE type);
		TEXTURE_TYPE GetType() const;
		int GetWidth() const;
		int GetHeight() const;
		Vec2I GetSize();
		PIXEL_FORMAT GetFormat() const;		
		void SetDebugName(const char* name);

		bool IsReady() const;
		void Bind(BINDING_SHADER shader, int slot);
		void Unbind();		
		MapData Map(UINT subResource, MAP_TYPE type, MAP_FLAG flag);
		void Unmap(UINT subResource);
		void CopyToStaging(TexturePtr dst, UINT dstSubresource, UINT dstX, UINT dstY, UINT dstZ,
			UINT srcSubresource, Box3D* srcBox);
		void SaveToFile(const char* filename);
		void GenerateMips();
		void SetPlatformTexture(IPlatformTexturePtr platformTexture);
	};
}