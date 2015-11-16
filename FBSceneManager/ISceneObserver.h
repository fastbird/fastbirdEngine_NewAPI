#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird
{
	DECLARE_SMART_PTR(Scene);
	class FB_DLL_PUBLIC ISceneObserver
	{
	public:
		enum{
			Timing
		};
		virtual void OnAfterMakeVisibleSet(Scene* scene){}
		virtual void OnBeforeRenderingOpaques(Scene* scene){}
		virtual void OnBeforeRenderingTransparents(Scene* scene){}
	};
}