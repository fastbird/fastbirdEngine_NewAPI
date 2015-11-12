#pragma once
#include "FBCommonHeaders/platform.h"
#include "RendererEnums.h"
#include "RendererStructs.h"
#include "Color.h"
#include "FBCommonHeaders/Observable.h"
#include "IRenderTargetObserver.h"
namespace fastbird
{
	struct RenderTargetParam;
	class Scene;
	class IRenderTargetListener;
	typedef unsigned RenderTargetId;
	DECLARE_SMART_PTR(IInputInjector);
	DECLARE_SMART_PTR(GaussianDist);
	DECLARE_SMART_PTR(RenderPipeline);	
	DECLARE_SMART_PTR(Texture);
	DECLARE_SMART_PTR(Scene);
	DECLARE_SMART_PTR(Camera);
	DECLARE_SMART_PTR(Mouse);
	DECLARE_SMART_PTR(Keyboard);
	DECLARE_SMART_PTR(Renderer);
	DECLARE_SMART_PTR(RenderTarget);
	class FB_DLL_PUBLIC RenderTarget : public Observable<IRenderTargetObserver>
	{
		static RenderTargetId NextRenderTargetId;
		static const int FB_NUM_BLOOM_TEXTURES = 3;
		static const int FB_NUM_STAR_TEXTURES = 12;

		friend class RenderResourceFactory;
		RenderTarget();

	public:
		DECLARE_PIMPL(RenderTarget);
		~RenderTarget();

		//-------------------------------------------------------------------
		// Observable<IRenderTargetObserver>
		//-------------------------------------------------------------------
		virtual void OnObserverAdded(IRenderTargetObserver* observer);
		virtual void OnObserverRemoved(IRenderTargetObserver* observer);

		//-------------------------------------------------------------------
		// InputConsumer From Renderer
		//-------------------------------------------------------------------
		void ConsumeInput(IInputInjectorPtr injector);

		//-------------------------------------------------------------------
		RenderTargetId GetId() const;
		bool CheckOptions(const RenderTargetParam& param);

		ScenePtr RegisterScene(ScenePtr scene);
		ScenePtr GetScene() const;
		CameraPtr ReplaceCamera(CameraPtr cam);
		CameraPtr GetCamera() const;

		TexturePtr GetRenderTargetTexture() const;
		TexturePtr GetDepthStencilTexture() const;
		const Vec2I& GetSize() const;
		void DeleteBuffers();

		void SetColorTextureDesc(int width, int height, PIXEL_FORMAT format, bool srv, bool miplevel, bool cubeMap);
		void SetDepthStencilDesc(int width, int height, PIXEL_FORMAT format, bool srv, bool cubeMap);
		void SetClearValues(const Color& color, Real z, UINT8 stencil);
		void SetClearColor(const Color& color);
		void SetClearDepthStencil(Real z, UINT8 stencil);

		void Bind(size_t face = 0);
		void BindTargetOnly(bool hdr);
		bool Render(size_t face=0);
		void Unbind();

		void SetEnable(bool enable);
		bool GetEnable() const;
		void SetEnvTexture(TexturePtr texture);
		void SetUsePool(bool usePool);
		bool GetUsePool() const;
		void AddListener(IRenderTargetListener* listener);
		void RemoveListener(IRenderTargetListener* listener);

		void SetColorTexture(TexturePtr pTexture);
		void SetDepthTexture(TexturePtr pTexture);
		CameraPtr GetLightCamera() const;
		const Viewport& GetViewport() const;

		void SetLightCamWidth(Real width);
		void SetLightCamHeight(Real height);
		void SetLightCamNear(Real n);
		void SetLightCamFar(Real f);
		void DrawOnEvent(bool set);
		void TriggerDrawEvent();

	};
}