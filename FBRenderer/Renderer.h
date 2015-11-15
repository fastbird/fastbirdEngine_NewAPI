#pragma once
#include "FBCommonHeaders/Observable.h"
#include "IRendererObserver.h"
#include "RendererEnums.h"
#include "RendererStructs.h"
#include "ShaderDefines.h"
#include "InputElementDesc.h"
#include "RenderTargetParam.h"
#include "InputLayout.h"
#include "IPlatformIndexBuffer.h"
#include "RenderStates.h"
#include "TextureBinding.h"
#include "SystemTextures.h"
#include "FBSceneManager/ISceneObserver.h"
#include "FBMathLib/Math.h"
#include "FBTimerLib/Timer.h"
#include "../EssentialEngineData/shaders/Constants.h"
struct lua_State;
namespace fastbird{	
	class RenderOptions;
	typedef unsigned RenderTargetId;
	DECLARE_SMART_PTR(Camera);
	DECLARE_SMART_PTR(VideoPlayer);
	DECLARE_SMART_PTR(PointLightManager);
	DECLARE_SMART_PTR(Font);
	DECLARE_SMART_PTR(DirectionalLight);
	DECLARE_SMART_PTR(PointLight);
	DECLARE_SMART_PTR(Scene);	
	DECLARE_SMART_PTR_STRUCT(TextureAtlasRegion);
	DECLARE_SMART_PTR(TextureAtlas);	
	DECLARE_SMART_PTR(Material);
	DECLARE_SMART_PTR(Shader);
	DECLARE_SMART_PTR(IndexBuffer);
	DECLARE_SMART_PTR(VertexBuffer);
	DECLARE_SMART_PTR(Texture);
	DECLARE_SMART_PTR(RenderTarget);
	DECLARE_SMART_PTR(Renderer);
	/** Render vertices with a specified material	
	Rednerer handles vertex/index data, materials, textures, shaders,
	render states, lights and render targets.
	\ingroup FBRenderer
	*/
	class FB_DLL_PUBLIC Renderer : public Observable<IRendererObserver>, public ISceneObserver{
		RendererWeakPtr mMe;

		DECLARE_PIMPL_NON_COPYABLE(Renderer);
		Renderer();		

	public:
		/** Create an empty Renderer. You have the owner ship of the pointer.
		*/
		static RendererPtr CreateRenderer();

		/** Create a Renderer with the platform specific render engine you specified in \a rendererPlugInName 
		\param rendererPlugInName The renderer plug-in name. You can specify "FBRendererD3D11" for
		the default renderer on Windows, and "FBRendererGL41" for Mac. \n
		\param L Renderer need the lua_State* to read the configuration file. If you pass NULL, all setting will be default.
		*/
		static RendererPtr CreateRenderer(const char* rendererPlugInName, lua_State* L);

		static RendererPtr GetInstance();
	public:
		~Renderer();

		static const HWindowId INVALID_HWND_ID = (HWindowId)-1;
		static const int FB_NUM_TONEMAP_TEXTURES = 4;
		static const int FB_NUM_TONEMAP_TEXTURES_NEW = 5;
		static const int FB_NUM_LUMINANCE_TEXTURES = 3;
		static const int FB_MAX_SAMPLES = 16;      // Maximum number of texture grabs

		/** Set lua state.
		Renderer load configuration file using lua. You need to set it before you call Renderer::PrepareRenderEngine()
		If you don't set the lua_State, all setting will be default values.
		*/
		void SetLuaState(lua_State* L);

		/** Prepare the platform specific render engine
		You don't call this function if you initiate the Renderer with the function
		 \code 
		 static RendererPtr CreateRenderer(const char* rendererPlugInName); 
		 \endcode
		 because the Renderer you got is already initialized the render engine.		 
		*/
		void PrepareRenderEngine(const char* rendererPlugInName);

		//-------------------------------------------------------------------
		// Canvas & System
		//-------------------------------------------------------------------
		bool InitCanvas(HWindowId id, HWindow window, int width, int height);
		void ReleaseCanvas(HWindowId id);
		void SetTimer(TimerPtr timer);
		TimerPtr GetTimer() const;

