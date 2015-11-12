#include "stdafx.h"
#include "Keyboard.h"

namespace fastbird
{
template <class T>
static	void ClearWithSwap(T& m)
{
	T empty;
	std::swap(m, empty);
}

class Keyboard::KeyboardImpl{
public:
	bool mValid;
	bool mKeyDown[255];
	bool mKeyPressed[255]; // Down - up
	bool mKeyUp[255];
	std::queue<unsigned> mCurrentChar;
	Timer::TIME_PRECISION mLastPushKeyTime;
	bool mInvalidatedTemporary;

	//--------------------------------------------------------------------------
	KeyboardImpl()
		:mValid(true), mLastPushKeyTime(0), mInvalidatedTemporary(false){
		memset(mKeyDown, 0, sizeof(mKeyDown));
		memset(mKeyPressed, 0, sizeof(mKeyPressed));
		memset(mKeyUp, 0, sizeof(mKeyUp));
	}

	void EndFrame(Timer::TIME_PRECISION gameTimeInSecond){
		mValid = true;
		memset(mKeyPressed, 0, sizeof(mKeyPressed));
		memset(mKeyUp, 0, sizeof(mKeyUp));
		if (!mCurrentChar.empty()){
			if (gameTimeInSecond - mLastPushKeyTime > 2.0f)
			{
				ClearWithSwap(mCurrentChar);
			}
		}
	}

	bool IsValid() const { 
		return mValid && !mInvalidatedTemporary; 
	}

	void Invalidate(bool buttonClicked = false){
		mValid = false;
	}

	void InvalidTemporary(bool invalidate){
		mInvalidatedTemporary = invalidate;
	}

	//--------------------------------------------------------------------------
	void PushEvent(HWindow hWnd, const KeyboardEvent& keyboardEvent){
		/*DebugOutput("MakeCode = %d, Flags = %x, VKey = %d, Message = %d, ExtraInformation = %d",
		keyboardEvent.MakeCode, keyboardEvent.Flags, keyboardEvent.VKey, keyboardEvent.Message, keyboardEvent.ExtraInformation);*/

		if (keyboardEvent.Flags & 1) // key up
		{

			unsigned int key[] = { keyboardEvent.VKey, 0 };
			if (IsPairedKey(keyboardEvent.VKey))
				key[1] = TranslateIndex(keyboardEvent);

			for (unsigned i = 0; i<2; i++)
			{
				if (key[i] == 0)
					continue;

				mKeyDown[key[i]] = false;
				mKeyUp[key[i]] = true;
			}
		}
		if (!(keyboardEvent.Flags & 1))// key down
		{
			unsigned int key[] = { keyboardEvent.VKey, 0 };
			if (IsPairedKey(keyboardEvent.VKey))
				key[1] = TranslateIndex(keyboardEvent);

			for (unsigned i = 0; i<2; i++)
			{
				if (key[i] == 0)
					continue;

				mKeyPressed[key[i]] = true;
				mKeyDown[key[i]] = true;
			}
		}
	}

	void PushChar(HWindow hWnd, unsigned keycode, Timer::TIME_PRECISION gameTimeInSec){
		mCurrentChar.push(keycode);
		while (mCurrentChar.size() > 10)
		{
			mCurrentChar.pop();
		}
		mLastPushKeyTime = gameTimeInSec;
	}

	unsigned GetChar(){
		if (mCurrentChar.empty())
			return 0;
		unsigned ret = mCurrentChar.front();
		return ret;
	}

	void PopChar(){
		if (!mCurrentChar.empty()){
			mCurrentChar.pop();
			Invalidate();
		}
	}

	void ClearBuffer(){
		ClearWithSwap(mCurrentChar);
	}

	void OnKillFocus(){
		memset(mKeyDown, 0, sizeof(mKeyDown));
		memset(mKeyPressed, 0, sizeof(mKeyPressed));
	}

	bool IsPairedKey(unsigned short keycode) const
	{
		return keycode == VK_SHIFT || keycode == VK_CONTROL || keycode == VK_MENU;
	}

	//--------------------------------------------------------------------------
	unsigned short TranslateIndex(const KeyboardEvent& keyboardEvent) const
	{
		assert(IsPairedKey(keyboardEvent.VKey));

		int offset = 0;
		if (keyboardEvent.Flags & KEYBOARD_FLAG_KEY_E1)
		{
			offset = 1;
		}

		switch (keyboardEvent.VKey)
		{
		case VK_SHIFT:
		{
			return VK_LSHIFT + offset;
		}
		break;

		case VK_CONTROL:
		{
			return VK_LCONTROL + offset;
		}
		break;

		case VK_MENU:
		{
			return VK_LMENU + offset;
		}
		break;

		default:
		{
			assert(0 && "Not defined pair key!");
			return 0;
		}

		}


	}
};

	//--------------------------------------------------------------------------
	Keyboard::Keyboard()
		:mImpl(new KeyboardImpl)		
	{
		
	}

	Keyboard::~Keyboard(){
		delete mImpl;
	}

	//--------------------------------------------------------------------------
	void Keyboard::PushEvent(HWindow hWnd, const KeyboardEvent& keyboardEvent){
		mImpl->PushEvent(hWnd, keyboardEvent);
	}

	void Keyboard::PushChar(HWindow hWnd, unsigned keycode, Timer::TIME_PRECISION gameTimeInSec){
		mImpl->PushChar(hWnd, keycode, gameTimeInSec);
	}

	//--------------------------------------------------------------------------
	void Keyboard::EndFrame(Timer::TIME_PRECISION gameTimeInSecond) {
		mImpl->EndFrame(gameTimeInSecond);
	}

	bool Keyboard::IsValid() const{
		return mImpl->IsValid();
	}

	//--------------------------------------------------------------------------
	void Keyboard::Invalidate(bool buttonClicked){
		mImpl->Invalidate(buttonClicked);
	}

	void Keyboard::InvalidTemporary(bool invalidate){
		mImpl->InvalidTemporary(invalidate);
	}

	//--------------------------------------------------------------------------
	bool Keyboard::IsKeyDown(unsigned short keycode) const{
		return mImpl->mKeyDown[keycode];
	}

	bool Keyboard::IsKeyPressed(unsigned short keycode) const{
		return mImpl->mKeyPressed[keycode];
	}

	bool Keyboard::IsKeyUp(unsigned short keycode) const{
		return mImpl->mKeyUp[keycode];
	}

	unsigned Keyboard::GetChar(){
		return mImpl->GetChar();
	}

	void Keyboard::PopChar()
	{
		mImpl->PopChar();
	}

	void Keyboard::ClearBuffer()
	{
		mImpl->ClearBuffer();
	}

	void Keyboard::OnKillFocus()
	{
		mImpl->PopChar();
	}
}