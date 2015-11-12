#pragma once
#include "FBCommonHeaders/platform.h"
#include "FBCommonHeaders/Types.h"
#include "FBInputDevice.h"
#include <memory>
namespace fastbird{
	DECLARE_SMART_PTR(IInputInjector);
	DECLARE_SMART_PTR(InputConsumer);
	/** When ever you have a class that need to get input device
	information inherit this class.
	\ingroup FBInputManager
	*/
	class FB_DLL_PUBLIC IInputConsumer{		
	public:
		/**Default priority
		*/
		enum Priority{
			Priority11_Console = 11, // handled first
			Priority33_UI = 33,			
			Priority55_INTERACTION = 55,
			Priority77_CAMERA = 77,	
		};		
		virtual ~IInputConsumer(){}

		/** Implement this function in your derived class to get user input.
		*/
		virtual void ConsumeInput(IInputInjectorPtr injector) = 0;
	};
}