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
		auto renderer = Renderer::GetInstance();
		mRasterizerState = renderer->CreateRasterizerState(DefaultRDesc);
	}

	void ResetBlendState(){
		mBDesc.reset();
		auto renderer = Renderer::GetInstance();
		mBlendState = renderer->CreateBlendState(DefaultBDesc);
	}

	void ResetDepthStencilState(){
		mDDesc.reset();
		auto renderer = Renderer::GetInstance();
		mDepthStencilState = renderer->CreateDepthStencilState(DefaultDDesc);
	}

	void CreateRasterizerState(const RASTERIZER_DESC& desc){
		auto renderer = Renderer::GetInstance();
		if (DefaultRDesc == desc){
			if (mRDesc){
				mRDesc.reset();
				mRasterizerState = renderer->CreateRasterizerState(DefaultRDesc);
			}
		}
		else{
			mRDesc = new RASTERIZER_DESC(desc);
			mRasterizerState = renderer->CreateRasterizerState(desc);
		}		
	}

	void CreateBlendState(const BLEND_DESC& desc){
		auto renderer = Renderer::GetInstance();
		if (DefaultBDesc == desc){
			if (mBDesc){
				mBDesc.reset();
				mBlendState = renderer->CreateBlendState(DefaultBDesc);
			}
		}
		else{
			mBDesc = new BLEND_DESC(desc);
			mBlendState = renderer->CreateBlendState(desc);
		}
	}

	void CreateDepthStencilState(const DEPTH_STENCIL_DESC& desc){
		auto renderer = Renderer::GetInstance();
		if (DefaultDDesc == desc){
			if (mDDesc){
				mDDesc.reset();
				mDepthStencilState = renderer->CreateDepthStencilState(DefaultDDesc);
			}
		}
		else{
			mDDesc = new DEPTH_STENCIL_DESC(desc);
			mDepthStencilState = renderer->CreateDepthStencilState(desc);
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