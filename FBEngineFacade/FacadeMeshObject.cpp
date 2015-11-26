#include "stdafx.h"
#include "FacadeMeshObject.h"
#include "EngineFacade.h"
#include "FBSceneObjectFactory/MeshObject.h"
#include "FBSceneObjectFactory/SceneObjectFactory.h"
#include "FBSceneManager/Scene.h"
using namespace fastbird;
class FacadeMeshObject::Impl{
public:
	MeshObjectPtr mMeshObject;

	//---------------------------------------------------------------------------
	Impl(){

	}

	~Impl(){

	}

	bool LoadDae(const char* daePath){
		auto loaded = SceneObjectFactory::GetInstance().CreateMeshObject(daePath);
		if (loaded){
			mMeshObject = loaded;
			return true;
		}
		return false;
	}

	void SetGameType(int type){
		if (!mMeshObject)
			return;
		mMeshObject->SetGameType(type);
	}

	void SetGameId(unsigned id){
		if (!mMeshObject)
			return;
		mMeshObject->SetGameId(id);
		
	}

	void SetGamePtr(void* ptr){
		if (!mMeshObject)
			return;
		mMeshObject->SetGamePtr(ptr);
	}

	/// Combination of SceneObjectFlag::Enum
	void ModifyObjFlag(unsigned flag, bool enable){
		if (!mMeshObject)
			return;
		mMeshObject->ModifyObjFlag(flag, enable);
	}

	void SetEnableHighlight(bool enable){
		if (!mMeshObject)
			return;
		mMeshObject->SetEnableHighlight(enable);
	}

	MaterialPtr GetMaterial() const{
		if (!mMeshObject)
			return 0;
		return mMeshObject->GetMaterial();
	}

	void SetMaterial(MaterialPtr material){
		if (!mMeshObject)
			return;
		mMeshObject->SetMaterial(material, PASS_NORMAL);
	}

	void SetMaterial(const char* path){
		if (!mMeshObject)
			return;
		mMeshObject->SetMaterial(path, PASS_NORMAL);
	}

	bool AttachToScene(){
		if (!mMeshObject)
			return false;
		auto scene = EngineFacade::GetInstance().GetMainScene();
		if (scene){
			return scene->AttachObjectFB(mMeshObject);
		}
		return false;
	}

	bool DetachFromScene(){
		if (!mMeshObject)
			return false;
		auto scene = EngineFacade::GetInstance().GetMainScene();
		if (scene){
			return scene->DetachObjectFB(mMeshObject);
		}
		return false;
	}

	bool IsAttached() const{
		if (!mMeshObject)
			return false;

		return mMeshObject->IsAttached();
	}

	void SetAlpha(float alpha){
		if (!mMeshObject)
			return;

		mMeshObject->SetAlpha(alpha);
	}

	void SetForceAlphaBlending(bool enable, float alpha, float forceGlow, bool disableDepth){
		if (!mMeshObject)
			return;
		mMeshObject->SetForceAlphaBlending(enable, alpha, forceGlow, disableDepth);
	}

	void SetAmbientColor(const Color& color){
		if (!mMeshObject)
			return;

		mMeshObject->SetAmbientColor(color);
	}

	const AUXILIARIES* GetAuxiliaries() const{
		if (!mMeshObject)
			return 0;
		return mMeshObject->GetAuxiliaries();
	}

	const Transformation& GetTransformation() const{
		if (!mMeshObject)
			return Transformation::IDENTITY;

		return mMeshObject->GetLocation();
	}

	void SetTransformation(const Transformation& transform){
		if (!mMeshObject)
			return;
		mMeshObject->SetLocation(transform);
	}

	void SetPosition(const Vec3& pos){
		if (!mMeshObject)
			return;
		mMeshObject->SetPosition(pos);
	}

	void SetRotation(const Quat& rot){
		if (!mMeshObject)
			return;
		mMeshObject->SetRotation(rot);
	}

	void SetScale(const Vec3& scale){
		if (!mMeshObject)
			return;
		mMeshObject->SetScale(scale);
	}

	const BoundingVolumePtr GetBoundingVolume() const{
		if (!mMeshObject)
			return 0;
		return mMeshObject->GetBoundingVolume();
	}

	const BoundingVolumePtr GetBoundingVolumeWorld() const{
		if (!mMeshObject)
			return 0;
		return mMeshObject->GetBoundingVolumeWorld();
	}

	bool RayCast(const Ray3& ray, Vec3& pos, const ModelTriangle** tri){
		if (!mMeshObject)
			return false;
		return mMeshObject->RayCast(ray, pos, tri);
	}

	bool CheckNarrowCollision(BoundingVolumePtr bv){
		if (!mMeshObject)
			return false;
		return mMeshObject->CheckNarrowCollision(bv);
	}

	Ray3::IResult CheckNarrowCollisionRay(const Ray3& ray){
		if (!mMeshObject)
			return Ray3::IResult{ false, FLT_MAX };
		return mMeshObject->CheckNarrowCollisionRay(ray);
	}

