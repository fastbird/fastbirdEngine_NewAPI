#pragma once
#include "FBCommonHeaders/platform.h"
#include "IInputInjector.h"

namespace fastbird{
	class FB_DLL_HIDDEN InputInjector{
		DECLARE_PIMPL_NON_COPYABLE(InputInjector);

	public:
		InputInjector();
		~InputInjector();

		//-------------------------------------------------------------------
		// IInputInjector
		//-------------------------------------------------------------------
		bool IsValid(FBInputDevice::Enum type) const;
		void Invalidate(FBInputDevice::Enum type) const;
		void InvalidTemporary(FBInputDevice::Enum type, bool invalidate);
		
		//-------------------------------------------------------------------
		// Keyboard
		//-------------------------------------------------------------------
		bool IsKeyDown(unsigned short keycode) const;
		bool IsKeyPressed(unsigned short keycode) const;
		bool IsKeyUp(unsigned short keycode) const;
		unsigned GetChar();
		void PopChar();
		void ClearBuffer();

		//-------------------------------------------------------------------
		// Mouse
		//-------------------------------------------------------------------
		// Positions
		void GetHDDeltaXY(long &x, long &y) const;
		void GetDeltaXY(long &x, long &y) const;
		Vec2ITuple GetDeltaXY() const;
		void GetMousePos(long &x, long &y) const;
		Vec2ITuple GetMousePos() const;
		void GetMousePrevPos(long &x, long &y) const;
		void GetMouseNPos(Real &x, Real &y) const; // normalized pos(0.0~1.0)
		Vec2Tuple GetMouseNPos() const;
		bool IsMouseMoved() const;
		void LockMousePos(bool lock, void* key);
		bool IsMouseIn(int left, int top, int right, int bottom);
		Real GetSensitivity() const;

		// Dragging
		void GetDragStart(long &x, long &y) const;
		Vec2ITuple GetDragStartedPos() const;
		bool IsDragStartIn(int left, int top, int right, int bottom) const;
		bool IsDragStarted(int& outX, int& outY) const;
		bool IsDragEnded() const;
		void PopDragEvent();
		bool IsRDragStarted(int& outX, int& outY) const;
		bool IsRDragEnded(int& outX, int& outY) const;
		void PopRDragEvent();

		// Buttons
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
		void ClearButton();
		void NoClickOnce();
		void ClearRightDown();
		Real GetLButtonDownTime() const;

		// Wheel		
		long GetWheel() const;
		void PopWheel();
		void ClearWheel();
		Real GetWheelSensitivity() const;
		unsigned long GetNumLinesWheelScroll() const;

	private:
		friend class InputManager;
		void SetKeyboard(IKeyboardPtr keyboard);
		void SetMouse(IMousePtr mouse);
	};
}