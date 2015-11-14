#pragma once
#include <memory>
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	DECLARE_SMART_PTR(Camera);
	DECLARE_SMART_PTR(Scene);
	DECLARE_SMART_PTR(RenderTarget);	
	DECLARE_SMART_PTR(IRenderStrategy);
	class IRenderStrategy{
	protected:
		virtual ~IRenderStrategy(){}

	public:
		virtual void SetScene(ScenePtr scene) = 0;
		virtual void SetRenderTarget(RenderTargetPtr renderTarget) = 0;
		virtual void Render(size_t face) = 0;
		virtual bool IsHDR() const = 0;
		virtual bool IsGlowSupported() = 0;		
		virtual CameraPtr GetLightCamera() const = 0;
		virtual bool SetHDRTarget() = 0;
		virtual bool SetSmallSilouetteBuffer() = 0;
		virtual bool SetBigSilouetteBuffer() = 0;	
		virtual void GlowRenderTarget(bool bind) = 0;
		virtual void DepthTexture(bool bind) = 0;
	};
}