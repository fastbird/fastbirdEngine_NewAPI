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
#include "SceneManager.h"
#include "Scene.h"
#include "FBTimer/Timer.h"
using namespace fastbird;

//---------------------------------------------------------------------------
class SceneManager::Impl{
public:
	SceneManagerWeakPtr mSelf;
	std::map<std::string, SceneWeakPtr> mScenes;
	SceneWeakPtr mMainScene;

	//---------------------------------------------------------------------------

	ScenePtr CreateScene(const char* name){
		if (!ValidCStringLength(name)){
			Logger::Log(FB_ERROR_LOG_ARG, "invalid arg");
			return 0;
		}

		auto it = mScenes.find(name);
		if (it != mScenes.end()){
			auto scene = it->second.lock();
			if (scene)
				return scene;
		}
		auto scene = Scene::Create(name);
		mScenes[name] = scene;
		if (mMainScene.expired())
			mMainScene = scene;
		return scene;
	}

	ScenePtr GetMainScene() const{
		return mMainScene.lock();
	}
};

Timer* fastbird::gpTimer = 0;
//---------------------------------------------------------------------------
SceneManagerWeakPtr sSceneManager;
SceneManagerPtr SceneManager::CreateSceneManager(){
	if (sSceneManager.expired()){
		auto sceneManager = SceneManagerPtr(new SceneManager, [](SceneManager* obj){ delete obj; });
		sceneManager->mImpl->mSelf = sceneManager;
		sSceneManager = sceneManager;
		gpTimer = Timer::GetMainTimer().get();
	}
	return sSceneManager.lock();
}

SceneManager& SceneManager::GetInstance(){
	return *sSceneManager.lock();
}

SceneManager::SceneManager()
	: mImpl(new Impl){

}

ScenePtr SceneManager::CreateScene(const char* name){
	return 	mImpl->CreateScene(name);
}

ScenePtr SceneManager::GetMainScene() const{
	return mImpl->GetMainScene();
}