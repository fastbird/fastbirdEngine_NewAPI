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
#include "Scene.h"
#include "DirectionalLight.h"
#include "Camera.h"
#include "SpatialObject.h"
#include "FBRenderer/RenderPass.h"
#include "FBRenderer/IRenderable.h"
#include "FBMathLib/Color.h"
#include "FBCommonHeaders/VectorMap.h"
#include "FBTimer/Timer.h"
#include "FBStringLib/StringLib.h"
#include "FBSceneObjectFactory/SkySphere.h"// this doesn't make denpendency
using namespace fastbird;

class Scene::Impl{
public:
	Scene* mSelf;
	SceneWeakPtr mSelfPtr;
	std::string mName;
	RENDER_PASS mRenderPass; // processing render pass
	typedef std::vector<SpatialObjectPtr> SPATIAL_OBJECTS;
	typedef std::vector<SpatialObjectWeakPtr> SPATIAL_OBJECTS_WEAK;
	typedef std::vector<SpatialObject*> SPATIAL_OBJECTS_RAW;
	typedef std::vector<SceneObjectPtr> OBJECTS;
	typedef std::vector<SceneObjectWeakPtr> OBJECTS_WEAK;
	typedef std::vector<SceneObject*> OBJECTS_RAW;
	OBJECTS_WEAK mObjects;
	SPATIAL_OBJECTS_WEAK mSpatialObjects;
	VectorMap<Camera*, SPATIAL_OBJECTS_RAW> mVisibleObjectsMain;
	VectorMap<Camera*, SPATIAL_OBJECTS_RAW> mVisibleObjectsLight;
	VectorMap<Camera*, SPATIAL_OBJECTS_RAW> mPreRenderList;
	VectorMap<Camera*, SPATIAL_OBJECTS_RAW> mVisibleTransparentObjects;	
	SkySpherePtr mSkySphere;
	DirectionalLightPtr mDirectionalLight[2];
	bool mSkipSpatialObjects;
	bool mSkyRendering;
	std::mutex mSpatialObjectsMutex;
	std::vector< SpatialObjectPtr > mCloudVolumes;
	Vec3 mWindDir;
	float mWindVelocity;
	Vec3 mWindVector;
	Color mFogColor;
	VectorMap<Camera*, unsigned> mLastPreRenderFramePerCam;
	bool mDrawClouds;
	bool mRttScene;

	Impl(Scene* self, const char* name)
		: mSelf(self)
		, mName(name)
		, mRenderPass(PASS_NORMAL)
		, mSkyRendering(true)
		, mSkipSpatialObjects(false)
		, mDrawClouds(true)
		, mRttScene(false)
		, mFogColor(0, 0, 0)
		, mWindDir(1, 0, 0)
		, mWindVelocity(0.f)
	{
		mWindVector = mWindDir * mWindVelocity;

		// Light
		for (int i = 0; i < 2; ++i)
		{
			mDirectionalLight[i] = DirectionalLight::Create();
			mDirectionalLight[i]->SetIntensity(1.0f);
		}

		mDirectionalLight[0]->SetDirection(Vec3(-3, 1, 1));
		mDirectionalLight[0]->SetDiffuse(Vec3(1, 1, 1));
		mDirectionalLight[0]->SetSpecular(Vec3(1, 1, 1));

		mDirectionalLight[1]->SetDirection(Vec3(3, 1, -1));
		mDirectionalLight[1]->SetDiffuse(Vec3(0.8f, 0.4f, 0.1f));
		mDirectionalLight[1]->SetSpecular(Vec3(0, 0, 0));
	}

	const char* GetName() const{
		return mName.c_str();
	}

