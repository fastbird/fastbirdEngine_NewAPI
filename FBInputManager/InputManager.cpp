#include "stdafx.h"
#include "InputManager.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "InputConsumer.h"
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

class InputManager::InputManagerImpl{
public:
	IKeyboardPtr mKeyboard;
	IMousePtr mMouse;
	std::map<int, std::vector<InputConsumer*>> mConsumers;
	int mValid;

	InputManagerImpl()
		:mValid(0){

		for (int i = 0; i < DeviceNum; ++i){
			mValid += 1 << i;
		}
	}

	void RegisterInputConsumer(int priority, InputConsumer* consumer){
		auto& consumers = mConsumers[priority];
		if (!ValueExistsInVector(consumers, consumer)){
			consumers.push_back(consumer);
			consumer->OnInputConsumerRegistered(sInputManager, (InputConsumer::Priority)priority);
		}
	}
	void UnregisterInputConsumer(int priority, InputConsumer* consumer){
		auto& consumers = mConsumers[priority];
		DeleteValuesInVector(consumers, consumer);
	}

	void Update(InputManager* inputManager){
		mValid = 0;
		for (int i = 0; i < DeviceNum; ++i){
			mValid += 1 << i;
		}
		for (const auto& it : mConsumers){
			for (const auto& consumer : it.second){
				consumer->ConsumeInput(inputManager);
				if (!(mValid & AllMask))
					return;
			}
		}
	}

	void Invalidate(Device type, bool includeButtonClicks){
		if (mValid & type)
			mValid -= type;
		switch (type){
		case DeviceKeyboard:
			if (mKeyboard){
				mKeyboard->Invalidate(includeButtonClicks);
			}
			break;
		case DeviceMouse:
			if (mMouse){
				mMouse->Invalidate(includeButtonClicks);
			}
			break;
		default:
			assert(0 && "Not implemented");
		}
	}

	void InvalidTemporary(Device type, bool invalidate){
		switch (type){
		case DeviceKeyboard:
			if (mKeyboard){
				mKeyboard->InvalidTemporary(invalidate);
			}
			break;
		case DeviceMouse:
			if (mMouse){
				mMouse->InvalidTemporary(invalidate);
			}
			break;
		default:
			assert(0 && "Not implemented");
		}
	}

	bool IsValid(Device type) const{
		switch (type){
		case DeviceKeyboard:
			if (mKeyboard){
				return mKeyboard->IsValid();
			}
			break;
		case DeviceMouse:
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
};

InputManager::InputManager()
: mImpl(new InputManagerImpl)
{
	
}

InputManager::~InputManager(){
	delete mImpl;
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

void InputManager::RegisterInputConsumer(int priority, InputConsumer* consumer){
	mImpl->RegisterInputConsumer(priority, consumer);
}

void InputManager::UnregisterInputConsumer(int priority, InputConsumer* consumer){
	mImpl->UnregisterInputConsumer(priority, consumer);
}

void InputManager::Update(){
	mImpl->Update(this);
}

void InputManager::Invalidate(Device type, bool includeButtonClicks){
	mImpl->Invalidate(type, includeButtonClicks);
}

void InputManager::InvalidTemporary(Device type, bool invalidate){
	mImpl->InvalidTemporary(type, invalidate);
}

bool InputManager::IsValid(Device type) const{
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

bool InputManager::IsKeyDown(unsigned short keycode) const{
	if (mImpl->mKeyboard){
		return mImpl->mKeyboard->IsKeyDown(keycode);
	}
	return false;
}

bool InputManager::IsKeyPressed(unsigned short keycode) const{
	if (mImpl->mKeyboard){
		return mImpl->mKeyboard->IsKeyPressed(keycode);
	}
	return false;
}

bool InputManager::IsKeyUp(unsigned short keycode) const{
	if (mImpl->mKeyboard){
		return mImpl->mKeyboard->IsKeyUp(keycode);
	}
	return false;
}

unsigned InputManager::GetChar(){
	if (mImpl->mKeyboard){
		return mImpl->mKeyboard->GetChar();
	}
	return 0;
}

void InputManager::PopChar(){
	if (mImpl->mKeyboard){
		mImpl->mKeyboard->PopChar();
	}
}

void InputManager::ClearBuffer(){
	if (mImpl->mKeyboard){
		mImpl->mKeyboard->ClearBuffer();
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

void InputManager::GetHDDeltaXY(long &x, long &y) const{
	if (mImpl->mMouse){
		mImpl->mMouse->GetHDDeltaXY(x, y);
	}
}

void InputManager::GetDeltaXY(long &x, long &y) const{
	if (mImpl->mMouse){
		mImpl->mMouse->GetDeltaXY(x, y);
	}
}

CoordinatesI InputManager::GetDeltaXY() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->GetDeltaXY();
	}	
	return{ 0, 0 };
}

void InputManager::GetMousePos(long &x, long &y) const{
	if (mImpl->mMouse){
		mImpl->mMouse->GetPos(x, y);
	}
}

CoordinatesI InputManager::GetMousePos() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->GetPos();
	}
	return{ 0, 0 };
}

void InputManager::GetMousePrevPos(long &x, long &y) const{
	if (mImpl->mMouse){
		mImpl->mMouse->GetPrevPos(x, y);
	}
}

// normalized pos(0.0~1.0)
void InputManager::GetMouseNPos(Real &x, Real &y) const{
	if (mImpl->mMouse){
		mImpl->mMouse->GetNPos(x, y);
	}
}

CoordinatesR InputManager::GetMouseNPos() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->GetNPos();
	}
	return{ 0., 0. };
}

