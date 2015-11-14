#pragma once
#include "FBCommonHeaders/VectorMap.h"
#include "FBMathLib/Math.h"
namespace fastbird{
	DECLARE_SMART_PTR_STRUCT(TextureAtlasRegion);
	struct TextureAtlasRegion
	{
		DWORD mID;
		std::string mName;
		Vec2 mUVStart;
		Vec2 mUVEnd;
		Vec2I mStart;
		Vec2I mSize;

		void GetQuadUV(Vec2 uv[]);
		const Vec2& GetStartUV() const;
		const Vec2 GetUVSize() const;
		const Vec2I& GetSize() const;
	};

	//------------------------------------------------------------------------
	DECLARE_SMART_PTR(Texture);
	DECLARE_SMART_PTR(TextureAtlas);
	class TextureAtlas
	{
		std::string mPath;
		TexturePtr mTexture;
		typedef VectorMap<std::string, TextureAtlasRegionPtr> REGION_MAP;
		REGION_MAP mRegions;
		
		TextureAtlas();

	public:
		static TextureAtlasPtr Create();
		~TextureAtlas();

		TextureAtlasRegionPtr AddRegion(const char* name);
		TextureAtlasRegionPtr GetRegion(const char* name);
		void SetTexture(TexturePtr texture);
		TexturePtr GetTexture() const;
		void SetPath(const char* path);		
		const char* GetPath() const;
		bool ReloadTextureAtlas();		
	};
}