	void MakeVisibleSet(const RenderParam& renderParam, RenderParamOut* renderParamOut)
	{
		if (mSkipSpatialObjects)
			return;

		auto mainCam = renderParam.mCamera;
		auto lightCam = renderParam.mLightCamera;
		mVisibleObjectsMain[mainCam].clear();
		mVisibleObjectsLight[lightCam].clear();
		mVisibleTransparentObjects[mainCam].clear();
		mPreRenderList[mainCam].clear();

		if (!mainCam)
		{
			Logger::Log(FB_FRAME_TIME, FB_ERROR_LOG_ARG, "Invalid main camera");
			return;
		}
		if (!lightCam){
			Logger::Log(FB_FRAME_TIME, FB_ERROR_LOG_ARG, "Invalid light camera");
			return;
		}

		{
			MutexLock lock(mSpatialObjectsMutex);			
			for (auto it = mSpatialObjects.begin(); it != mSpatialObjects.end(); /**/)
			{
				auto obj = it->lock();
				if (!obj){
					it = mSpatialObjects.erase(it);
					continue;
				}

				if (obj->HasObjFlag(SceneObjectFlag::Ignore) || !obj->GetRenderable()){
					++it;
					continue;
				}
				++it;

				bool inserted = false;
				if (!mainCam->IsCulled(obj->GetBoundingVolumeWorld().get())){				
					if (obj->HasObjFlag(SceneObjectFlag::Transparent))
					{
						mVisibleTransparentObjects[mainCam].push_back(obj.get());
					}
					else
					{
						mVisibleObjectsMain[mainCam].push_back(obj.get());
					}
					inserted = true;
				}

				if (lightCam && !lightCam->IsCulled(obj->GetBoundingVolumeWorld().get()))
				{
					mVisibleObjectsLight[lightCam].push_back((obj.get()));
					inserted = true;
				}
				if (inserted)
					mPreRenderList[mainCam].push_back((obj.get()));
			}
		}

		const fastbird::Vec3& camPos = mainCam->GetPosition();
		for (const auto obj : mPreRenderList[mainCam])
		{
			assert(obj);
			const Vec3& objPos = obj->GetPosition();
			float dist = (camPos - objPos).Length();
			obj->SetDistToCam(dist);
		}

		std::sort(mVisibleObjectsMain[mainCam].begin(), mVisibleObjectsMain[mainCam].end(),
			[](SpatialObject* a, SpatialObject* b) -> bool
		{
			return a->GetDistToCam() < b->GetDistToCam();
		}
		);

		std::sort(mVisibleObjectsLight[lightCam].begin(), mVisibleObjectsLight[lightCam].end(),
			[](SpatialObject* a, SpatialObject* b) -> bool
		{
			return a->GetDistToCam() < b->GetDistToCam();
		}
		);

		std::sort(mVisibleTransparentObjects[mainCam].begin(), mVisibleTransparentObjects[mainCam].end(),
			[](SpatialObject* a, SpatialObject* b) -> bool
		{
			return a->GetDistToCam() > b->GetDistToCam();
		}
		);

		auto& observers = mSelf->mObservers_[ISceneObserver::Timing];
		for (auto it = observers.begin(); it != observers.end(); /**/){
			auto observer = it->lock();
			if (!observer){
				it = observers.erase(it);
				continue;
			}
			++it;

			observer->OnAfterMakeVisibleSet(mSelf, renderParam, renderParamOut);
		}		
	}

	void PreRender(const RenderParam& renderParam, RenderParamOut* renderParamOut){
		mRenderPass = (RENDER_PASS)renderParam.mRenderPass;
		if (!mSkipSpatialObjects)
		{
			auto cam = renderParam.mCamera;
			assert(cam);

			auto it = mLastPreRenderFramePerCam.Find(cam);
			if (it != mLastPreRenderFramePerCam.end() && it->second == gpTimer->GetFrame())
				return;
			mLastPreRenderFramePerCam[cam] = gpTimer->GetFrame();

			MakeVisibleSet(renderParam, renderParamOut);

			auto objIt = mPreRenderList[cam].begin(), objItEnd = mPreRenderList[cam].end();
			for (; objIt != objItEnd; objIt++)
			{
				// Only objects that have a valid renderable is in the render lists.
				auto renderable = (*objIt)->GetRenderable();
				renderable->PreRender(renderParam, renderParamOut);
			}
		}

		if (mSkyRendering)
		{
			if (mSkySphere)
			{
				mSkySphere->PreRender(renderParam, renderParamOut);
			}
		}

		for (auto it = mObjects.begin(); it != mObjects.end(); /**/){
			auto obj = it->lock();
			if (!obj){
				it = mObjects.erase(it);
				continue;
			}
			++it;
			auto renderable = obj->GetRenderable();
			if (renderable)
				renderable->PreRender(renderParam, renderParamOut);
		}
	}

