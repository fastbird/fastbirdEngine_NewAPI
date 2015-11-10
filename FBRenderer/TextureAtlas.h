#pragma once
#include "FBCommonHeaders/VectorMap.h"
namespace fastbird{
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
		typedef VectorMap<std::string, TextureAtlasRegion*> REGION_MAP;
		REGION_MAP mRegions;
		
		TextureAtlas();

	public:
		static TextureAtlasPtr CreateTextureAtlas();
		~TextureAtlas()
		{
			for (auto& c : mRegions)
			{
				FB_SAFE_DELETE(c.second);
			}
			mRegions.clear();
		}

		TextureAtlasRegion* AddRegion(const char* name);
		TextureAtlasRegion* GetRegion(const char* name);
		bool ReloadTextureAtlas();
	};
}