		//-------------------------------------------------------------------
		// Resource Creation
		//-------------------------------------------------------------------
		RenderTargetPtr CreateRenderTarget(const RenderTargetParam& param);
		/** After the render target is kept in pool, you should stop using it.
		If you need to get it again use Renderer::CreateRenderTarget() again with
		setting mUsePool in param as true.*/
		void KeepRenderTargetInPool(RenderTargetPtr rt);
		/** Load texture file asynchronously */
		TexturePtr CreateTexture(const char* file, bool async);		
		TexturePtr CreateTexture(void* data, int width, int height, PIXEL_FORMAT format,
			BUFFER_USAGE usage, int  buffer_cpu_access, int texture_type);
		VertexBufferPtr CreateVertexBuffer(void* data, unsigned stride,
			unsigned numVertices, BUFFER_USAGE usage, BUFFER_CPU_ACCESS_FLAG accessFlag);
		IndexBufferPtr CreateIndexBuffer(void* data, unsigned int numIndices,
			INDEXBUFFER_FORMAT format);
		ShaderPtr CreateShader(const char* filepath, int shaders,
			const SHADER_DEFINES& defines = SHADER_DEFINES(), ShaderPtr pReloadingShader = 0);
		MaterialPtr CreateMaterial(const char* file);
		MaterialPtr GetMissingMaterial();
		// use this if you are sure there is instance of the descs.
		InputLayoutPtr CreateInputLayout(const INPUT_ELEMENT_DESCS& descs, ShaderPtr shader);
		InputLayoutPtr GetInputLayout(const INPUT_ELEMENT_DESCS& descs);
		// use these if you are not sure.
		InputLayoutPtr GetInputLayout(const INPUT_ELEMENT_DESCS& descs,
			MaterialPtr material);
		InputLayoutPtr GetInputLayout(const INPUT_ELEMENT_DESCS& descs,
			ShaderPtr shader);
		// auxiliary
		InputLayoutPtr GetInputLayout(DEFAULT_INPUTS::Enum e,
			MaterialPtr material);
		InputLayoutPtr GetInputLayout(DEFAULT_INPUTS::Enum e,
			ShaderPtr shader);
		RasterizerStatePtr CreateRasterizerState(const RASTERIZER_DESC& desc);
		BlendStatePtr CreateBlendState(const BLEND_DESC& desc);
		DepthStencilStatePtr CreateDepthStencilState(const DEPTH_STENCIL_DESC& desc);
		SamplerStatePtr CreateSamplerState(const SAMPLER_DESC& desc);		
		TextureAtlasPtr GetTextureAtlas(const char* path);		
		TextureAtlasRegionPtr GetTextureAtlasRegion(const char* path, const char* region);
		TexturePtr GetTemporalDepthBuffer(const Vec2I& size);
		PointLightPtr CreatePointLight(const Vec3& pos, Real range, const Vec3& color, Real intensity, Real lifeTime,
			bool manualDeletion);
		MaterialPtr GetMaterial(DEFAULT_MATERIALS::Enum type);

		//-------------------------------------------------------------------
		// Hot reloading
		//-------------------------------------------------------------------
		bool ReloadShader(ShaderPtr shader);
		bool ReloadTexture(ShaderPtr shader);

