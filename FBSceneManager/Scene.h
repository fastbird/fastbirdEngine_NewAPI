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
#include "FBCommonHeaders/Types.h"
#include "SceneManager.h"
#include "ISceneObserver.h"
#include "FBRenderer/RenderParam.h"
#include "FBCommonHeaders/Observable.h"
namespace fastbird{
	class Vec3;
	class Color;
	class Ray3;
	DECLARE_SMART_PTR(Camera);
	DECLARE_SMART_PTR(SkySphere);
	DECLARE_SMART_PTR(SpatialObject);
	DECLARE_SMART_PTR(SceneObject);
	DECLARE_SMART_PTR(DirectionalLight);
	DECLARE_SMART_PTR(Scene);
	class FB_DLL_SCENEMANAGER Scene : public Observable<ISceneObserver>{
		DECLARE_PIMPL_NON_COPYABLE(Scene);		
		Scene(const char* name);		
		
	public:		
		typedef std::vector<SpatialObject*> SPATIAL_OBJECTS_RAW;
		static ScenePtr Create(const char* name);		
		
		const char* GetName() const;
		void PreRender(const RenderParam& prarm, RenderParamOut* paramOut);
		void Render(const RenderParam& prarm, RenderParamOut* paramOut);

		/// Returns currently rendering pass which is RenderParam.mRenderPass when the Render() is called.
		int GetRenderPass() const;

		bool AttachObject(SceneObjectPtr pObject);
		bool DetachObject(SceneObjectPtr pObject);

		bool AttachObject(SpatialObjectPtr pSpatialObject);
		bool DetachObject(SpatialObjectPtr pSpatialObject);
		void SetSkipSpatialObjects(bool skip);
		void ClearEverySpatialObject();
		unsigned GetNumSpatialObjects() const;
		/** You do not own the returned pointers of this function
		do not keep any pointer. */
		const SPATIAL_OBJECTS_RAW& GetVisibleSpatialList(CameraPtr cam);
		void PrintSpatialObject();

		void AttachSkySphere(SkySpherePtr p);		
		void DetachSkySphere();				
		SkySpherePtr GetSkySphere();		
		void ToggleSkyRendering();
		void SetSkyRendering(bool render);
		bool GetSkyRendering();		

		const Vec3& GetWindVector() const;

		/** p is a MeshObject*/
		void AddCloudVolume(SpatialObjectPtr p);
		void ClearClouds();
		void PreRenderCloudVolumes(const RenderParam& prarm, RenderParamOut* paramOut);
		void RenderCloudVolumes(const RenderParam& prarm, RenderParamOut* paramOut);
				
		const Color& GetFogColor() const;
		void SetFogColor(const Color& c);
		void SetDrawClouds(bool e);

		/** True if this scene is attached in the render target which is not related swap-chain.*/
		void SetRttScene(bool set);
		bool IsRttScene() const;

		DirectionalLightPtr GetDirectionalLight(unsigned idx);				
	};
}