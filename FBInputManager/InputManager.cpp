#include "stdafx.h"
#include "InputManager.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "IInputConsumer.h"
#include "IInputInjector.h"
#include "FBCommonHeaders/Helpers.h"

using namespace fastbird;
InputManager* InputManager::sInputManager = 0;
InputManager* InputManager::CreateInputManager(){
	if (sInputManager)
		return sInputManager;
	sInputManager = new InputManager;
	return sInputManager;
}

InputManager* InputManager::GetInstance(){
	return sInputManager;
}
void InputManager::DeleteInputManager(){
	delete sInputManager;
	sInputManager = 0;
}

class InputManager::Impl{
public:
	IKeyboardPtr mKeyboard;
	IMousePtr mMouse;
	IInputInjectorPtr mInjector;
	std::map<int, std::vector<IInputConsumer*>> mConsumers;
	int mValid;

	Impl()
		:mValid(0){

		for (int i = 0; i < FBInputDevice::DeviceNum; ++i){
			mValid += 1 << i;
		}
	}

	void RegisterInputConsumer(IInputConsumer* consumer, int priority){
		auto& consumers = mConsumers[priority];
		if (!ValueExistsInVector(consumers, consumer)){
			consumers.push_back(consumer);			
		}
	}
	void UnregisterInputConsumer(IInputConsumer* consumer, int priority){
		auto& consumers = mConsumers[priority];
		DeleteValuesInVector(consumers, consumer);
	}

	void Update(InputManager* inputManager){
		mValid = 0;
		for (int i = 0; i < FBInputDevice::DeviceNum; ++i){
			mValid += 1 << i;
		}
		for (const auto& it : mConsumers){
			for (const auto& consumer : it.second){
				consumer->ConsumeInput(mInjector);
				if (!(mValid & FBInputDevice::AllMask))
					return;
			}
		}
	}

	void Invalidate(FBInputDevice::Enum type, bool includeButtonClicks){
		if (mValid & type)
			mValid -= type;
		switch (type){
		case FBInputDevice::DeviceKeyboard:
			if (mKeyboard){
				mKeyboard->Invalidate(includeButtonClicks);
			}
			break;
		case FBInputDevice::DeviceMouse:
			if (mMouse){
				mMouse->Invalidate(includeButtonClicks);
			}
			break;
		default:
			assert(0 && "Not implemented");
		}
	}

	void InvalidTemporary(FBInputDevice::Enum type, bool invalidate){
		switch (type){
		case FBInputDevice::DeviceKeyboard:
			if (mKeyboard){
				mKeyboard->InvalidTemporary(invalidate);
			}
			break;
		case FBInputDevice::DeviceMouse:
			if (mMouse){
				mMouse->InvalidTemporary(invalidate);
			}
			break;
		default:
			assert(0 && "Not implemented");
		}
	}

	bool IsValid(FBInputDevice::Enum type) const{
		switch (type){
		case FBInputDevice::DeviceKeyboard:
			if (mKeyboard){
				return mKeyboard->IsValid();
			}
			break;
		case FBInputDevice::DeviceMouse:
			if (mMouse){
				return mMouse->IsValid();
			}
			break;
		default:
			assert(0 && "Not implemented");
		}
		return false;
	}

	void EndFrame(Timer::TIME_PRECISION gameTimeInSecond){
		if (mKeyboard){
			mKeyboard->EndFrame(gameTimeInSecond);
		}
		if (mMouse){
			mMouse->EndFrame(gameTimeInSecond);
		}
	}

	void SetInputInjector(IInputInjectorPtr injector){
		mInjector = injector;
		mInjector->SetKeyboard(mKeyboard);
		mInjector->SetMouse(mMouse);
	}
};

InputManager::InputManager()
: mImpl(new Impl)
{
	
}

InputManager::~InputManager(){	
}
//-------------------------------------------------------------------
// Manager
//-------------------------------------------------------------------
void InputManager::PrepareKeyboard(){
	mImpl->mKeyboard = IKeyboardPtr(new Keyboard);
}

