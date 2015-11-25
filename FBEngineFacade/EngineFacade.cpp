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
#include "EngineFacade.h"
#include "FBRenderer/RenderTarget.h"
#include "FBSceneManager/SceneManager.h"
#include "FBSceneManager/Scene.h"
#include "FBSceneObjectFactory/SceneObjectFactory.h"
using namespace fastbird;
class EngineFacade::Impl{
public:
	static const int MainWindowId = 1;
	RendererPtr mRenderer;
	lua_State* mL;
	std::map<HWindowId, HWindow> mWindowById;
	std::map<HWindow, HWindowId> mWindowIdByWindow;
	HWindowId mNextWindowId;
	SceneManagerPtr mSceneManager;
	ScenePtr mMainScene;
	SceneObjectFactoryPtr mSceneObjectFactory;

	//---------------------------------------------------------------------------
	Impl()
		:mL(0)
		, mNextWindowId(MainWindowId)
	{
		mSceneManager = SceneManager::Create();
		if (!mSceneManager){
			Logger::Log(FB_ERROR_LOG_ARG, "Cannot create SceneManager.");
		}
		else{
			mMainScene = mSceneManager->CreateScene("MainScene");
			if (!mMainScene){
				Logger::Log(FB_ERROR_LOG_ARG, "Cannot create the main scene.");
			}
		}

		mSceneObjectFactory = SceneObjectFactory::Create();
		if (!mSceneObjectFactory){
			Logger::Log(FB_ERROR_LOG_ARG, "SceneObjectFactory is not initialized.");
		}
	}

	~Impl(){
		LuaUtils::CloseLuaState(mL);
	}

	bool InitRenderer(const char* pluginName){
		mL = LuaUtils::OpenLuaState();
		mRenderer = Renderer::Create(pluginName, mL);
		return mRenderer != 0;
	}

	bool InitCanvas(HWindow window, int width, int height){
		if (!mRenderer){
			Logger::Log(FB_ERROR_LOG_ARG, "Renderer is not initialized.");
			return false;
		}
		auto windowId = mNextWindowId;		
		bool sucess = mRenderer->InitCanvas(windowId, window, width, height);
		if (!sucess){
			return INVALID_HWND_ID;
		}
		else{
			++mNextWindowId;			
			mWindowById[windowId] = window;
			mWindowIdByWindow[window] = windowId;
			if (windowId == MainWindowId){
				auto rt = mRenderer->GetRenderTarget(windowId);
				if (!rt){
					Logger::Log(FB_ERROR_LOG_ARG, "Main RenderTarget is not initialized.");
				}
				else{
					rt->RegisterScene(mMainScene);
				}
			}

			return true;
		}
	}

	ScenePtr GetMainScene() const{
		return mMainScene;
	}

	void Update(TIME_PRECISION dt){
		mRenderer->Update(dt);
		mSceneManager->Update(dt);
		mSceneObjectFactory->Update(dt);
	}

	void Render(){
		mRenderer->Render();
	}
};

//---------------------------------------------------------------------------
FB_IMPLEMENT_STATIC_CREATE(EngineFacade);
EngineFacade::EngineFacade(){

}
EngineFacade::~EngineFacade(){

}

bool EngineFacade::InitRenderer(const char* pluginName) {
	return mImpl->InitRenderer(pluginName);
}

bool EngineFacade::InitCanvas(HWindow window, int width, int height) {
	return mImpl->InitCanvas(window, width, height);
}

ScenePtr EngineFacade::GetMainScene() const {
	return mImpl->GetMainScene();
}

void EngineFacade::Update(TIME_PRECISION dt) {
	mImpl->Update(dt);
}

void EngineFacade::Render() {
	mImpl->Render();
}

