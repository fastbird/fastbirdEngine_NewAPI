#pragma once
namespace fastbird{
	class ICameraObserver{
	public:
		virtual void OnViewMatrixChanged() = 0;
		virtual void OnProjMatrixChanged() = 0;
	};
}