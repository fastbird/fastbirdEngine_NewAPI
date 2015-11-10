#include "stdafx.h"
#include "RenderStates.h"

namespace fastbird{

class RenderStates::RenderStatesImpl{
public:
	IRasterizerStatePtr mRasterizerState;
	IBlendStatePtr mBlendState;
	IDepthStencilStatePtr mDepthStencilState;

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
		/*if (gFBEnv && gFBEnv->pRenderer)
			mRasterizerState = gFBEnv->pRenderer->CreateRasterizerState(desc);*/
	}

	void CreateBlendState(const BLEND_DESC& desc){
		mBDesc = desc;
		/*if (gFBEnv && gFBEnv->pRenderer)
			mBlendState = gFBEnv->pRenderer->CreateBlendState(desc);*/
	}

	void CreateDepthStencilState(const DEPTH_STENCIL_DESC& desc){
		mDDesc = desc;
		/*if (gFBEnv && gFBEnv->pRenderer)
			mDepthStencilState = gFBEnv->pRenderer->CreateDepthStencilState(desc);*/
	}

	void Bind(unsigned stencilRef){
		mRasterizerState->Bind();
		mBlendState->Bind();
		mDepthStencilState->Bind(stencilRef);
	}

	RenderStatesPtr Clone() const{
		RenderStatesPtr newObj(FB_NEW(RenderStates), [](RenderStates* obj){ delete obj; });
		newObj->CreateBlendState(mBDesc);
		newObj->CreateDepthStencilState(mDDesc);
		newObj->CreateRasterizerState(mRDesc);
		return newObj;
	}
};

//---------------------------------------------------------------------------
RenderStates::RenderStates()
	: mImpl(new RenderStatesImpl)
{

}

RenderStates::~RenderStates(){
	delete mImpl;
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