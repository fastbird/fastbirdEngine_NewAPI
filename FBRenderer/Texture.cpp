#include "stdafx.h"
#include "Texture.h"
#include "Renderer.h"
#include "ITexture.h"

namespace fastbird{
size_t Texture::sNextTextureID = 0;
class Texture::TextureImpl{
public:
	unsigned mTextureID;
	std::string mName;
	TEXTURE_TYPE mType;
	static std::vector<Texture*> mTextures;
	TextureWeakPtr mAdamTexture;
	ITexturePtr mPlatformTexture;
	TextureImpl()
		: mTextureID(sNextTextureID++)
		, mType(TEXTURE_TYPE_DEFAULT)
	{
	}

	bool LoadFile(const char* filepath, bool async){
		return mPlatformTexture->LoadFile(filepath, async);
	}

	Vec2I GetSize() const{
		return Vec2I((int)mPlatformTexture->GetWidth(), (int)mPlatformTexture->GetHeight());
	}

	MapData Map(UINT subResource, MAP_TYPE type, MAP_FLAG flag){
		return mPlatformTexture->Map(subResource, type, flag);
	}
	void Unmap(UINT subResource){
		return mPlatformTexture->Unmap(subResource);
	}
};

Texture::Texture()
	: mImpl(new TextureImpl){
}

Texture::~Texture(){
	delete mImpl;	
}

bool Texture::LoadFile(const char* filepath, bool async){
	return mImpl->LoadFile(filepath, async);
}

size_t Texture::GetTextureID() const{
	return mImpl->mTextureID;
}

void Texture::SetName(const char* filepath)
{
	mImpl->mName = filepath;
}

const char* Texture::GetName() const{
	return mImpl->mName.c_str();
}

Vec2I Texture::GetSize(){
	return mImpl->GetSize();
}

void Texture::SetType(TEXTURE_TYPE type)
{
	mImpl->mType = type;
}

MapData Texture::Map(UINT subResource, MAP_TYPE type, MAP_FLAG flag)
{
	return mImpl->Map(subResource, type, flag);	
}
void Texture::Unmap(UINT subResource)
{
	mImpl->Unmap(subResource);
}

}