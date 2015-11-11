#pragma once
#include "IRenderStates.h"
#include "RendererStructs.h"
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	DECLARE_SMART_PTR(RenderStates);
	//------------------------------------------------------------------------
	class FB_DLL_PUBLIC RenderStates
	{
		DECLARE_PIMPL(RenderStates);

		IRasterizerStatePtr mRasterizerState;
		IBlendStatePtr mBlendState;
		IDepthStencilStatePtr mDepthStencilState;

		RASTERIZER_DESC mRDesc;
		BLEND_DESC mBDesc;
		DEPTH_STENCIL_DESC mDDesc;

	public:
		RenderStates();
		~RenderStates();

		void Reset();
		void ResetRasterizerState();
		void ResetBlendState();
		void ResetDepthStencilState();
		void CreateRasterizerState(const RASTERIZER_DESC& desc);
		void CreateBlendState(const BLEND_DESC& desc);
		void CreateDepthStencilState(const DEPTH_STENCIL_DESC& desc);
		void Bind();
		void Bind(unsigned stencilRef);
		RenderStatesPtr Clone() const;
	};
}