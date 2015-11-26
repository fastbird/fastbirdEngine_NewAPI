#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	FB_DECLARE_SMART_PTR(EngineOptions);
	class EngineOptions{		
		EngineOptions();
		~EngineOptions();

	public:
		static EngineOptionsPtr Create();

		float WheelSens;
		float MouseSens;
		int MoveEditParticle;
		int e_profile;
		int e_NoMeshLoad;	
	};
}