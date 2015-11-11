#pragma once
#include "KeyboardEvent.h"
#include "MouseEvent.h"
#include "FBTimerLib/Timer.h"
namespace fastbird{
	class InputConsumer;
	DECLARE_SMART_PTR(IMouse);
	DECLARE_SMART_PTR(IKeyboard);
	class FB_DLL_PUBLIC InputManager{
		DECLARE_PIMPL(InputManager);
		InputManager();
		~InputManager();
		static InputManager* sInputManager;
	public:

		static InputManager* CreateInputManager();
		static InputManager* GetInstance();
		static void DeleteInputManager();

		//-------------------------------------------------------------------
		// Manager
		//-------------------------------------------------------------------
		void PrepareKeyboard(); /// for default keyboard
		void PrepareMouse(); /// for default mouse
		void SetKeyboard(IKeyboardPtr keyboard);
		void SetMouse(IMousePtr mouse);

		void RegisterInputConsumer(int priority, InputConsumer* consumer);
		void UnregisterInputConsumer(int priority, InputConsumer* consumer);
		void Update();		
		// bit field
		enum Device{
			DeviceKeyboard=0x01,
			DeviceMouse = 0x02,
			AllMask = 0x03,
			DeviceNum = 2,
		};		
		void Invalidate(Device type, bool includeButtonClicks = false);
		void InvalidTemporary(Device type, bool invalidate);
		bool IsValid(Device type) const;
		void EndFrame(Timer::TIME_PRECISION gameTimeInSecond);

		//-------------------------------------------------------------------
		// Common
		//-------------------------------------------------------------------
		void OnSetFocus(HWindow hWnd);
		void OnKillFocus();
		void AddHwndInterested(HWindow wnd);

		//-------------------------------------------------------------------
		// Keyboard
		//-------------------------------------------------------------------
		void PushKeyEvent(HWindow hWnd, const KeyboardEvent& keyboardEvent);
		void PushChar(HWindow hWnd, unsigned keycode, Timer::TIME_PRECISION gameTimeInSec);
		bool IsKeyDown(unsigned short keycode) const;
		bool IsKeyPressed(unsigned short keycode) const;
		bool IsKeyUp(unsigned short keycode) const;
		unsigned GetChar();
		void PopChar();		
		void ClearBuffer();

		//-------------------------------------------------------------------
		// Mouse
		//-------------------------------------------------------------------
		void PushMouseEvent(HWindow handle, const MouseEvent& mouseEvent, Timer::TIME_PRECISION);
		void GetHDDeltaXY(long &x, long &y) const;
		void GetDeltaXY(long &x, long &y) const;
		CoordinatesI GetDeltaXY() const;
		void GetMousePos(long &x, long &y) const;
		CoordinatesI GetMousePos() const;
		void GetMousePrevPos(long &x, long &y) const;
		// normalized pos(0.0~1.0)
		void GetMouseNPos(Real &x, Real &y) const;
		CoordinatesR GetMouseNPos() const;
		bool IsLButtonDownPrev() const;
		bool IsLButtonDown(Real* time = 0) const;
		bool IsLButtonClicked() const;
		bool IsLButtonDoubleClicked() const;
		bool IsLButtonPressed() const;
		bool IsRButtonDown(Real* time = 0) const;
		bool IsRButtonDownPrev() const;
		bool IsRButtonClicked() const;
		bool IsRButtonPressed() const;
		bool IsMButtonDown() const;
		bool IsMouseMoved() const;
		void GetDragStart(long &x, long &y) const;
		CoordinatesI GetDragStartedPos() const;
		bool IsDragStartIn(int left, int top, int right, int bottom) const;
		bool IsDragStarted(int& outX, int& outY) const;
		bool IsDragEnded() const;
		void PopDragEvent();
		bool IsRDragStarted(int& outX, int& outY) const;
		bool IsRDragEnded(int& outX, int& outY) const;
		void PopRDragEvent();
		long GetWheel() const;
		void PopWheel();
		void ClearWheel();
		void ClearButton();
		unsigned long GetNumLinesWheelScroll() const;
		void LockMousePos(bool lock, void* key);
		void NoClickOnce();		
		bool IsMouseIn(int left, int top, int right, int bottom);
		void MouseToCenter();
		void SetMousePosition(int x, int y);
		void ClearRightDown();
		Real GetLButtonDownTime() const;		
		Real GetSensitivity() const;
		void SetSensitivity(Real sens);
		Real GetWheelSensitivity() const;
		void SetWheelSensitivity(Real sens);
	};
}