		//-------------------------------------------------------------------
		// Resource Bindings
		//-------------------------------------------------------------------
		void SetRenderTarget(TexturePtr pRenderTargets[], size_t rtViewIndex[], int num,
			TexturePtr pDepthStencil, size_t dsViewIndex);		
		void SetViewports(const Viewport viewports[], int num);
		void SetScissorRects(Rect rects[], int num);
		void SetVertexBuffer(unsigned int startSlot, unsigned int numBuffers,
			VertexBufferPtr pVertexBuffers[], unsigned int strides[], unsigned int offsets[]);
		void SetPrimitiveTopology(PRIMITIVE_TOPOLOGY pt);
		void SetInputLayout(InputLayoutPtr pInputLayout);
		void SetIndexBuffer(IndexBufferPtr pIndexBuffer);
		void SetShaders(ShaderPtr pShader);
		void SetVSShader(ShaderPtr pShader);
		void SetPSShader(ShaderPtr pShader);
		void SetGSShader(ShaderPtr pShader);
		void SetDSShader(ShaderPtr pShader);
		void SetHSShader(ShaderPtr pShader);		
		void SetTexture(TexturePtr pTexture, BINDING_SHADER shaderType, unsigned int slot);
		/** Bind serveral texture at once.
		The texture order apears in the shader(BINDING_SHADER) should be sequential.
		*/
		void SetTextures(TexturePtr pTextures[], int num, BINDING_SHADER shaderType, int startSlot);
		void SetSystemTexture(SystemTextures::Enum type, TexturePtr texture);
		// pre defined
		void BindDepthTexture(bool set);		
		void SetDepthWriteShader();
		void SetDepthWriteShaderCloud();
		void SetOccPreShader();
		void SetOccPreGSShader();
		void SetPositionInputLayout();

		void SetSystemTextureBindings(SystemTextures::Enum type, const TextureBindings& bindings);
		const TextureBindings& GetSystemTextureBindings(SystemTextures::Enum type) const;

		ShaderPtr GetGodRayPS();
		ShaderPtr GetGlowShader();
		void SetShadowMapShader();
		ShaderPtr GetSilouetteShader();
		ShaderPtr GetCopyPS();
		ShaderPtr GetCopyPSMS();
		TexturePtr GetToneMap(unsigned idx);
		ShaderPtr GetSampleLumInitialShader();
		ShaderPtr GetSampleLumIterativeShader();
		ShaderPtr GetSampleLumFinalShader();
		void SwapLuminanceMap();
		TexturePtr GetLuminanceMap(unsigned idx);
		ShaderPtr GetCalcAdapedLumShader();
		ShaderPtr GetBrightPassPS();
		ShaderPtr GetBlur5x5PS();
		ShaderPtr GetBloomPS();
		ShaderPtr GetStarGlareShader();
		ShaderPtr GetMergeTexturePS();
		ShaderPtr GetToneMappingPS();

		//-------------------------------------------------------------------
		// Device RenderStates
		//-------------------------------------------------------------------
		void RestoreRenderStates();
		void RestoreRasterizerState();
		void RestoreBlendState();
		void RestoreDepthStencilState();
		void LockDepthStencilState();
		void UnlockDepthStencilState();
		void LockBlendState();
		void UnlockBlendState();
		// blend
		void SetAlphaBlendState();
		void SetAdditiveBlendState();
		void SetMaxBlendState();
		void SetRedAlphaMask();
		void SetGreenAlphaMask();
		void SetGreenMask();
		void SetBlueMask();
		void SetGreenAlphaMaskMax();
		void SetGreenAlphaMaskAddAddBlend();
		void SetRedAlphaMaskAddMinusBlend();
		void SetGreenAlphaMaskAddMinusBlend();
		void SetRedAlphaMaskAddAddBlend();
		// depth
		void SetNoDepthWriteLessEqual();
		void SetLessEqualDepth();
		void SetNoDepthStencil();
		// raster
		void SetFrontFaceCullRS();
		// sampler
		void SetSamplerState(SAMPLERS::Enum s, BINDING_SHADER shader, int slot);

		//-------------------------------------------------------------------
		// Resource Manipulation
		//-------------------------------------------------------------------
		MapData MapVertexBuffer(VertexBufferPtr pBuffer, UINT subResource,
			MAP_TYPE type, MAP_FLAG flag);
		void UnmapVertexBuffer(VertexBufferPtr pBuffer, unsigned int subResource);
		MapData MapTexture(TexturePtr pTexture, UINT subResource,
			MAP_TYPE type, MAP_FLAG flag);
		void UnmapTexture(TexturePtr pTexture, UINT subResource);
		void CopyToStaging(TexturePtr dst, UINT dstSubresource, UINT dstx, UINT dsty, UINT dstz,
			TexturePtr src, UINT srcSubresource, Box3D* pBox);
		void SaveTextureToFile(TexturePtr texture, const char* filename);