bool InputManager::IsLButtonDownPrev() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->IsLButtonDownPrev();
	}
	return false;
}

bool InputManager::IsLButtonDown(Real* time) const{
	if (mImpl->mMouse){
		return mImpl->mMouse->IsLButtonDown(time);
	}
	return false;
}

bool InputManager::IsLButtonClicked() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->IsLButtonClicked();
	}
	return false;
}

bool InputManager::IsLButtonDoubleClicked() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->IsLButtonDoubleClicked();
	}
	return false;
}

bool InputManager::IsLButtonPressed() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->IsLButtonPressed();
	}
	return false;
}

bool InputManager::IsRButtonDown(Real* time) const{
	if (mImpl->mMouse){
		return mImpl->mMouse->IsRButtonDown(time);
	}
	return false;
}

bool InputManager::IsRButtonDownPrev() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->IsRButtonDownPrev();
	}
	return false;
}

bool InputManager::IsRButtonClicked() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->IsRButtonClicked();
	}
	return false;
}

bool InputManager::IsRButtonPressed() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->IsRButtonPressed();
	}
	return false;
}

bool InputManager::IsMButtonDown() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->IsMButtonDown();
	}
	return false;
}

bool InputManager::IsMouseMoved() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->IsMoved();
	}
	return false;
}

void InputManager::GetDragStart(long &x, long &y) const{
	if (mImpl->mMouse){
		mImpl->mMouse->GetDragStart(x, y);
	}
}

CoordinatesI InputManager::GetDragStartedPos() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->GetDragStartedPos();
	}
	return{ 0, 0 };
}

bool InputManager::IsDragStartIn(int left, int top, int right, int bottom) const{
	if (mImpl->mMouse){
		return mImpl->mMouse->IsDragStartIn(left, top, right, bottom);
	}
	return false;
}

bool InputManager::IsDragStarted(int& outX, int& outY) const{
	if (mImpl->mMouse){
		return mImpl->mMouse->IsDragStarted(outX, outY);
	}
	return false;
}

bool InputManager::IsDragEnded() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->IsDragEnded();
	}
	return false;
}

void InputManager::PopDragEvent(){
	if (mImpl->mMouse){
		mImpl->mMouse->PopDragEvent();
	}	
}

bool InputManager::IsRDragStarted(int& outX, int& outY) const{
	if (mImpl->mMouse){
		return mImpl->mMouse->IsRDragStarted(outX, outY);
	}
	return false;
}

bool InputManager::IsRDragEnded(int& outX, int& outY) const{
	if (mImpl->mMouse){
		return mImpl->mMouse->IsRDragEnded(outX, outY);
	}
	return false;
}

void InputManager::PopRDragEvent(){
	if (mImpl->mMouse){
		mImpl->mMouse->PopRDragEvent();
	}
}

long InputManager::GetWheel() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->GetWheel();
	}
	return 0;
}

void InputManager::PopWheel(){
	if (mImpl->mMouse){
		mImpl->mMouse->PopWheel();
	}
}

void InputManager::ClearWheel(){
	if (mImpl->mMouse){
		mImpl->mMouse->ClearWheel();
	}
}

void InputManager::ClearButton(){
	if (mImpl->mMouse){
		mImpl->mMouse->ClearButton();
	}
}

unsigned long InputManager::GetNumLinesWheelScroll() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->GetNumLinesWheelScroll();
	}
	return 0;
}

void InputManager::LockMousePos(bool lock, void* key){
	if (mImpl->mMouse){
		mImpl->mMouse->LockMousePos(lock, key);
	}
}

void InputManager::NoClickOnce(){
	if (mImpl->mMouse){
		mImpl->mMouse->NoClickOnce();
	}
}

bool InputManager::IsMouseIn(int left, int top, int right, int bottom){
	if (mImpl->mMouse){
		return mImpl->mMouse->IsIn(left, top, right, bottom);
	}
	return false;
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

void InputManager::ClearRightDown(){
	if (mImpl->mMouse){
		mImpl->mMouse->ClearRightDown();
	}
}

Real InputManager::GetLButtonDownTime() const{
	if (mImpl->mMouse){
		return mImpl->mMouse->GetLButtonDownTime();
	}
	return 0.;
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