#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	DECLARE_SMART_PTR(Scene);
	class FB_DLL_PUBLIC SceneManager{
		DECLARE_PIMPL_NON_COPYABLE(SceneManager);
		SceneManager();
		~SceneManager();
		static SceneManager* sSceneManager;
	public:
		static SceneManager* CreateSceneManager();
		static SceneManager* GetSceneManager();
		static void DeleteSceneManager();

		ScenePtr CreateScene(const char* name);
	};
}