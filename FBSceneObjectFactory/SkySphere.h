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
#include "FBCommonHeaders/platform.h"
#include "FBSceneManager/SceneObject.h"
#include "FBRenderer/IRenderable.h"
namespace fastbird{
	DECLARE_SMART_PTR(SkySphere);
	class FB_DLL_PUBLIC SkySphere : public SceneObject, public IRenderable{
		DECLARE_PIMPL_NON_COPYABLE(SkySphere);
		SkySphere();

	public:
		static SkySpherePtr Create();
		static void CreateSharedEnvRT();
		static void DeleteSharedEnvRT();

		~SkySphere();

		// IRenderable
		virtual void SetMaterial(const char* name, int pass) = 0;
		virtual void SetMaterial(MaterialPtr pMat, int pass) = 0;
		virtual MaterialPtr GetMaterial(int pass = 0) const = 0;
		virtual bool IsTransparent() const = 0;
		virtual void SetVertexBuffer(VertexBufferPtr pVertexBuffer) = 0;
		virtual void SetIndexBuffer(IndexBufferPtr pIndexBuffer) = 0;
		// override the input layout defined in material
		virtual void SetInputLayout(InputLayoutPtr i) = 0;
		virtual void PreRender(const RenderParam& param, RenderParamOut* paramOut) = 0;
		virtual void Render(const RenderParam& param, RenderParamOut* paramOut) = 0;
		virtual void PostRender(const RenderParam& param, RenderParamOut* paramOut) = 0;
		virtual void SetEnableHighlight(bool enable) {}
		virtual AnimationPtr GetAnimation() const { return 0; }

		// ISkySphere
		virtual void UpdateEnvironmentMap(const Vec3& origin);
		virtual void SetInterpolationData(unsigned index, const Vec4& data);
		virtual void PrepareInterpolation(float time, SkySpherePtr startFrom);
		virtual void SetUseAlphaBlend(bool use);
		virtual void SetAlpha(float alpha);
		virtual float GetAlpha() const;
	};
}