		//-------------------------------------------------------------------
		// GPU constants
		//-------------------------------------------------------------------
		void UpdateObjectConstantsBuffer(const void* pData, bool record = false);
		void UpdatePointLightConstantsBuffer(const void* pData);
		void UpdateFrameConstantsBuffer();
		void UpdateMaterialConstantsBuffer(const void* pData);
		void UpdateCameraConstantsBuffer();
		void UpdateRenderTargetConstantsBuffer();
		void UpdateSceneConstantsBuffer();
		void UpdateRareConstantsBuffer();
		void UpdateRadConstantsBuffer(const void* pData);
		void* MapMaterialParameterBuffer();
		void UnmapMaterialParameterBuffer();
		void* MapBigBuffer();
		void UnmapBigBuffer();

		//-------------------------------------------------------------------
		// GPU Manipulation
		//-------------------------------------------------------------------
		void DrawIndexed(unsigned indexCount, unsigned startIndexLocation, unsigned startVertexLocation);
		void Draw(unsigned int vertexCount, unsigned int startVertexLocation);
		void SetClearColor(HWindowId id, const Color& color);
		void SetClearDepthStencil(HWindowId id, Real z, UINT8 stencil);
		void Clear(Real r, Real g, Real b, Real a, Real z, UINT8 stencil);
		void Clear(Real r, Real g, Real b, Real a);
		// Avoid to use
		void ClearState();
		void BeginEvent(const char* name);
		void EndEvent();
		void TakeScreenshot();
		

		//-------------------------------------------------------------------
		// Renderer State
		//-------------------------------------------------------------------
		void SetWireframe(bool enable);
		bool GetWireframe() const;
		RenderTargetPtr GetMainRenderTarget() const;
		ScenePtr GetMainScene() const; // move to SceneManager
		const Vec2I& GetMainRTSize() const;
		void SetCurrentRenderTarget(RenderTargetPtr renderTarget);
		RenderTargetPtr GetCurrentRenderTarget() const;
		bool IsMainRenderTarget() const;
		const Vec2I& GetRenderTargetSize(HWindowId id = INVALID_HWND_ID) const;
		const Vec2I& GetRenderTargetSize(HWindow hwnd = 0) const;
		void SetDirectionalLight(DirectionalLightPtr pLight, int idx);
		DirectionalLightPtr GetDirectionalLight(int idx) const;
		DirectionalLightPtr GetMainDirectionalLight(int idx) const;
		void InitFrameProfiler(Real dt);
		const RENDERER_FRAME_PROFILER& GetFrameProfiler() const;
		inline FontPtr GetFont(Real fontHeight) const;
		const INPUT_ELEMENT_DESCS& GetInputElementDesc(
			DEFAULT_INPUTS::Enum e);
		void SetEnvironmentTexture(TexturePtr pTexture);
		void SetEnvironmentTextureOverride(TexturePtr texture);		
		void SetDebugRenderTarget(unsigned idx, const char* textureName);
		void SetFadeAlpha(Real alpha);
		PointLightManPtr GetPointLightMan() const;
		void RegisterVideoPlayer(VideoPlayerPtr player);
		void UnregisterVideoPlayer(VideoPlayerPtr player);
		void GetSampleOffsets_GaussBlur5x5(DWORD texWidth, DWORD texHeight, Vec4** avTexCoordOffset, Vec4** avSampleWeight, Real fMultiplier);
		void GetSampleOffsets_DownScale2x2(DWORD texWidth, DWORD texHeight, Vec4* avTexCoordOffset);
		bool IsLuminanceOnCpu() const;