	void Render(const RenderParam& param, RenderParamOut* paramOut){
		mRenderPass = (RENDER_PASS)param.mRenderPass;
		auto lightCamera = param.mLightCamera;
		auto cam = param.mCamera;
		if (!mSkipSpatialObjects)
		{
			if (param.mRenderPass == PASS_SHADOW)
			{
				for (auto& obj : mVisibleObjectsLight[lightCamera])
				{
					auto renderable = obj->GetRenderable();
					renderable->Render(param, paramOut);
				}
			}
			else
			{
				auto& observers = mSelf->mObservers_[ISceneObserver::Timing];
				for (auto it = observers.begin(); it != observers.end(); /**/){
					auto observer = it->lock();
					if (!observer){
						it = observers.erase(it);
						continue;
					}
					++it;
					observer->OnBeforeRenderingOpaques(mSelf, param, paramOut);
				}

				for (auto& obj : mVisibleObjectsMain[cam])
				{
					auto renderable = obj->GetRenderable();
					renderable->Render(param, paramOut);
				}
			}
		}

		if (!(param.mRenderPass == RENDER_PASS::PASS_SHADOW || param.mRenderPass == RENDER_PASS::PASS_DEPTH))
		{
			if (mSkyRendering)
			{
				if (mSkySphere)
				{
					mSkySphere->Render(param, paramOut);
				}

			}

			for (auto it : mSelf->mObservers_[ISceneObserver::Timing]){
				auto observer = it.lock();
				if (observer){
					observer->OnBeforeRenderingTransparents(mSelf, param, paramOut);
				}
			}

			if (!mSkipSpatialObjects)
			{
				auto it = mVisibleTransparentObjects[cam].begin(), itEnd = mVisibleTransparentObjects[cam].end();
				for (; it != itEnd; it++)
				{
					auto renderable = (*it)->GetRenderable();
					renderable->Render(param, paramOut);
				}
			}
			{
				for (auto it : mObjects){
					auto obj = it.lock();
					if (obj){
						auto renderable = obj->GetRenderable();
						if (renderable){
							renderable->Render(param, paramOut);
						}
					}
				}
			}
		}
	}

	/// Returns currently rendering pass which is RenderParam.mRenderPass when the Render() is called.
	int GetRenderPass() const{
		return mRenderPass;
	}


	bool AttachObject(SceneObjectPtr pObject){
		if (!ValueExistsInVector(mObjects, pObject)){
			mObjects.push_back(pObject);
			pObject->OnAttachedToScene(mSelfPtr.lock());
			return true;
		}

		return false;
	}

	bool DetachObject(SceneObjectPtr pObject){
		DeleteValuesInVector(mObjects, pObject);
		if (mObjectsDeletedAny) {
			pObject->OnDetachedFromScene(mSelfPtr.lock());
		}
		return mObjectsDeletedAny;
	}


	bool AttachSpatialObject(SpatialObjectPtr pSpatialObject){
		if (!ValueExistsInVector(mSpatialObjects, pSpatialObject)){
			mSpatialObjects.push_back(pSpatialObject);
			pSpatialObject->OnAttachedToScene(mSelfPtr.lock());
			return true;
		}
		return false;
	}

	bool DetachSpatialObject(SpatialObjectPtr pSpatialObject){
		DeleteValuesInVector(mSpatialObjects, pSpatialObject);
		if (mSpatialObjectsDeletedAny) {
			pSpatialObject->OnDetachedFromScene(mSelfPtr.lock());
		}
		return mSpatialObjectsDeletedAny;
	}

	void SetSkipSpatialObjects(bool skip){
		mSkipSpatialObjects = skip;
	}

