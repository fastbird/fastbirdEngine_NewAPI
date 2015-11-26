#pragma once
namespace fastbird{
	class IConsoleRenderer{
	public:
		/// text should be in the format defined in Font.
		/// default is wide: 2 bytes.
		virtual int GetTextWidth(const wchar_t* text, int count = -1, float *outMinY = 0, float *outMaxY = 0) = 0;
		virtual std::wstring StripTags(const wchar_t* text) = 0;
		virtual int NumLinesRender() = 0;
		virtual HWindow GetMainWindowHandle() = 0;
	};
}