	bool HasCollisionShapes() const{
		if (!mMeshObject)
			return false;
		return mMeshObject->HasCollisionShapes();
	}

	Vec3 GetRandomPosInVolume(const Vec3* nearWorld){
		if (!mMeshObject)
			return Vec3::ZERO;

		return mMeshObject->GetRandomPosInVolume(nearWorld);
	}

	void PlayAction(const char* action, bool immediate, bool reverse){
		if (!mMeshObject)
			return;

		mMeshObject->PlayAction(action, immediate, reverse);
	}

	bool IsPlayingAction() const{
		if (!mMeshObject)
			return false;
		return mMeshObject->IsPlayingAction();
	}

};

//---------------------------------------------------------------------------
FB_IMPLEMENT_STATIC_CREATE(FacadeMeshObject);
FacadeMeshObject::FacadeMeshObject()
	: mImpl(new Impl)
{

}

FacadeMeshObject::~FacadeMeshObject(){

}

bool FacadeMeshObject::LoadDae(const char* daePath) {
	return mImpl->LoadDae(daePath);
}

void FacadeMeshObject::SetGameType(int type) {
	mImpl->SetGameType(type);
}

void FacadeMeshObject::SetGameId(unsigned id) {
	mImpl->SetGameId(id);
}

void FacadeMeshObject::SetGamePtr(void* ptr) {
	mImpl->SetGamePtr(ptr);
}

void FacadeMeshObject::ModifyObjFlag(unsigned flag, bool enable) {
	mImpl->ModifyObjFlag(flag, enable);
}

void FacadeMeshObject::SetEnableHighlight(bool enable) {
	mImpl->SetEnableHighlight(enable);
}

MaterialPtr FacadeMeshObject::GetMaterial() const {
	return mImpl->GetMaterial();
}

void FacadeMeshObject::SetMaterial(MaterialPtr material) {
	mImpl->SetMaterial(material);
}

void FacadeMeshObject::SetMaterial(const char* path) {
	mImpl->SetMaterial(path);
}

bool FacadeMeshObject::AttachToScene() {
	return mImpl->AttachToScene();
}

bool FacadeMeshObject::DetachFromScene() {
	return mImpl->DetachFromScene();
}

bool FacadeMeshObject::IsAttached() const {
	return mImpl->IsAttached();
}

void FacadeMeshObject::SetAlpha(float alpha) {
	mImpl->SetAlpha(alpha);
}

void FacadeMeshObject::SetForceAlphaBlending(bool enable, float alpha, float forceGlow, bool disableDepth) {
	mImpl->SetForceAlphaBlending(enable, alpha, forceGlow, disableDepth);
}

void FacadeMeshObject::SetAmbientColor(const Color& color) {
	mImpl->SetAmbientColor(color);
}

const AUXILIARIES* FacadeMeshObject::GetAuxiliaries() const {
	return mImpl->GetAuxiliaries();
}

const Transformation& FacadeMeshObject::GetTransformation() const {
	return mImpl->GetTransformation();
}

void FacadeMeshObject::SetTransformation(const Transformation& transform) {
	mImpl->SetTransformation(transform);
}

void FacadeMeshObject::SetPosition(const Vec3& pos) {
	mImpl->SetPosition(pos);
}

void FacadeMeshObject::SetRotation(const Quat& rot) {
	mImpl->SetRotation(rot);
}

void FacadeMeshObject::SetScale(const Vec3& scale) {
	mImpl->SetScale(scale);
}

const BoundingVolumePtr FacadeMeshObject::GetBoundingVolume() const {
	return mImpl->GetBoundingVolume();
}

const BoundingVolumePtr FacadeMeshObject::GetBoundingVolumeWorld() const {
	return mImpl->GetBoundingVolumeWorld();
}

bool FacadeMeshObject::RayCast(const Ray3& ray, Vec3& pos, const ModelTriangle** tri) {
	return mImpl->RayCast(ray, pos, tri);
}

bool FacadeMeshObject::CheckNarrowCollision(BoundingVolumePtr bv) {
	return mImpl->CheckNarrowCollision(bv);
}

Ray3::IResult FacadeMeshObject::CheckNarrowCollisionRay(const Ray3& ray) {
	return mImpl->CheckNarrowCollisionRay(ray);
}

bool FacadeMeshObject::HasCollisionShapes() const {
	return mImpl->HasCollisionShapes();
}

Vec3 FacadeMeshObject::GetRandomPosInVolume(const Vec3* nearWorld) {
	return mImpl->GetRandomPosInVolume(nearWorld);
}

void FacadeMeshObject::PlayAction(const char* action, bool immediate, bool reverse) {
	mImpl->PlayAction(action, immediate, reverse);
}

bool FacadeMeshObject::IsPlayingAction() const {
	return mImpl->IsPlayingAction();
}

