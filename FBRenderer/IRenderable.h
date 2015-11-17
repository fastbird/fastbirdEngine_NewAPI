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