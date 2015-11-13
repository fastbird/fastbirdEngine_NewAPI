#pragma once
#include "FBCommonHeaders/platform.h"
#include "IRenderStrategy.h"
namespace fastbird{
	DECLARE_SMART_PTR(RenderTarget);
	DECLARE_SMART_PTR(Scene);
	DECLARE_SMART_PTR(RenderStrategyDefault);
	class RenderStrategyDefault : public IRenderStrategy{
		DECLARE_PIMPL_NON_COPYABLE(RenderStrategyDefault);
		RenderStrategyDefault();

	public:	
		static RenderStrategyDefaultPtr Create();
		~RenderStrategyDefault();

		//-------------------------------------------------------------------
		// IRenderStrategy
		//-------------------------------------------------------------------
		void SetScene(ScenePtr scene);
		void SetRenderTarget(RenderTargetPtr renderTarget);
		void Render(size_t face);
		bool IsHDR() const;
		bool SetHDRTarget();
		CameraPtr GetLightCamera() const;
		bool SetSmallSilouetteBuffer();
		bool SetBigSilouetteBuffer();
		void SetGlowRenderTarget();
	};
}