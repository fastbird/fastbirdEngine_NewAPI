#pragma once
namespace fastbird{
	class IRenderTargetObserver{
	public:
		virtual void OnRenderTargetSizeChanged(int x, int y, HWindow associatedWindow){}
	};
}