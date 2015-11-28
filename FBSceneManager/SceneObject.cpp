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
#include "SceneObject.h"
#include "FBCommonHeaders/Helpers.h"
#include "Scene.h"
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

bool SceneObject::IsAttached() const{
	return mImpl->IsAttached(0);
}

bool SceneObject::IsAttached(ScenePtr pScene) const{
	return mImpl->IsAttached(pScene);
}

bool SceneObject::DetachFromScene(){
	return DetachFromScene(false);
}
bool SceneObject::DetachFromScene(bool includingRtt){
	auto& scenes = mImpl->mScenes;
	unsigned limit = scenes.size();
	unsigned count = 0;
	bool detached = false;
	for (unsigned i = 0; i < scenes.size() && count < limit;)
	{
		auto scene = scenes[i].lock();
		if (scene){
			if (includingRtt)
			{
				detached = scene->DetachObject(this) || detached;
			}
			else
			{
				if (!scene->IsRttScene())
				{
					detached = scene->DetachObject(this) || detached;
				}
				else
				{
					++i;
				}
			}
		}
		++count;
	}
	return detached;
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