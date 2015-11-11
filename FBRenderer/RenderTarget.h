#pragma once
#include "RendererEnums.h"
#include "RendererStructs.h"
#include "Color.h"
#include "FBCommonHeaders/Observable.h"
#include "IRenderTargetObserver.h"
namespace fastbird
{
	struct GaussianDist;
	class Scene;
	class IRenderTargetListener;
	typedef unsigned RenderTargetId;
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

		DECLARE_PIMPL(RenderTarget);
		friend class RenderResourceFactory;
		RenderTarget();

	public:
		~RenderTarget();

		//-------------------------------------------------------------------
		// Observable<IRenderTargetObserver>
		//-------------------------------------------------------------------
		virtual void OnObserverAdded(IRenderTargetObserver* observer);
		virtual void OnObserverRemoved(IRenderTargetObserver* observer);

		//-------------------------------------------------------------------
		bool CheckOptions(const RenderTargetParam& param);
		RenderPipeline& GetRenderPipeline() const;		

		ScenePtr RegisterScene(ScenePtr scene);
		ScenePtr GetScene() const;
		CameraPtr ReplaceCamera(CameraPtr cam);
		CameraPtr GetCamera() const;

		TexturePtr GetRenderTargetTexture() const;
		TexturePtr GetDepthStencilTexture() const;
		const Vec2I& GetSize() const;
		void DeleteBuffers();
		void SetClearValues(const Color& color, float z, UINT8 stencil);
		void SetClearColor(const Color& color);
		void SetClearDepthStencil(float z, UINT8 stencil);

		void Bind(size_t face = 0);
		void BindTargetOnly(bool hdr);
		bool Render(size_t face=0);
		void Unbind();

		void SetEnable(bool enable);
		bool GetEnable() const;
		void SetEnvTexture(TexturePtr texture);
		void SetUsePool(bool usePool);
		bool GetUsePool() const;

		void OnInputFromHandler(MousePtr pMouse, KeyboardPtr pKeyboard);

		void AddListener(IRenderTargetListener* listener);
		void RemoveListener(IRenderTargetListener* listener);

		void SetColorTexture(TexturePtr pTexture);
		CameraPtr GetLightCamera() const;

		//-------------------------------------------------------------------
		// Post processors
		//-------------------------------------------------------------------
		void SetDepthRenderTarget(bool clear);
		void UnsetDepthRenderTarget();
		void BindDepthTexture(bool set);

		void SetGlowRenderTarget();
		void UnSetGlowRenderTarget();

		void PrepareShadowMapRendering();
		void EndShadowMapRendering();
		void BindShadowMap(bool bind);

		void SetCloudVolumeTarget();
		void SetCloudVolumeTexture(bool set);

		void SetGodRayRenderTarget();
		void GodRay();
		
		void SetHDRTarget();
		void DrawSilouette();
		
		void SetSmallSilouetteBuffer();
		void SetBigSilouetteBuffer();

		void CalcLuminance();

		void BlendGlow();
		void BlendGodRay();
		void MeasureLuminanceOfHDRTargetNew();
		void BrightPass();
		void BrightPassToStarSource();
		void StarSourceToBloomSource();
		void Bloom();
		void RenderStarGlare();
		void ToneMapping();

		// internal access only
		TexturePtr GetShadowMap() const;
		void DeleteShadowMap();

		void SetLightCamWidth(float width);
		void SetLightCamHeight(float height);
		void SetLightCamNear(float n);
		void SetLightCamFar(float f);
		void UpdateLightCamera();

		static void ReleaseStarDef();

		void DrawOnEvent(bool set);
		void TriggerDrawEvent();

	};
}