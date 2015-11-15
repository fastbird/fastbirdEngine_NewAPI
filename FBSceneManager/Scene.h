#pragma once
#include "FBCommonHeaders/Types.h"
#include "SceneManager.h"
#include "ISceneObserver.h"
#include "FBRenderer/RenderParam.h"
#include "FBCommonHeaders/Observable.h"
namespace fastbird{
	DECLARE_SMART_PTR(DirectionalLight);
	DECLARE_SMART_PTR(Scene);
	class FB_DLL_PUBLIC Scene : public Observable<ISceneObserverPtr>{
		DECLARE_PIMPL_NON_COPYABLE(Scene);		
		Scene();		
		
	public:
		static ScenePtr Create(const char* name);
		~Scene();
		void PreRender(const RenderParam& renderParam, RenderParamOut* renderParamOut);
		void Render(const RenderParam& renderParam, RenderParamOut* renderParamOut);
		void PostRender(const RenderParam& renderParam, RenderParamOut* renderParamOut);

		DirectionalLightPtr GetLight(unsigned idx);
	};
}