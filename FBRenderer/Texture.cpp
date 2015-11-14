#include "stdafx.h"
#include "Texture.h"
#include "Renderer.h"
#include "IPlatformTexture.h"
#include "FBCommonHeaders/CowPtr.h"

namespace fastbird{
static std::vector<TextureWeakPtr> mAllTextures;
TexturePtr GetTextureFromExistings(IPlatformTexturePtr platformShader) {
	for (auto it = mAllTextures.begin(); it != mAllTextures.end();){
		auto texture = it->lock();
		if (texture){
			++it;
			if (texture->GetPlatformTexture() == platformShader){
				return texture;
			}
		}
		else{
			it = mAllTextures.erase(it);
		}
	}
}

size_t Texture::sNextTextureID = 0;
class Texture::Impl{
public:
	// I think having texture properties like size, format in the Texture is good idea.
	// Platform renderer doesn't use that information so don't need to have it.
	// If that's true, let's move the data to here.
	unsigned mTextureID;
	IPlatformTexturePtr mPlatformTexture;	
	TEXTURE_TYPE mType;
	//---------------------------------------------------------------------------
	Impl()
		: mTextureID(sNextTextureID++)
		, mType(TEXTURE_TYPE_DEFAULT)
	{
	}

	const char* GetName() const{
		return mPlatformTexture->GetPath();
	}

	void SetType(TEXTURE_TYPE type){
		mType = type;
	}

	TEXTURE_TYPE GetType() const{
		return mType;
	}

	int GetWidth() const{
		return mPlatformTexture->GetSize().first;
	}

	int GetHeight() const{
		return mPlatformTexture->GetSize().second;
	}

	Vec2I GetSize() const{
		return mPlatformTexture->GetSize();
	}

	PIXEL_FORMAT GetFormat() const{
		return mPlatformTexture->GetPixelFormat();		
	}

	void SetDebugName(const char* name){
		mPlatformTexture->SetDebugName(name);
	}

	bool IsReady() const{
		return mPlatformTexture->IsReady();
	}

	void Bind(BINDING_SHADER shader, int slot) const{
		mPlatformTexture->Bind(shader, slot);
	}

	void Unbind() const{
		mPlatformTexture->Unbind();
	}

	MapData Map(UINT subResource, MAP_TYPE type, MAP_FLAG flag) const{
		return mPlatformTexture->Map(subResource, type, flag);
	}

	void Unmap(UINT subResource) const{
		return mPlatformTexture->Unmap(subResource);
	}

	void CopyToStaging(TexturePtr dst, UINT dstSubresource, UINT dstX, UINT dstY, UINT dstZ, 
		UINT srcSubresource, Box3D* srcBox) const{
		mPlatformTexture->CopyToStaging(dst->mImpl->mPlatformTexture.get(), 
			dstSubresource, dstX, dstY, dstZ, srcSubresource, srcBox);
	}

	void SaveToFile(const char* filename) const{
		mPlatformTexture->SaveToFile(filename);
	}

	void GenerateMips() {
		mPlatformTexture->GenerateMips();
	}

	void SetPlatformTexture(IPlatformTexturePtr platformTexture){
		mPlatformTexture = platformTexture;
	}

	IPlatformTexturePtr GetPlatformTexture() const{
		return mPlatformTexture;
	}
};

//---------------------------------------------------------------------------
TexturePtr Texture::Create(){
	auto p = TexturePtr(FB_NEW(Texture), [](Texture* obj){ FB_DELETE(obj); });
	mAllTextures.push_back(p);
	return p;
}

Texture::Texture()
	: mImpl(new Impl){
}

Texture::~Texture(){
	auto itEnd = mAllTextures.end();
	for (auto it = mAllTextures.begin(); it != itEnd; it++){
		if (it->expired()){
			mAllTextures.erase(it);
			return;
		}
	}
}

Texture& Texture::operator = (const Texture& other){
	assert(0 && "not implemented");
	return *this;
}

size_t Texture::GetTextureID() const{
	return mImpl->mTextureID;
}

const char* Texture::GetName() const{
	return mImpl->GetName();
}

void Texture::SetType(TEXTURE_TYPE type)
{
	mImpl->SetType(type);
}

TEXTURE_TYPE Texture::GetType() const{
	return mImpl->GetType();
}

int Texture::GetWidth() const{
	return mImpl->GetWidth();
}

int Texture::GetHeight() const{
	return mImpl->GetHeight();
}

PIXEL_FORMAT Texture::GetFormat() const{
	return mImpl->GetFormat();
}

Vec2I Texture::GetSize(){
	return mImpl->GetSize();
}

void Texture::SetDebugName(const char* name){
	mImpl->SetDebugName(name);
}

bool Texture::IsReady() const{
	return mImpl->IsReady();
}

void Texture::Bind(BINDING_SHADER shader, int slot){
	mImpl->Bind(shader, slot);
}

void Texture::Unbind(){
	mImpl->Unbind();
}

MapData Texture::Map(UINT subResource, MAP_TYPE type, MAP_FLAG flag)
{
	return mImpl->Map(subResource, type, flag);	
}
void Texture::Unmap(UINT subResource)
{
	mImpl->Unmap(subResource);
}

void Texture::CopyToStaging(TexturePtr dst, UINT dstSubresource, UINT dstX, UINT dstY, UINT dstZ,
	UINT srcSubresource, Box3D* srcBox){
	mImpl->CopyToStaging(dst, dstSubresource, dstX, dstY, dstZ, srcSubresource, srcBox);
}

void Texture::SaveToFile(const char* filename){
	mImpl->SaveToFile(filename);
}

void Texture::GenerateMips(){
	mImpl->GenerateMips();
}

void Texture::SetPlatformTexture(IPlatformTexturePtr platformTexture){
	mImpl->SetPlatformTexture(platformTexture);
}

IPlatformTexturePtr Texture::GetPlatformTexture() const{
	return mImpl->GetPlatformTexture();
}

}