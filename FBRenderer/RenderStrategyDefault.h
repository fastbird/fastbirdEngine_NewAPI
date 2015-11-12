#pragma once
#include "FBCommonHeaders/platform.h"
#include "IRenderStrategy.h"
namespace fastbird{
	DECLARE_SMART_PTR(RenderTarget);
	DECLARE_SMART_PTR(Scene);
	class RenderStrategyDefault : public IRenderStrategy{
		DECLARE_PIMPL(RenderStrategyDefault);		

	public:
		RenderStrategyDefault();
		~RenderStrategyDefault();

	private:

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

		//-------------------------------------------------------------------
		// Own - Intentionally not located in the RenderStrategyDefaultImpl class.
		//-------------------------------------------------------------------
		void ClearSilouetteBuffer();
		void RenderTargetColor(bool bind);
		void RenderTargetDepth(bool bind);

		void UpdateLightCamera();
		void ShadowTarget(bool bind);
		void ShadowMap(bool bind);

		void DepthTarget(bool bind, bool clear);
		void DepthTexture(bool bind);

		void CloudVolumeTarget(bool bind);
		void CloudVolumeTexture(bool bind);

		void GodRayTarget(bool bind);
		void GodRay();

		void HDRTarget(bool bind);
		void Silouette();

		void GlowTarget(bool bind);
		void GlowTexture(bool bind);
		void BlendGlow();
		void BlendGodRay();

		void MeasureLuminanceOfHDRTarget();
		void BrightPass();
		void BrightPassToStarSource();
		void StarSourceToBloomSource();
		void Bloom();
		void RenderStarGlare();
		void ToneMapping();

		void ScenePreRender();
		void SceneRender();
	};
}