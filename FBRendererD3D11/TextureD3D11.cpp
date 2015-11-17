/*
 -----------------------------------------------------------------------------
 This source file is part of fastbird engine
 For the latest info, see http://www.jungwan.net/
 
 Copyright (c) 2013-2015 Jungwan Byun
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "TextureD3D11.h"
#include "RendererD3D11.h"
#include "D3D11Types.h"
#include "ConvertEnumD3D11.h"
#include "ResourceBinder.h"
#include "FBCommonHeaders/CowPtr.h"
DEFINE_GUID(WKPDID_D3DDebugObjectName, 0x429b8c22, 0x9188, 0x4b0c, 0x87, 0x42, 0xac, 0xb0, 0xbf, 0x85, 0xc2, 0x00);
using namespace fastbird;
static unsigned sNextTextureId = 1;
class TextureD3D11::Impl{
public:
	TextureD3D11* mSelf;
	unsigned mId;
	ID3D11Texture2DPtr mTexture;
	ID3D11ShaderResourceViewPtr mSRView;
	ID3D11ShaderResourceView* mSRViewSync;
	HRESULT mHr;
	D3DX11_IMAGE_INFO mImageInfo;
	D3DX11_IMAGE_LOAD_INFO mLoadInfo;	
	// These views are in vector because Cube render targets has 6 views;
	std::vector<ID3D11RenderTargetViewPtr> mRTViews;
	std::vector<ID3D11DepthStencilViewPtr> mDSViews;
	

	//---------------------------------------------------------------------------
	Impl() 
		: mHr(S_FALSE)
		, mSRViewSync(0)
		, mId(sNextTextureId++)
	{
		mLoadInfo.pSrcInfo = &mImageInfo;
		mImageInfo.Width = 0;
		mImageInfo.Height = 0;
		mImageInfo.Format = ConvertEnumD3D11(PIXEL_FORMAT_R8G8B8A8_UNORM);
	}

	const Vec2ITuple GetSize() const {
		return Vec2ITuple{ mImageInfo.Width, mImageInfo.Height };
	}

	PIXEL_FORMAT GetPixelFormat() const {
		return  ConvertEnumFB(mImageInfo.Format);
	}

	bool IsReady() const {
		return mSRView != 0;
	}

	void Bind(BINDING_SHADER shader, int slot) const {
		RendererD3D11::GetInstance().SetTexture(mSelf, shader, slot);
	}

	void Unbind() const {
		RendererD3D11::GetInstance().UnsetTexture(mSelf);
	}

	MapData Map(UINT subResource, MAP_TYPE type, MAP_FLAG flag) const {
		return RendererD3D11::GetInstance().MapTexture(this, subResource, type, flag);
	}

	void Unmap(UINT subResource) const {
		
	}

	void CopyToStaging(IPlatformTexture* dst, UINT dstSubresource,
		UINT dstX, UINT dstY, UINT dstZ, UINT srcSubresource, Box3D* srcBox) const {
		
	}

	void SaveToFile(const char* filename) const {
		
	}

	void GenerateMips() {
		
	}

	void SetDebugName(const char*) {
		
	}


};

//----------------------------------------------------------------------------
TextureD3D11* TextureD3D11::Create()
{
	TextureD3D11* pTexture = FB_NEW(TextureD3D11)();
	return pTexture;
}

void TextureD3D11::Delete()
{
	FB_DELETE(this);
}

//----------------------------------------------------------------------------
TextureD3D11::TextureD3D11()
	: mTexture(0)
	, mSRView(0)
	//, mSamplerState(0)
	, mSlot(0)
	, mSRViewParent(0)
	, mBindingShader(BINDING_SHADER_PS)
{
	mHr = S_FALSE;
	mLoadInfo.pSrcInfo = &mImageInfo;
				
	mPixelFormat = PIXEL_FORMAT_R8G8B8A8_UNORM;
}

//----------------------------------------------------------------------------
TextureD3D11::~TextureD3D11()
{
	for (auto v : mRTViews)
	{
		SAFE_RELEASE(v);
	}		
	for (auto v : mDSViews)
	{
		SAFE_RELEASE(v);
	}

	SAFE_RELEASE(mSRView);	
	SAFE_RELEASE(mTexture);	
}

//----------------------------------------------------------------------------
// ITexture
//----------------------------------------------------------------------------
bool TextureD3D11::IsReady() const
{
	return mSRView!=0 || (*mSRViewParent)!=0;
}

//----------------------------------------------------------------------------
Vec2I TextureD3D11::GetSize() const
{
	return Vec2I(mImageInfo.Width, mImageInfo.Height);
}

//----------------------------------------------------------------------------
unsigned TextureD3D11::GetWidth() const
{
	return mImageInfo.Width;
}

//----------------------------------------------------------------------------
unsigned TextureD3D11::GetHeight() const
{
	return mImageInfo.Height;
}

//----------------------------------------------------------------------------
PIXEL_FORMAT TextureD3D11::GetFormat() const
{
	return mPixelFormat;
}

//----------------------------------------------------------------------------
void TextureD3D11::Bind()
{
	gFBEnv->prenderer.SetTexture(this, mBindingShader, mSlot);
}

void TextureD3D11::Unbind()
{
	gFBEnv->prenderer.SetTexture(0, mBindingShader, mSlot);
}

//----------------------------------------------------------------------------
void TextureD3D11::SetSlot(int slot)
{
	mSlot = slot;
}

//----------------------------------------------------------------------------
void TextureD3D11::SetShaderStage(BINDING_SHADER shader)
{
	mBindingShader = shader;
}

//----------------------------------------------------------------------------
// OWN
//----------------------------------------------------------------------------
ID3D11Texture2D* TextureD3D11::GetHardwareTexture() const
{
	if (mAdamTexture){
		TextureD3D11* adam = (TextureD3D11*)mAdamTexture.get();
		return adam->GetHardwareTexture();
	}
	if (!mTexture && mSRView){
		mSRView->GetResource((ID3D11Resource**)&mTexture);
	}
	return mTexture;
}

//----------------------------------------------------------------------------
ID3D11ShaderResourceView* TextureD3D11::GetHardwareResourceView()
{
	if (!mSRView && mSRViewParent && *mSRViewParent)
	{
		mSRView = *mSRViewParent;
		mSRView->AddRef();
		mSRViewParent = 0;
	}
	return mSRView;
}

//----------------------------------------------------------------------------
void TextureD3D11::SetHardwareTexture(ID3D11Texture2D* pTexture)
{
	SAFE_RELEASE(mTexture);
	mTexture = pTexture;
}
//----------------------------------------------------------------------------
void TextureD3D11::SetHardwareResourceView(ID3D11ShaderResourceView* pResourceView)
{
	SAFE_RELEASE(mSRView);
	mSRView = pResourceView;
}

//----------------------------------------------------------------------------
void TextureD3D11::AddRenderTargetView(ID3D11RenderTargetView* pRenderTargetView)
{
	mRTViews.push_back(pRenderTargetView);
}

void TextureD3D11::ClearRenderTargetViews()
{
	for (auto v : mRTViews)
	{
		RendererD3D11* pRenderer = static_cast<RendererD3D11*>(gFBEnv->pRenderer);
		SAFE_RELEASE(v);
	}
	mRTViews.clear();
}

void TextureD3D11::AddDepthStencilView(ID3D11DepthStencilView* pDepthStencilView)
{
	mDSViews.push_back(pDepthStencilView);
}

void TextureD3D11::ClearDepthStencilViews()
{
	for (auto v : mDSViews)
	{
		SAFE_RELEASE(v);
	}

	mDSViews.clear();
}

void TextureD3D11::SetSize(const Vec2I& size)
{
	mImageInfo.Width = size.x;
	mImageInfo.Height = size.y;
}

ITexture* TextureD3D11::Clone() const
{
	// we don't clone render target and depth stencil.
	assert(mRTViews.empty() && mDSViews.empty());

	TextureD3D11* pNewTexture = TextureD3D11::CreateInstance();
	pNewTexture->mTexture = GetHardwareTexture(); if (pNewTexture->mTexture) pNewTexture->mTexture->AddRef();
	pNewTexture->mSRView = mSRView; if (mSRView) mSRView->AddRef();
	Texture* pAdam = (Texture*)this;
	while (pAdam->GetAdamTexture())
	{
		pAdam = pAdam->GetAdamTexture();
	}
	pNewTexture->SetAdamTexture(pAdam);
	
	if (!pNewTexture->mSRView)
	{
		TextureD3D11* pT = (TextureD3D11*)pAdam;
		assert(pT);
		pNewTexture->mSRViewParent = (ID3D11ShaderResourceView**)&pT->mSRView;
	}
	/*pNewTexture->mSamplerState = mSamplerState; if (mSamplerState) mSamplerState->AddRef();
	pNewTexture->mSamplerDesc = mSamplerDesc;*/
	pNewTexture->mPixelFormat = mPixelFormat;
	pNewTexture->mName = mName;
	pNewTexture->mType = mType;
	
	pNewTexture->mHr = mHr;
	pNewTexture->mImageInfo = mImageInfo;
	pNewTexture->mLoadInfo = mLoadInfo;
	
	return pNewTexture;
}

