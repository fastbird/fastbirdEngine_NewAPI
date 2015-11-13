#include "stdafx.h"
#include "RenderStates.h"
#include "Renderer.h"

namespace fastbird{

class RenderStates::Impl{
public:
	IPlatformRasterizerStatePtr mRasterizerState;
	IPlatformBlendStatePtr mBlendState;
	IPlatformDepthStencilStatePtr mDepthStencilState;

	// todo: Good to be CowPtrs.
	RASTERIZER_DESC mRDesc;
	BLEND_DESC mBDesc;
	DEPTH_STENCIL_DESC mDDesc;

	//-----------------------------------------------------------------------
	void Reset(){
		ResetRasterizerState();
		ResetBlendState();
		ResetDepthStencilState();
	}

	void ResetRasterizerState(){
		mRDesc = RASTERIZER_DESC();
		CreateRasterizerState(mRDesc);
	}

	void ResetBlendState(){
		mBDesc = BLEND_DESC();
		CreateBlendState(mBDesc);
	}

	void ResetDepthStencilState(){
		mDDesc = DEPTH_STENCIL_DESC();
		CreateDepthStencilState(mDDesc);
	}

	void CreateRasterizerState(const RASTERIZER_DESC& desc){
		mRDesc = desc;
		auto renderer = Renderer::GetInstance();
		mRasterizerState = renderer->CreateRasterizerState(desc);
	}

	void CreateBlendState(const BLEND_DESC& desc){
		mBDesc = desc;
		auto renderer = Renderer::GetInstance();
		mBlendState = renderer->CreateBlendState(desc);
	}

	void CreateDepthStencilState(const DEPTH_STENCIL_DESC& desc){
		mDDesc = desc;
		auto renderer = Renderer::GetInstance();
		mDepthStencilState = renderer->CreateDepthStencilState(desc);
	}

	void Bind(unsigned stencilRef){
		mRasterizerState->Bind();
		mBlendState->Bind();
		mDepthStencilState->Bind(stencilRef);
	}

	RenderStatesPtr Clone() const{
		RenderStatesPtr newObj = RenderStates::Create();		
		newObj->CreateBlendState(mBDesc);
		newObj->CreateDepthStencilState(mDDesc);
		newObj->CreateRasterizerState(mRDesc);
		return newObj;
	}
};

//---------------------------------------------------------------------------
IMPLEMENT_STATIC_CREATE(RenderStates);
RenderStates::RenderStates()
	: mImpl(new Impl)
{
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

void RenderStates::Bind(){
	mImpl->Bind(0);
}

void RenderStates::Bind(unsigned stencilRef){
	mImpl->Bind(stencilRef);
}
RenderStatesPtr RenderStates::Clone() const{
	return mImpl->Clone();
}

}