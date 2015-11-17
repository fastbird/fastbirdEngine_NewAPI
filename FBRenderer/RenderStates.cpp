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
#include "RenderStates.h"
#include "Renderer.h"
#include "FBCommonHeaders/CowPtr.h"

namespace fastbird{

class RenderStates::Impl{
public:
	IPlatformRasterizerStatePtr mRasterizerState;
	IPlatformBlendStatePtr mBlendState;
	IPlatformDepthStencilStatePtr mDepthStencilState;

	CowPtr<RASTERIZER_DESC> mRDesc;
	CowPtr<BLEND_DESC> mBDesc;
	CowPtr<DEPTH_STENCIL_DESC> mDDesc;

	static const RASTERIZER_DESC DefaultRDesc;
	static const BLEND_DESC DefaultBDesc;
	static const DEPTH_STENCIL_DESC DefaultDDesc;	

	Impl()
	{
		Reset();
	}

	//-----------------------------------------------------------------------
	void Reset(){
		ResetRasterizerState();
		ResetBlendState();
		ResetDepthStencilState();
	}

	void ResetRasterizerState(){
		mRDesc.reset();
		auto& renderer = Renderer::GetInstance();
		mRasterizerState = renderer.CreateRasterizerState(DefaultRDesc);
	}

	void ResetBlendState(){
		mBDesc.reset();
		auto& renderer = Renderer::GetInstance();
		mBlendState = renderer.CreateBlendState(DefaultBDesc);
	}

	void ResetDepthStencilState(){
		mDDesc.reset();
		auto& renderer = Renderer::GetInstance();
		mDepthStencilState = renderer.CreateDepthStencilState(DefaultDDesc);
	}

	void CreateRasterizerState(const RASTERIZER_DESC& desc){
		auto& renderer = Renderer::GetInstance();
		if (DefaultRDesc == desc){
			if (mRDesc){
				mRDesc.reset();
				mRasterizerState = renderer.CreateRasterizerState(DefaultRDesc);
			}
		}
		else{
			mRDesc = new RASTERIZER_DESC(desc);
			mRasterizerState = renderer.CreateRasterizerState(desc);
		}		
	}

	void CreateBlendState(const BLEND_DESC& desc){
		auto& renderer = Renderer::GetInstance();
		if (DefaultBDesc == desc){
			if (mBDesc){
				mBDesc.reset();
				mBlendState = renderer.CreateBlendState(DefaultBDesc);
			}
		}
		else{
			mBDesc = new BLEND_DESC(desc);
			mBlendState = renderer.CreateBlendState(desc);
		}
	}

	void CreateDepthStencilState(const DEPTH_STENCIL_DESC& desc){
		auto& renderer = Renderer::GetInstance();
		if (DefaultDDesc == desc){
			if (mDDesc){
				mDDesc.reset();
				mDepthStencilState = renderer.CreateDepthStencilState(DefaultDDesc);
			}
		}
		else{
			mDDesc = new DEPTH_STENCIL_DESC(desc);
			mDepthStencilState = renderer.CreateDepthStencilState(desc);
		}		
	}

	void Bind(unsigned stencilRef) const{
		if (mRasterizerState)
			mRasterizerState->Bind();
		if (mBlendState)
			mBlendState->Bind();
		if (mDepthStencilState)
			mDepthStencilState->Bind(stencilRef);
	}
};

//---------------------------------------------------------------------------
IMPLEMENT_STATIC_CREATE(RenderStates);
RenderStates::RenderStates()
	: mImpl(new Impl)
{
}

RenderStates::RenderStates(const RenderStates& other)
	: mImpl(new Impl(*other.mImpl)){

}

RenderStates::~RenderStates(){
}

void RenderStates::Reset(){
	mImpl->Reset();
}
void RenderStates::ResetRasterizerState(){
	mImpl->ResetRasterizerState();
}
void RenderStates::ResetBlendState(){
	mImpl->ResetBlendState();
}
void RenderStates::ResetDepthStencilState(){
	mImpl->ResetDepthStencilState();
}
void RenderStates::CreateRasterizerState(const RASTERIZER_DESC& desc){
	mImpl->CreateRasterizerState(desc);
}
void RenderStates::CreateBlendState(const BLEND_DESC& desc){
	mImpl->CreateBlendState(desc);
}
void RenderStates::CreateDepthStencilState(const DEPTH_STENCIL_DESC& desc){
	mImpl->CreateDepthStencilState(desc);
}

void RenderStates::Bind() const{
	mImpl->Bind(0);
}

void RenderStates::Bind(unsigned stencilRef) const{
	mImpl->Bind(stencilRef);
}

}