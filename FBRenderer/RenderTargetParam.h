#pragma once
#include "RendererEnums.h"
#include "FBMathLib/Vec2I.h"
namespace fastbird{
	struct RenderTargetParam
	{
		RenderTargetParam()
			: mEveryFrame(false)
			, mSize(200, 200)
			, mPixelFormat(PIXEL_FORMAT_R8G8B8A8_UNORM)
			, mShaderResourceView(false)
			, mMipmap(false)
			, mCubemap(false)
			, mWillCreateDepth(false)
			, mUsePool(false)
		{

		}
		bool mEveryFrame;
		Vec2I mSize;
		PIXEL_FORMAT mPixelFormat;
		bool mShaderResourceView;
		bool mMipmap;
		bool mCubemap;
		bool mWillCreateDepth; // set true, and call IRenderTarget::SetDepthStencilDesc().
		bool mUsePool;
	};
}