	void ClearEverySpatialObject(){
		mSpatialObjects.clear();
	}

	unsigned GetNumSpatialObjects() const{
		return mSpatialObjects.size();
	}

	const SPATIAL_OBJECTS_RAW& GetVisibleSpatialList(CameraPtr cam){
		return mVisibleObjectsMain[cam.get()];
	}

	void PrintSpatialObject(){
		for (auto it = mSpatialObjects.begin(); it != mSpatialObjects.end(); /**/){
			auto obj = it->lock();
			if (!obj){
				it = mSpatialObjects.erase(it);
				continue;
			}
			++it;
		}

		Logger::Log(FB_ERROR_LOG_ARG, FormatString("void Scene::PrintSpatialObject(): num:  %u", mSpatialObjects.size()).c_str());
		for (auto it : mSpatialObjects)
		{
			auto obj = it.lock();
			if (!obj)
				continue;
			auto name = obj->GetName();
			if (name && strlen(name) != 0)
			{
				Logger::Log(FB_DEFAULT_LOG_ARG_NO_LINE, "Spatial Object Name = %s	", name);
			}
			else
			{
				int gameType = obj->GetGameType();
				Logger::Log("GameType = %d\n", gameType);				
			}
		}
	}

	void AttachSkySphere(SkySpherePtr p){
		if (mSkySphere)
			mSkySphere->OnDetachedFromScene(mSelfPtr.lock());
		mSkySphere = p;
		if (mSkySphere){			
			mSkySphere->OnAttachedToScene(mSelfPtr.lock());
		}
	}

	void DetachSkySphere(){
		mSkySphere = 0;
	}

	SkySpherePtr GetSkySphere(){
		return mSkySphere;
	}

	void ToggleSkyRendering(){
		mSkyRendering = !mSkyRendering;
	}

	void SetSkyRendering(bool render){
		mSkyRendering = render;
	}

	bool GetSkyRendering(){
		return mSkyRendering;
	}

	const Vec3& GetWindVector() const{
		return mWindVector;
	}

	/** p is a MeshObject*/
	void AddCloudVolume(SpatialObjectPtr p){
		mCloudVolumes.push_back(p);
	}

	void ClearClouds(){
		mCloudVolumes.clear();
	}

	void PreRenderCloudVolumes(const RenderParam& param, RenderParamOut* paramOut){
		for (auto var : mCloudVolumes)
		{
			auto renderable = var->GetRenderable();
			renderable->PreRender(param, paramOut);			
		}
	}

	void RenderCloudVolumes(const RenderParam& param, RenderParamOut* paramOut){
		for (auto var : mCloudVolumes)
		{
			auto renderable = var->GetRenderable();
			renderable->Render(param, paramOut);
		}
	}


	const Color& GetFogColor() const{
		return mFogColor;
	}

	void SetFogColor(const Color& c){
		mFogColor = c;
	}

	void SetDrawClouds(bool e){
		mDrawClouds = e;
	}

	/** True if this scene is attached in the render target which is not related swap-chain.*/
	void SetRttScene(bool set){
		mRttScene = set;
	}

	bool IsRttScene() const{
		return mRttScene;
	}

	DirectionalLightPtr GetDirectionalLight(unsigned idx){
		assert(idx < 2);
		return mDirectionalLight[idx];
	}
};

//---------------------------------------------------------------------------
ScenePtr Scene::Create(const char* name){
	if (!ValidCStringLength(name)){
		Logger::Log(FB_ERROR_LOG_ARG, "Scene should have a valid name.");
		return 0;
	}
	auto p =  ScenePtr(new Scene(name), [](Scene* obj){ delete obj; });
	p->mImpl->mSelfPtr = p;
	return p;
}

Scene::Scene(const char* name)
	:mImpl(new Impl(this, name))
{
}

const char* Scene::GetName() const {
	return mImpl->GetName();
}

void Scene::PreRender(const RenderParam& prarm, RenderParamOut* paramOut) {
	mImpl->PreRender(prarm, paramOut);
}

