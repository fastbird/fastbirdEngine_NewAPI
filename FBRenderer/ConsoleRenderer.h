#pragma once
#include "FBCommonHeaders/Types.h"
#include "FBConsole/IConsoleRenderer.h"
namespace fastbird{
	struct RenderParamOut;
	struct RenderParam;
	FB_DECLARE_SMART_PTR(ConsoleRenderer);
	class ConsoleRenderer : public IConsoleRenderer{
		FB_DECLARE_PIMPL_NON_COPYABLE(ConsoleRenderer);
		ConsoleRenderer();
		~ConsoleRenderer();

	public:
		static ConsoleRendererPtr Create();		

		void Render();

		// IConsoleRenderer
		int GetTextWidth(const wchar_t* text, int count = -1, float *outMinY = 0, float *outMaxY = 0);
		std::wstring StripTags(const wchar_t* text);
		int NumLinesRender();
		HWindow GetMainWindowHandle();
	};
}