#pragma once
#ifndef _fastbird_Keyboard_header_included_
#define _fastbird_Keyboard_header_included_

#include "IKeyboard.h"
#include <queue>
namespace fastbird
{
	class Keyboard : public IKeyboard
	{
		class KeyboardImpl;
		KeyboardImpl* mImpl;
		Keyboard(const Keyboard&) = delete;
		Keyboard& Keyboard::operator= (const Keyboard&) = delete;

	public:	
		Keyboard();
		~Keyboard();
		//-------------------------------------------------------------------
		// IInputDevice
		//-------------------------------------------------------------------
		virtual void EndFrame(Timer::TIME_PRECISION gameTimeInSecond);
		virtual bool IsValid() const;
		virtual void Invalidate(bool buttonClicked = false);
		virtual void InvalidTemporary(bool invalidate);

		virtual void PushEvent(HWindow hWnd, const KeyboardEvent& keyboardEvent);
		virtual void PushChar(HWindow hWnd, unsigned keycode, Timer::TIME_PRECISION gameTimeInSec);
		virtual bool IsKeyDown(unsigned short keycode) const;
		virtual bool IsKeyPressed(unsigned short keycode) const;
		virtual bool IsKeyUp(unsigned short keycode) const;
		virtual unsigned GetChar();
		virtual void PopChar();
		virtual void ClearBuffer();

		virtual void OnKillFocus();
		
	};
}

#endif //_fastbird_Keyboard_header_included_