void Scene::Render(const RenderParam& prarm, RenderParamOut* paramOut) {
	mImpl->Render(prarm, paramOut);
}

/// Returns currently rendering pass which is RenderParam.mRenderPass when the Render() is called.
int Scene::GetRenderPass() const {
	return mImpl->GetRenderPass();
}

template<typename T, typename std::enable_if_t< std::is_same<T, SceneObject>::value >*>
bool Scene::AttachObject(std::shared_ptr<T> pObject) {
#if defined(_DEBUG)
	if (std::dynamic_pointer_cast<SpatialObject>(pObject)){
		Logger::Log(FB_ERROR_LOG_ARG, "A SpatialObject is added as a SceneObject. Consider using Scene::AttachSpatialObject() rather than Scene::AttachObject()");
	}
#endif
	return mImpl->AttachObject(pObject);
}

template<typename T, typename std::enable_if_t< std::is_same<T, SceneObject>::value >*>
bool Scene::DetachObject(std::shared_ptr<T> pObject) {
	return mImpl->DetachObject(pObject);
}

template<typename T, typename std::enable_if_t<std::is_base_of<SpatialObject, T>::value>*>
bool Scene::AttachObject(std::shared_ptr<T> pSpatialObject) {
	return mImpl->AttachSpatialObject(pSpatialObject);
}

template<typename T, typename std::enable_if_t<std::is_base_of<SpatialObject, T>::value>*>
bool Scene::DetachObject(std::shared_ptr<T> pSpatialObject) {
	return mImpl->DetachSpatialObject(pSpatialObject);
}

void Scene::SetSkipSpatialObjects(bool skip) {
	mImpl->SetSkipSpatialObjects(skip);
}

void Scene::ClearEverySpatialObject() {
	mImpl->ClearEverySpatialObject();
}

unsigned Scene::GetNumSpatialObjects() const {
	return mImpl->GetNumSpatialObjects();
}

const Scene::SPATIAL_OBJECTS_RAW& Scene::GetVisibleSpatialList(CameraPtr cam) {
	return mImpl->GetVisibleSpatialList(cam);
}

void Scene::PrintSpatialObject() {
	mImpl->PrintSpatialObject();
}

void Scene::AttachSkySphere(SkySpherePtr p) {
	mImpl->AttachSkySphere(p);
}

void Scene::DetachSkySphere() {
	mImpl->DetachSkySphere();
}

SkySpherePtr Scene::GetSkySphere() {
	return mImpl->GetSkySphere();
}

void Scene::ToggleSkyRendering() {
	mImpl->ToggleSkyRendering();
}

void Scene::SetSkyRendering(bool render) {
	mImpl->SetSkyRendering(render);
}

bool Scene::GetSkyRendering() {
	return mImpl->GetSkyRendering();
}

const Vec3& Scene::GetWindVector() const {
	return mImpl->GetWindVector();
}

void Scene::AddCloudVolume(SpatialObjectPtr p) {
	mImpl->AddCloudVolume(p);
}

void Scene::ClearClouds() {
	mImpl->ClearClouds();
}

void Scene::PreRenderCloudVolumes(const RenderParam& prarm, RenderParamOut* paramOut) {
	mImpl->PreRenderCloudVolumes(prarm, paramOut);
}

void Scene::RenderCloudVolumes(const RenderParam& prarm, RenderParamOut* paramOut) {
	mImpl->RenderCloudVolumes(prarm, paramOut);
}

const Color& Scene::GetFogColor() const {
	return mImpl->GetFogColor();
}

void Scene::SetFogColor(const Color& c) {
	mImpl->SetFogColor(c);
}

void Scene::SetDrawClouds(bool e) {
	mImpl->SetDrawClouds(e);
}

void Scene::SetRttScene(bool set) {
	mImpl->SetRttScene(set);
}

bool Scene::IsRttScene() const {
	return mImpl->IsRttScene();
}

DirectionalLightPtr Scene::GetDirectionalLight(unsigned idx) {
	return mImpl->GetDirectionalLight(idx);
}