		//-------------------------------------------------------------------
		// Queries
		//-------------------------------------------------------------------
		unsigned GetMultiSampleCount() const;
		RenderTargetPtr GetRenderTarget(HWindowId id) const;
		void SetCamera(CameraPtr pCamera);
		CameraPtr GetCamera() const; // this is for current carmera.
		CameraPtr GetMainCamera() const;
		HWindow GetWindowHandle(RenderTargetId rtId);
		Vec2I ToSreenPos(HWindowId id, const Vec3& ndcPos) const;
		Vec2 ToNdcPos(HWindowId id, const Vec2I& screenPos) const;
		unsigned GetNumLoadingTexture() const;
		Vec2I FindClosestSize(HWindowId id, const Vec2I& input);
		bool GetResolutionList(unsigned& outNum, Vec2I* list);
		RenderOptions* GetOptions() const;

		//-------------------------------------------------------------------
		// Drawing
		//-------------------------------------------------------------------
		void DrawFullscreenQuad(ShaderPtr pixelShader, bool farside);
		void DrawText(const Vec2I& pos, WCHAR* text, const Color& color, Real size = 20);
		void DrawText(const Vec2I& pos, const char* text, const Color& color, Real size = 20);
		void Draw3DText(const Vec3& worldpos, WCHAR* text, const Color& color, Real size = 20);
		void Draw3DText(const Vec3& worldpos, const char* text, const Color& color, Real size = 20);
		void DrawTextForDuration(Real secs, const Vec2I& pos, WCHAR* text,
			const Color& color, Real size = 20);
		void DrawTextForDuration(Real secs, const Vec2I& pos, const char* text,
			const Color& color, Real size = 20);
		void ClearDurationTexts();
		void DrawLine(const Vec3& start, const Vec3& end,
			const Color& color0, const Color& color1);
		void DrawLineBeforeAlphaPass(const Vec3& start, const Vec3& end,
			const Color& color0, const Color& color1);
		void DrawLine(const Vec2I& start, const Vec2I& end,
			const Color& color, const Color& color1);
		void DrawQuadLater(const Vec2I& pos, const Vec2I& size, const Color& color);
		// with depth culling
		void DrawTexturedThickLine(const Vec3& start, const Vec3& end, const Color& color0, const Color& color1, Real thickness,
			const char* texture, bool textureFlow);
		void DrawSphere(const Vec3& pos, Real radius, const Color& color);
		void DrawBox(const Vec3& boxMin, const Vec3& boxMax, const Color& color, Real alpha);
		void DrawTriangle(const Vec3& a, const Vec3& b, const Vec3& c, const Color& color, Real alpha);
		void DrawTriangleNow(const Vec3& a, const Vec3& b, const Vec3& c, const Vec4& color, MaterialPtr mat);
		void DrawQuad(const Vec2I& pos, const Vec2I& size, const Color& color, bool updateRs = true);
		void DrawQuadLine(const Vec2I& pos, const Vec2I& size, const Color& color);
		void DrawQuadWithTexture(const Vec2I& pos, const Vec2I& size, const Color& color, TexturePtr texture, MaterialPtr materialOverride = 0);
		void DrawQuadWithTextureUV(const Vec2I& pos, const Vec2I& size, const Vec2& uvStart, const Vec2& uvEnd,
			const Color& color, TexturePtr texture, MaterialPtr materialOverride = 0);
		void DrawBillboardWorldQuad(const Vec3& pos, const Vec2& size, const Vec2& offset,
			DWORD color, MaterialPtr pMat);
		
		void RenderGeoms();
		void RenderDebugHud();

		//-------------------------------------------------------------------
		// Internal
		//-------------------------------------------------------------------
		void GatherPointLightData(BoundingVolume* aabb, const Transformation& transform, POINT_LIGHT_CONSTANTS* plConst);
		void RefreshPointLight();
		bool NeedToRefreshPointLight() const;

		//-------------------------------------------------------------------
		// ISceneObserver
		//-------------------------------------------------------------------
		void OnAfterMakeVisibleSet(Scene* scene);
		void OnBeforeRenderingOpaques(Scene* scene);
		void OnBeforeRenderingTransparents(Scene* scene);
	};

}