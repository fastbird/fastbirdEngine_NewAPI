#pragma once
#include "IInputConsumer.h"
#include "ITextManipulatorObserver.h"
#include "FBCommonHeaders/Observable.h"
namespace fastbird{
	class IKeyboard;
	class IMouse;	
	DECLARE_SMART_PTR(TextManipulator);
	class FB_DLL_INPUTMANAGER TextManipulator : public IInputConsumer, public Observable<ITextManipulatorObserver>
	{
		int mCursorPos;
		int mHighlightStart;
		std::wstring* mText;

		TextManipulator();
		~TextManipulator();

	private:
		
		void EndHighlighting() { mHighlightStart = -1; }
		void OnCursorPosChanged();
		void OnTextChanged();
		void Highlighting(bool shiftkey);
		void StartHighlighting();
		

	public:
		
		static TextManipulatorPtr Create();
		// Input Consumer
		virtual void ConsumeInput(IInputInjectorPtr injector);	

		virtual void SetText(std::wstring* text);				
		virtual int GetCursorPos() const;
		virtual void SetCursorPos(int pos);
		bool IsHighlighting() const { return mHighlightStart != -1; }
		virtual int GetHighlightStart() const { return mHighlightStart; }
		virtual void SelectAll();
	};
}