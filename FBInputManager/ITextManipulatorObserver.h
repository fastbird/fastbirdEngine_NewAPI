#pragma once

namespace fastbird{
	class TextManipulator;
	class ITextManipulatorObserver
	{
	public:
		enum{
			Default
		};
		virtual void OnCursorPosChanged(TextManipulator* mani) {}
		virtual void OnTextChanged(TextManipulator* mani) {}
	};
}