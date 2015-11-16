#pragma once
#define FB_RENDERABLE_VERSION 1
#include "InputLayout.h"
#include "FBCommonHeaders/platform.h"
#include "FBMathLib/Vec3.h"
namespace fastbird{		
	struct RenderParam;
	struct RenderParamOut;
	DECLARE_SMART_PTR(Animation);	
	DECLARE_SMART_PTR(IndexBuffer);
	DECLARE_SMART_PTR(VertexBuffer);
	DECLARE_SMART_PTR(Material);
	DECLARE_SMART_PTR(Scene);
	DECLARE_SMART_PTR(IRenderable);
	class FB_DLL_PUBLIC IRenderable
	{
	public:
		virtual void SetMaterial(const char* name, int pass) = 0;
		virtual void SetMaterial(MaterialPtr pMat, int pass) = 0;
		virtual MaterialPtr GetMaterial(int pass = 0) const = 0;
		virtual void SetVertexBuffer(VertexBufferPtr pVertexBuffer) = 0;
		virtual void SetIndexBuffer(IndexBufferPtr pIndexBuffer) = 0;
		// override the input layout defined in material
		virtual void SetInputLayout(InputLayoutPtr i) = 0;		

		virtual void PreRender(const RenderParam& renderParam, RenderParamOut* renderParamOut) = 0;
		virtual void Render(const RenderParam& renderParam, RenderParamOut* renderParamOut) = 0;
		virtual void PostRender(const RenderParam& renderParam, RenderParamOut* renderParamOut) = 0;

		virtual void SetEnableHighlight(bool enable) {}
		virtual AnimationPtr GetAnimation() const { return 0; }

	protected:
		virtual ~IRenderable() {}
	};
}