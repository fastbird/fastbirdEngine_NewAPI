#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	FB_DECLARE_SMART_PTR(EngineOptions);
	class FB_DLL_ENGINEFACADE EngineOptions{
		EngineOptions();
		~EngineOptions();

	public:
		static EngineOptionsPtr Create();

		float WheelSens;
		float MouseSens;
		int e_profile;
		int e_NoMeshLoad;	
	};
}