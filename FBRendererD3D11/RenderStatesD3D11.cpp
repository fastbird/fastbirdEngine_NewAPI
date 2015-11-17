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