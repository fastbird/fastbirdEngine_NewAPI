/**
\defgroup FBInputManager
Handles use inputs such as keyboard and mouse.

Required libraries: \b FBDebugLib \b FBTimerLib
*/
#pragma once
#include <memory>
#include "FBCommonHeaders/Types.h"
#include "FBTimerLib/Timer.h"
namespace fastbird
{
	class IInputDevice;
	typedef std::shared_ptr<IInputDevice> IInputDevicePtr;
	typedef std::weak_ptr<IInputDevice> IInputDeviceWeakPtr;
	class IInputDevice
	{
	public:
		virtual void EndFrame(Timer::TIME_PRECISION gameTimeInSecond) = 0;
		virtual bool IsValid() const = 0;
		virtual void Invalidate(bool buttonClicked = false) = 0;
		virtual void InvalidTemporary(bool invalidate) = 0;
	};
}