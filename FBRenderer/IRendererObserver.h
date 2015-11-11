#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	class IRendererObserver{
	public:
		virtual void BeforeUIRendering(HWindowId hwndId, HWindow hwnd) {}
		virtual void AfterUIRendered(HWindowId hwndId, HWindow hwnd) {}
		virtual void BeforeDebugHudRendered(HWindowId hwndId, HWindow hwnd) {}
		virtual void AfterDebugHudRendered(HWindowId hwndId, HWindow hwnd) {}
		virtual void OnResolutionChanged(HWindowId hwndId, HWindow hwnd) {}		
	};
}