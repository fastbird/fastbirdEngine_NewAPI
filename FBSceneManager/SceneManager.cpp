#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"

using namespace fastbird;

SceneManager* SceneManager::sSceneManager = 0;
SceneManager* SceneManager::CreateSceneManager(){
	if (sSceneManager)
		return sSceneManager;
	sSceneManager = new SceneManager;
	return sSceneManager;
}
SceneManager* SceneManager::GetSceneManager(){
	return sSceneManager;
}
void SceneManager::DeleteSceneManager(){
	delete sSceneManager;
	sSceneManager = 0;
}

//---------------------------------------------------------------------------
class SceneManager::Impl{
public:
	std::map<std::string, SceneWeakPtr> mScenes;
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
