#pragma once
#include <memory>
namespace fastbird{
	class Camera;
	typedef std::shared_ptr<Camera> CameraPtr;
	class Scene;
	typedef std::shared_ptr<Scene> ScenePtr;
	class RenderTarget;
	typedef std::shared_ptr<RenderTarget> RenderTargetPtr;
	class IRenderStrategy;
	typedef std::shared_ptr<IRenderStrategy> IRenderStrategyPtr;
	typedef std::weak_ptr<IRenderStrategy> IRenderStrategyWeakPtr;
	class IRenderStrategy{
	public:
		virtual ~IRenderStrategy(){}

	private:
		friend class RenderTarget::RenderTargetImpl;
		virtual void SetScene(ScenePtr scene) = 0;
		virtual void SetRenderTarget(RenderTargetPtr renderTarget) = 0;
		virtual void Render(size_t face) = 0;
		virtual bool IsHDR() const = 0;
		virtual bool SetHDRTarget() = 0;
		virtual CameraPtr GetLightCamera() const = 0;
		virtual bool SetSmallSilouetteBuffer() = 0;
		virtual bool SetBigSilouetteBuffer() = 0;
	};
}