void TextureD3D11::CopyToStaging(ITexture* dst, UINT dstSubresource, UINT dstX, UINT dstY, UINT dstZ,
			UINT srcSubresource, Box3D* srcBox)
{
	gFBEnv->prenderer.CopyToStaging(dst, dstSubresource, dstX, dstY, dstZ,
		this, srcSubresource, srcBox);
		
}

void TextureD3D11::SaveToFile(const char* filename)
{
	gFBEnv->prenderer.SaveTextureToFile(this, filename);
}

ID3D11RenderTargetView* TextureD3D11::GetRenderTargetView(int idx) const
{
	assert((size_t)idx < mRTViews.size());
	return mRTViews[idx];
}
ID3D11DepthStencilView* TextureD3D11::GetDepthStencilView(int idx) const
{
	assert((size_t)idx < mDSViews.size());
	return mDSViews[idx];
}

void TextureD3D11::SetDebugName(const char* name)
{
	if (mTexture)
		mTexture->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name), name);
	if (mSRView)
	{
		char buff[255];
		sprintf_s(buff, "%s SRV", name);
		mSRView->SetPrivateData(WKPDID_D3DDebugObjectName, 0, 0);
		mSRView->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(buff), buff);
	}
	int i = 0;
	for (auto it : mRTViews)
	{
		char buff[255];
		sprintf_s(buff, "%s RTV %d", name, i++);
		it->SetPrivateData(WKPDID_D3DDebugObjectName, 0, 0);
		it->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(buff), buff);
	}

	i = 0;
	for (auto it : mDSViews)
	{
		char buff[255];
		sprintf_s(buff, "%s DSV %d", name, i++);
		it->SetPrivateData(WKPDID_D3DDebugObjectName, 0, 0);
		it->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(buff), buff);
	}
}

void TextureD3D11::GenerateMips()
{
	gFBEnv->prenderer.GenerateMips(this);
}

void TextureD3D11::OnReloaded()
{
	// only called for adam texture
	assert(!mAdamTexture);

	FB_FOREACH(it, Texture::mTextures)
	{
		TextureD3D11* pT = (TextureD3D11*)*it;
		if (pT->GetAdamTexture() == this)
		{
			SAFE_RELEASE(pT->mSRView);
			pT->mSRViewParent = (ID3D11ShaderResourceView**)&mSRView;
		}
	}
}