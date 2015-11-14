#pragma once
#include "IPlatformRenderStates.h"
#include "RendererStructs.h"
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	typedef IPlatformRasterizerStatePtr RasterizerStatePtr;
	typedef IPlatformRasterizerStateWeakPtr RasterizerStateWeakPtr;
	typedef IPlatformBlendStatePtr BlendStatePtr;
	typedef IPlatformBlendStateWeakPtr BlendStateWeakPtr;
	typedef IPlatformDepthStencilStatePtr DepthStencilStatePtr;
	typedef IPlatformDepthStencilStateWeakPtr DepthStencilStateWeakPtr;
	typedef IPlatformSamplerStatePtr SamplerStatePtr;
	typedef IPlatformSamplerStateWeakPtr SamplerStateWeakPtr;
	////---------------------------------------------------------------------------
	//DECLARE_SMART_PTR(RasterizerState);
	//class FB_DLL_PUBLIC RasterizerState{
	//	DECLARE_PIMPL_NON_COPYABLE(RasterizerState);
	//	RasterizerState();

	//public:
	//	static RasterizerStatePtr Create();
	//	~RasterizerState();

	//	void SetPlatformState(IRasterizerStatePtr state);
	//	IRasterizerStatePtr GetPlatformState() const;
	//	void SetDescription(const RASTERIZER_DESC& desc);
	//	const RASTERIZER_DESC& GetDescription() const;
	//	void ApplyDescription();

	//	void Bind();
	//	void SetDebugName(const char* name);
	//};

	////---------------------------------------------------------------------------
	//DECLARE_SMART_PTR(BlendState);
	//class FB_DLL_PUBLIC BlendState{
	//	DECLARE_PIMPL_NON_COPYABLE(BlendState);

	//public:
	//	static BlendStatePtr Create();
	//	~BlendState();

	//	void SetPlatformState(IBlendStatePtr state);
	//	IBlendStatePtr GetPlatformState() const;
	//	void SetDescription(const BLEND_DESC& desc);
	//	const BLEND_DESC& GetDescription() const;
	//	void ApplyDescription();

	//	void Bind();
	//	void SetDebugName(const char* name);
	//};

	////---------------------------------------------------------------------------
	//DECLARE_SMART_PTR(DepthStencilState);
	//class FB_DLL_PUBLIC DepthStencilState{
	//	DECLARE_PIMPL_NON_COPYABLE(DepthStencilState);

	//public:
	//	static DepthStencilStatePtr Create();
	//	~DepthStencilState();

	//	void SetPlatformState(IDepthStencilStatePtr state);
	//	IDepthStencilStatePtr GetPlatformState() const;
	//	void SetDescription(const DEPTH_STENCIL_DESC& desc);
	//	const DEPTH_STENCIL_DESC& GetDescription() const;
	//	void ApplyDescription();

	//	void Bind();
	//	void SetDebugName(const char* name);
	//};

	////---------------------------------------------------------------------------
	//DECLARE_SMART_PTR(SamplerState);
	//class FB_DLL_PUBLIC SamplerState{
	//	DECLARE_PIMPL_NON_COPYABLE(SamplerState);

	//public:
	//	static SamplerStatePtr Create();
	//	~SamplerState();

	//	void SetPlatformState(ISamplerStatePtr state);
	//	ISamplerStatePtr GetPlatformState() const;
	//	void SetDescription(const SAMPLER_DESC& desc);
	//	const SAMPLER_DESC& GetDescription() const;
	//	void ApplyDescription();

	//	void Bind();
	//	void SetDebugName(const char* name);
	//};

	//---------------------------------------------------------------------------
	DECLARE_SMART_PTR(RenderStates);
	/** represents RasterizerState, BlendState and DepthStencilState as one integrated object.
	RenderStates usually resides in Material. Internal data will be efficiently shared among
	RenderStates objects indicating the same states.
	*/
	class FB_DLL_PUBLIC RenderStates
	{
		DECLARE_PIMPL(RenderStates);
		RenderStates();

	public:
		static RenderStatesPtr Create();
		RenderStates(const RenderStates& other);
		~RenderStates();

		void Reset();
		void ResetRasterizerState();
		void ResetBlendState();
		void ResetDepthStencilState();
		void CreateRasterizerState(const RASTERIZER_DESC& desc);
		void CreateBlendState(const BLEND_DESC& desc);
		void CreateDepthStencilState(const DEPTH_STENCIL_DESC& desc);
		void Bind() const;
		void Bind(unsigned stencilRef) const;
	};
}