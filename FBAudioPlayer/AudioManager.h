#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	DECLARE_SMART_PTR(AudioManager);
	class FB_DLL_PUBLIC AudioManager{
		DECLARE_PIMPL_NON_COPYABLE(AudioManager);		
		AudioManager();
	public:
		static AudioManagerPtr Create();
		bool Init();
		void Deinit();
	};
}