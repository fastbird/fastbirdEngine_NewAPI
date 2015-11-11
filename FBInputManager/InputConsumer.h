#pragma once
#include "FBCommonHeaders/platform.h"
#include "FBCommonHeaders/Types.h"
#include <memory>
namespace fastbird{
	DECLARE_SMART_PTR(InputManager);
	
	class FB_DLL_PUBLIC InputConsumer{
		DECLARE_PIMPL(InputConsumer);
		
	public:
		enum Priority{
			Priority22_Console = 11, // handled first
			Priority55_UI = 33,			
			Priority77_INTERACTION = 55,
			Priority99_CAMERA = 77,	
		};

		InputConsumer();
		virtual ~InputConsumer();
		virtual void OnInputConsumerRegistered(InputManager* inputManager, Priority level) final;

		virtual void ConsumeInput(InputManager* inputManager) = 0;
	};
}