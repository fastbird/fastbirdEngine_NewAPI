#pragma once
#ifndef fastbird_Mouse_header_included_
#define fastbird_Mouse_header_included_

#include "IMouse.h"
#include "FBRenderer/IRenderTargetObserver.h"

namespace fastbird
{
	class Mouse : public IMouse, public IRenderTargetObserver
	{
		class MouseImpl;
		MouseImpl* mImpl;
		Mouse(const Mouse&) = delete;
		Mouse& Mouse::operator= (const Mouse&) = delete;

	public:
		Mouse();
		~Mouse();

	public:

		//-------------------------------------------------------------------
		// IInputDevice
		//-------------------------------------------------------------------
		virtual void EndFrame(Timer::TIME_PRECISION gameTimeInSec);
		virtual bool IsValid() const;
		virtual void Invalidate(bool buttonClicked = false);
		virtual void InvalidTemporary(bool invalidate);
		
		//-------------------------------------------------------------------
		// IMouse
		//-------------------------------------------------------------------
		virtual void PushEvent(HWindow handle, const MouseEvent& mouseEvent, Timer::TIME_PRECISION);
		virtual void GetHDDeltaXY(long &x, long &y) const;
		virtual void GetDeltaXY(long &x, long &y) const;
		virtual CoordinatesI GetDeltaXY() const;
		virtual void GetPos(long &x, long &y) const;
		virtual CoordinatesI GetPos() const;
		virtual void GetPrevPos(long &x, long &y) const;
		virtual void GetNPos(Real &x, Real &y) const;
		virtual CoordinatesR GetNPos() const;		
		virtual bool IsLButtonDownPrev() const;
		virtual bool IsLButtonDown(Real* time = 0) const;
		virtual bool IsLButtonClicked() const;
		virtual bool IsLButtonDoubleClicked() const;
		virtual bool IsLButtonPressed() const;
		virtual bool IsRButtonDown(Real* time = 0) const;
		virtual bool IsRButtonDownPrev() const;
		virtual bool IsRButtonClicked() const;
		virtual bool IsRButtonPressed() const;
		virtual bool IsMButtonDown() const;
		virtual bool IsMoved() const;

		virtual void GetDragStart(long &x, long &y) const;
		virtual CoordinatesI GetDragStartedPos() const;
		virtual bool IsDragStartIn(int left, int top, int right, int bottom) const;
		virtual bool IsDragStarted(int& outX, int& outY) const;
		virtual bool IsDragEnded() const;
		virtual void PopDragEvent();
		virtual bool IsRDragStarted(int& outX, int& outY) const;
		virtual bool IsRDragEnded(int& outX, int& outY) const;
		virtual void PopRDragEvent();
		
		virtual long GetWheel() const;
		virtual void PopWheel();
		virtual void ClearWheel();
		virtual void ClearButton();
		virtual unsigned long GetNumLinesWheelScroll() const;

		virtual void LockMousePos(bool lock, void* key);
		virtual void NoClickOnce();
		virtual void OnKillFocus();
		virtual void OnSetFocus(HWindow hWnd);
		virtual bool IsIn(int left, int top, int right, int bottom);
		
		virtual void CursorToCenter();
		virtual void SetCursorPosition(int x, int y);

		virtual void ClearRightDown();
		virtual Real GetLButtonDownTime() const;
		virtual void AddHwndInterested(HWindow wnd);
		virtual Real GetSensitivity() const;
		virtual void SetSensitivity(Real sens);
		virtual Real GetWheelSensitivity() const;
		virtual void SetWheelSensitivity(Real sens);

		//-------------------------------------------------------------------
		// IRenderTargetObserver
		//-------------------------------------------------------------------
		virtual void OnRenderTargetSizeChanged(int x, int y, HWindow associatedWindow);
	};
}

#endif