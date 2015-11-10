#include "stdafx.h"
#include "RenderStatesD3D11.h"
#include "ResourceBinder.h"

using namespace fastbird;

//----------------------------------------------------------------------------
// RASTERIZER STATE
//-------------------------------------------------------------------------
//----------------------------------------------------------------------------
RasterizerStateD3D11::RasterizerStateD3D11(ID3D11RasterizerState* rasterizerState)
	: mRasterizerState(rasterizerState)
{
}

//----------------------------------------------------------------------------
RasterizerStateD3D11::~RasterizerStateD3D11()
{
	SAFE_RELEASE(mRasterizerState);
}

//----------------------------------------------------------------------------
void RasterizerStateD3D11::Bind()
{
	ResourceBinder::Bind(this);
}

//----------------------------------------------------------------------------
void RasterizerStateD3D11::SetDebugName(const char* name)
{
	if (mRasterizerState)
	{
		mRasterizerState->SetPrivateData(WKPDID_D3DDebugObjectName, 0, 0);
		mRasterizerState->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name), name);
	}
}

//----------------------------------------------------------------------------
void RasterizerStateD3D11::SetHardwareRasterizerState()
{
	SAFE_RELEASE(mRasterizerState);
	mRasterizerState = pRasterizerState;
}


//----------------------------------------------------------------------------
// BLEND STATE
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
BlendStateD3D11::BlendStateD3D11(ID3D11BlendState* blendState)
	:mBlendState(blendState)
{
}

//----------------------------------------------------------------------------
BlendStateD3D11::~BlendStateD3D11()
{
	SAFE_RELEASE(mBlendState);
}

//----------------------------------------------------------------------------
void BlendStateD3D11::Bind()
{
	ResourceBinder::Bind(this);	
}

//----------------------------------------------------------------------------
void BlendStateD3D11::SetDebugName(const char* name)
{
	if (mBlendState)
	{
		mBlendState->SetPrivateData(WKPDID_D3DDebugObjectName, 0, 0);
		mBlendState->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name), name);
	}
}


//----------------------------------------------------------------------------
// DEPTH STENCIL STATE
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
DepthStencilStateD3D11::DepthStencilStateD3D11(ID3D11DepthStencilState* depthStencilState)
	:mDepthStencilState(depthStencilState)
{
}

//----------------------------------------------------------------------------
DepthStencilStateD3D11::~DepthStencilStateD3D11()
{
	SAFE_RELEASE(mDepthStencilState);
}

//----------------------------------------------------------------------------
void DepthStencilStateD3D11::Bind(unsigned stencilRef)
{
	ResourceBinder::Bind(this);	
}

//----------------------------------------------------------------------------
void DepthStencilStateD3D11::SetDebugName(const char* name)
{
	if (mDepthStencilState) {
		mDepthStencilState->SetPrivateData(WKPDID_D3DDebugObjectName, 0, 0);
		mDepthStencilState->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name), name);
	}
}

//----------------------------------------------------------------------------
// Sampler STATE
//----------------------------------------------------------------------------

SamplerStateD3D11::SamplerStateD3D11(ID3D11SamplerState* samplerState)
	:mSamplerState(samplerState)
{

}
SamplerStateD3D11::~SamplerStateD3D11()
{
	SAFE_RELEASE(mSamplerState);
}

void SamplerStateD3D11::Bind(BINDING_SHADER shader, int slot)
{
	ResourceBinder::Bind(this);	
}

void SamplerStateD3D11::SetDebugName(const char* name)
{
	if (mSamplerState)
	{
		mSamplerState->SetPrivateData(WKPDID_D3DDebugObjectName, 0, 0);
		mSamplerState->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name), name);
	}
}