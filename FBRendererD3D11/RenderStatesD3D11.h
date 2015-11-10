#pragma once
#include "FBRenderer/IRenderStates.h"
#include "FBRenderer/RendererEnums.h"

namespace fastbird
{
	//-------------------------------------------------------------------------
	class RasterizerStateD3D11 : public IRasterizerState
	{
	public:
		explicit RasterizerStateD3D11(ID3D11RasterizerState* rasterizerState);
		virtual ~RasterizerStateD3D11();

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
	class BlendStateD3D11 : public IBlendState
	{
	public:
		explicit BlendStateD3D11(ID3D11BlendState* blendState);
		virtual ~BlendStateD3D11();

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

	//-------------------------------------------------------------------------
	class DepthStencilStateD3D11 : public IDepthStencilState
	{
	public:
		explicit DepthStencilStateD3D11(ID3D11DepthStencilState* depthStencilState);
		virtual ~DepthStencilStateD3D11();

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

	class SamplerStateD3D11 : public ISamplerState
	{
	public:
		SamplerStateD3D11(ID3D11SamplerState* samplerState);
		virtual ~SamplerStateD3D11();

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