#pragma once
#include "FBCommonHeaders/platform.h"
#include "IRenderStrategy.h"
namespace fastbird{
	DECLARE_SMART_PTR(RenderStrategyMinimum);
	class FB_DLL_RENDERER RenderStrategyMinimum : public IRenderStrategy{
		DECLARE_PIMPL_NON_COPYABLE(RenderStrategyMinimum);
		RenderStrategyMinimum();

	public:
		static RenderStrategyMinimumPtr Create();
		~RenderStrategyMinimum();

		//-------------------------------------------------------------------
		// IRenderStrategy
		//-------------------------------------------------------------------
		void SetScene(ScenePtr scene);
		void SetRenderTarget(RenderTargetPtr renderTarget);
		void UpdateLightCamera();
		void Render(size_t face);
		bool IsHDR() const;
		bool IsGlowSupported();
		CameraPtr GetLightCamera() const;
		bool SetHDRTarget();
		bool SetSmallSilouetteBuffer();
		bool SetBigSilouetteBuffer();
		void GlowRenderTarget(bool bind);
		void DepthTexture(bool bind);
	};
}