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
#include "SkySphere.h"

using namespace fastbird;

SceneManager* SceneManager::sSceneManager = 0;
SceneManager* SceneManager::CreateSceneManager(){
	if (sSceneManager)
		return sSceneManager;
	sSceneManager = new SceneManager;
	return sSceneManager;
}
SceneManager& SceneManager::GetInstance(){
	return *sSceneManager;
}
void SceneManager::DeleteSceneManager(){
	delete sSceneManager;
	sSceneManager = 0;
}

//---------------------------------------------------------------------------
class SceneManager::Impl{
public:
	std::map<std::string, SceneWeakPtr> mScenes;

	Impl(){
		SkySphere::CreateSharedEnvRT();
	}
	~Impl(){
		SkySphere::DeleteSharedEnvRT();
	}
};

//---------------------------------------------------------------------------
ScenePtr SceneManager::CreateScene(const char* name){
	if (!name || !strlen(name)){
		Logger::Log(FB_ERROR_LOG_ARG, "invalid arg");
		return 0;
	}
	auto it = mImpl->mScenes.find(name);
	if (it != mImpl->mScenes.end()){
		auto scene = it->second.lock();
		if (scene)
			return scene;
	}
	auto scene = Scene::Create(name);
	mImpl->mScenes[name] = scene;
	return scene;
}
