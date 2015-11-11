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

	unsigned GetWidth() const{
		return mPlatformTexture->GetWidth();
	}

	unsigned GetHeight() const{
		return mPlatformTexture->GetHeight();
	}

	void SetSlot(int slot){
		mPlatformTexture->SetSlot(slot);
	}

	int GetSlot() const{
		return mPlatformTexture->GetSlot();
	}

	Vec2I GetSize() const{
		return Vec2I((int)mPlatformTexture->GetWidth(), (int)mPlatformTexture->GetHeight());
	}

	void SetShaderStage(BINDING_SHADER shader){
		mPlatformTexture->SetShaderStage(shader);
	}

	BINDING_SHADER GetShaderStage() const{
		return mPlatformTexture->GetShaderStage();
	}

	void SetDebugName(const char* name){
		mPlatformTexture->SetDebugName(name);
	}

	bool IsReady() const{
		return mPlatformTexture->IsReady();
	}

	void Bind(){
		mPlatformTexture->Bind();
	}

	void Unbind(){
		mPlatformTexture->Unbind();
	}

	MapData Map(UINT subResource, MAP_TYPE type, MAP_FLAG flag){
		return mPlatformTexture->Map(subResource, type, flag);
	}

	void Unmap(UINT subResource){
		return mPlatformTexture->Unmap(subResource);
	}

	void CopyToStaging(TexturePtr dst, UINT dstSubresource, UINT dstX, UINT dstY, UINT dstZ, 
		UINT srcSubresource, Box3D* srcBox){
		mPlatformTexture->CopyToStaging(dst->GetPlatformTexture().get(), 
			dstSubresource, dstX, dstY, dstZ, srcSubresource, srcBox);
	}

	void SaveToFile(const char* filename){
		mPlatformTexture->SaveToFile(filename);
	}

	void GenerateMips(){
		mPlatformTexture->GenerateMips();
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

void Texture::SetType(TEXTURE_TYPE type)
{
	mImpl->mType = type;
}

TEXTURE_TYPE Texture::GetType() const{
	return mImpl->mType;
}

unsigned Texture::GetWidth() const{
	return mImpl->GetWidth();
}

unsigned Texture::GetHeight() const{
	return mImpl->GetHeight();
}

Vec2I Texture::GetSize(){
	return mImpl->GetSize();
}

void Texture::SetSlot(int slot){
	mImpl->SetSlot(slot);
}

int Texture::GetSlot() const{
	return mImpl->GetSlot();
}

void Texture::SetShaderStage(BINDING_SHADER shader){
	mImpl->SetShaderStage(shader);
}

BINDING_SHADER Texture::GetShaderStage() const{
	return mImpl->GetShaderStage();
}

void Texture::SetAdamTexture(TexturePtr adam){
	mImpl->mAdamTexture = adam;
}

TexturePtr Texture::GetAdamTexture() const{
	return mImpl->mAdamTexture.lock();
}

void Texture::SetDebugName(const char* name){
	mImpl->SetDebugName(name);
}

bool Texture::IsReady() const{
	return mImpl->IsReady();
}

void Texture::Bind(){
	mImpl->Bind();
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

ITexturePtr Texture::GetPlatformTexture() const{
	return mImpl->mPlatformTexture;
}

}