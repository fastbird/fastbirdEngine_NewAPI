#pragma once
#include "FBCommonHeaders/Types.h"
#include "SceneManager.h"
namespace fastbird{
	DECLARE_SMART_PTR(Scene);
	class FB_DLL_PUBLIC Scene{
		DECLARE_PIMPL(Scene);		
		friend ScenePtr (SceneManager::CreateScene)(const char* );
		Scene();
		~Scene();		
		static ScenePtr CreateScene();

	public:




		
	};
}