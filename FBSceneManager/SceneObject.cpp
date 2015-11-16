#include "stdafx.h"
#include "SceneObject.h"
#include "FBCommonHeaders/Helpers.h"
#include <vector>
using namespace fastbird;

class SceneObject::Impl{
public:
	std::string mName;
	std::vector<SceneWeakPtr> mScenes;
	int mObjFlag;
	int mGameType;
	unsigned mGameId;
	void* mGamePtr;

	//---------------------------------------------------------------------------
	Impl()
		: mObjFlag(0)
		, mGameType(-1)
		, mGameId(-1)
		, mGamePtr(0)		
	{
	}

	Impl(const Impl& other)
		: mName(other.mName)
		, mScenes(other.mScenes)
		, mObjFlag(other.mObjFlag)
		, mGameType(-1)
		, mGameId(-1)
		, mGamePtr(0)		
	{
	}

	void SetName(const char* name){
		if (name)
			mName = name;
	}

	const char* GetName() const{
		return mName.c_str();
	}

	void OnAttachedToScene(ScenePtr pScene)
	{
		if (pScene == 0)
			return;

		if (!ValueExistsInVector(mScenes, pScene)) {
			mScenes.push_back(pScene);
		}
	}

	void OnDetachedFromScene(ScenePtr pScene)
	{
		DeleteValuesInVector(mScenes, pScene);
	}

	bool IsAttached(ScenePtr pScene) const {
		if (!pScene)
		{
			return !mScenes.empty();
		}
		return ValueExistsInVector(mScenes, pScene);
	}

	//-------------------------------------------------------------------
	// Object Flags
	//-------------------------------------------------------------------
	void SetObjFlag(unsigned flag) {
		mObjFlag = flag;
	}

	unsigned GetObjFlag() const {
		return mObjFlag;
	}

	void ModifyObjFlag(unsigned flag, bool enable) {
		if (enable)
		{
			mObjFlag |= flag;
		}
		else
		{
			mObjFlag = mObjFlag  & ~flag;
		}
	}

	bool HasObjFlag(unsigned flag) {
		return (mObjFlag & flag) != 0;
	}

	void SetVisible(bool visible) {
		ModifyObjFlag(SceneObjectFlag::Hide, !visible);
	}

	bool GetVisible() const {
		return mObjFlag & SceneObjectFlag::Hide ? false : true;
	}

	//-------------------------------------------------------------------
	// Debugging features
	//-------------------------------------------------------------------
	void SetGameType(int type){
		mGameType = type;
	}

	int GetGameType() const { 
		return mGameType;
	}

	void SetGameId(unsigned id) {
		mGameId = id;
	}

	unsigned GetGameId() const { 
		return mGameId;
	}

	void SetGamePtr(void* ptr) {
		mGamePtr = ptr;
	}

	void* GetGamePtr() const { 
		return mGamePtr;
	}
};

//---------------------------------------------------------------------------
SceneObject::SceneObject()
: mImpl(new Impl)
{
}

SceneObject::SceneObject(const SceneObject& other)
	: mImpl(new Impl(*other.mImpl))
{
}

SceneObject::~SceneObject()
{
}

void SceneObject::SetName(const char* name){
	mImpl->SetName(name);
}

const char* SceneObject::GetName() const{
	return mImpl->GetName();
}

void SceneObject::OnAttachedToScene(ScenePtr pScene){
	mImpl->OnAttachedToScene(pScene);
}

void SceneObject::OnDetachedFromScene(ScenePtr pScene){
	mImpl->OnDetachedFromScene(pScene);
}

bool SceneObject::IsAttached(ScenePtr pScene) const{
	return mImpl->IsAttached(pScene);
}

//-------------------------------------------------------------------
// Object Flags
//-------------------------------------------------------------------
void SceneObject::SetObjFlag(unsigned flag){
	mImpl->SetObjFlag(flag);
}

unsigned SceneObject::GetObjFlag() const{
	return mImpl->GetObjFlag();
}

void SceneObject::ModifyObjFlag(unsigned flag, bool enable){
	mImpl->ModifyObjFlag(flag, enable);
}

bool SceneObject::HasObjFlag(unsigned flag){
	return mImpl->HasObjFlag(flag);
}

void SceneObject::SetVisible(bool visible){
	mImpl->SetVisible(visible);
}

bool SceneObject::GetVisible() const{
	return mImpl->GetVisible();

}

//-------------------------------------------------------------------
// Debugging features
//-------------------------------------------------------------------
void SceneObject::SetGameType(int type){
	mImpl->SetGameType(type);
}

int SceneObject::GetGameType() const{
	return mImpl->GetGameType();
}

void SceneObject::SetGameId(unsigned id){
	mImpl->SetGameId(id);
}

unsigned SceneObject::GetGameId() const{
	return mImpl->GetGameId();
}

void SceneObject::SetGamePtr(void* ptr){
	mImpl->SetGamePtr(ptr);
}

void* SceneObject::GetGamePtr() const{
	return mImpl->GetGamePtr();
}