void InputManager::PrepareMouse(){
	mImpl->mMouse = IMousePtr(new Mouse);
}

void InputManager::SetKeyboard(IKeyboardPtr keyboard){
	mImpl->mKeyboard = keyboard;
}

void InputManager::SetMouse(IMousePtr mouse){
	mImpl->mMouse = mouse;
}

void InputManager::RegisterInputConsumer(IInputConsumer* consumer, int priority){
	mImpl->RegisterInputConsumer(consumer, priority);
}

void InputManager::UnregisterInputConsumer(IInputConsumer* consumer, int priority){
	mImpl->UnregisterInputConsumer(consumer, priority);
}

void InputManager::Update(){
	mImpl->Update(this);
}

void InputManager::Invalidate(FBInputDevice::Enum type, bool includeButtonClicks){
	mImpl->Invalidate(type, includeButtonClicks);
}

void InputManager::InvalidTemporary(FBInputDevice::Enum type, bool invalidate){
	mImpl->InvalidTemporary(type, invalidate);
}

bool InputManager::IsValid(FBInputDevice::Enum type) const{
	return mImpl->IsValid(type);
}

void InputManager::EndFrame(Timer::TIME_PRECISION gameTimeInSecond){
	mImpl->EndFrame(gameTimeInSecond);
}

//-------------------------------------------------------------------
// Common
//-------------------------------------------------------------------
void InputManager::OnSetFocus(HWindow hWnd){
	if (mImpl->mMouse){
		mImpl->mMouse->OnSetFocus(hWnd);
	}
}

void InputManager::OnKillFocus(){
	if (mImpl->mKeyboard){
		mImpl->mKeyboard->OnKillFocus();
	}
	if (mImpl->mMouse){
		mImpl->mMouse->OnKillFocus();
	}
}

void InputManager::AddHwndInterested(HWindow wnd){
	if (mImpl->mMouse){
		mImpl->mMouse->AddHwndInterested(wnd);
	}
}

void InputManager::SetInputInjector(IInputInjectorPtr injector){
	mImpl->SetInputInjector(injector);
}

//-------------------------------------------------------------------
// Keyboard
//-------------------------------------------------------------------
void InputManager::PushKeyEvent(HWindow hwnd, const KeyboardEvent& keyboardEvent){
	if (mImpl->mKeyboard){
		mImpl->mKeyboard->PushEvent(hwnd, keyboardEvent);
	}
}

void InputManager::PushChar(HWindow hwnd, unsigned keycode, Timer::TIME_PRECISION gameTimeInSec){
	if (mImpl->mKeyboard){
		mImpl->mKeyboard->PushChar(hwnd, keycode, gameTimeInSec);
	}
}

//-------------------------------------------------------------------
// Mouse
//-------------------------------------------------------------------
void InputManager::PushMouseEvent(HWindow handle, const MouseEvent& mouseEvent, Timer::TIME_PRECISION timeInSec){
	if (mImpl->mMouse){
		mImpl->mMouse->PushEvent(handle, mouseEvent, timeInSec);
	}
}

void InputManager::MouseToCenter(){
	if (mImpl->mMouse){
		mImpl->mMouse->CursorToCenter();
	}
}

void InputManager::SetMousePosition(int x, int y){
	if (mImpl->mMouse){
		mImpl->mMouse->SetCursorPosition(x, y);
	}
}

Real InputManager::GetSensitivity() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->GetSensitivity();
	}
	return 0.;
}

void InputManager::SetSensitivity(Real sens){
	if (mImpl->mMouse){
		mImpl->mMouse->SetSensitivity(sens);
	}
}

Real InputManager::GetWheelSensitivity() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->GetWheelSensitivity();
	}
	return 0;
}

void InputManager::SetWheelSensitivity(Real sens){
	if (mImpl->mMouse){
		mImpl->mMouse->SetWheelSensitivity(sens);
	}
}