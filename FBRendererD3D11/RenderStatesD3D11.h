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

#pragma once
#include "FBRenderer/IPlatformRenderStates.h"
#include "FBRenderer/RendererEnums.h"

namespace fastbird
{
	DECLARE_SMART_PTR(RasterizerStateD3D11);
	class RasterizerStateD3D11 : public IPlatformRasterizerState
	{
		DECLARE_PIMPL_NON_COPYABLE(RasterizerStateD3D11);
		explicit RasterizerStateD3D11(ID3D11RasterizerState* rasterizerState);

	public:
		static RasterizerStateD3D11Ptr Create(ID3D11RasterizerState* rasterizerState);
		~RasterizerStateD3D11();

	public:

		//--------------------------------------------------------------------
		// IRasterizerState Interfacec
		//--------------------------------------------------------------------
		virtual void Bind();
		virtual void SetDebugName(const char* name);

		//--------------------------------------------------------------------
		// OWN Interfacec
		//--------------------------------------------------------------------		
		ID3D11RasterizerState* GetHardwareRasterizerState() const { return mRasterizerState; }

	private:
		ID3D11RasterizerState* mRasterizerState;
	};

	//-------------------------------------------------------------------------
	DECLARE_SMART_PTR(BlendStateD3D11);
	class BlendStateD3D11 : public IPlatformBlendState
	{
		DECLARE_PIMPL_NON_COPYABLE(BlendStateD3D11);
		explicit BlendStateD3D11(ID3D11BlendState* blendState);

	public:
		static BlendStateD3D11Ptr Create(ID3D11BlendState* blendState);
		~BlendStateD3D11();

	public:
		//--------------------------------------------------------------------
		// IBlendState Interfacec
		//--------------------------------------------------------------------
		virtual void Bind();
		virtual void SetDebugName(const char* name);

		//--------------------------------------------------------------------
		// OWN Interfacec
		//--------------------------------------------------------------------		
		ID3D11BlendState* GetHardwareBlendState() const { return mBlendState; }
		float* GetBlendFactor() const { return 0; }
		DWORD GetBlendMask() const { return 0xffffffff;}

	private:
		ID3D11BlendState* mBlendState;
	};

	DECLARE_SMART_PTR(DepthStencilStateD3D11);
	class DepthStencilStateD3D11 : public IPlatformDepthStencilState
	{
		DECLARE_PIMPL_NON_COPYABLE(DepthStencilStateD3D11);
		explicit DepthStencilStateD3D11(ID3D11DepthStencilState* depthStencilState);

	public:
		static DepthStencilStateD3D11Ptr Create(ID3D11DepthStencilState* depthStencilState);
		~DepthStencilStateD3D11();

	public:
		//--------------------------------------------------------------------
		// IDepthStencilState Interfacec
		//--------------------------------------------------------------------
		virtual void Bind(unsigned stencilRef);
		virtual void SetDebugName(const char* name);

		//--------------------------------------------------------------------
		// OWN Interfacec
		//--------------------------------------------------------------------		
		ID3D11DepthStencilState* GetHardwareDSState() const { return mDepthStencilState; }

	private:
		ID3D11DepthStencilState* mDepthStencilState;
	};

	DECLARE_SMART_PTR(SamplerStateD3D11);
	class SamplerStateD3D11 : public IPlatformSamplerState
	{
		DECLARE_PIMPL_NON_COPYABLE(SamplerStateD3D11);
		explicit SamplerStateD3D11(ID3D11SamplerState* samplerState);
	public:
		
		static SamplerStateD3D11Ptr Create(ID3D11SamplerState* samplerState);
		~SamplerStateD3D11();

	public:
		//--------------------------------------------------------------------
		// ISamplerState Interfacec
		//--------------------------------------------------------------------
		virtual void Bind(BINDING_SHADER shader, int slot);
		virtual void SetDebugName(const char* name);

		//--------------------------------------------------------------------
		// OWN Interfacec
		//--------------------------------------------------------------------		
		ID3D11SamplerState* GetHardwareSamplerState() const { return mSamplerState; }

	private:
		ID3D11SamplerState* mSamplerState;
	};
}