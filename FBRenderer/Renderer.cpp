#include "stdafx.h"
#include "Renderer.h"
#include "IPlatformRenderer.h"
#include "RendererEnums.h"
#include "RendererStructs.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "RenderResourceFactory.h"
#include "IRenderable.h"
#include "IPlatformTexture.h"
#include "Font.h"
#include "RenderOptions.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Material.h"
#include "TextureAtlas.h"
#include "PointLightManager.h"
#include "FBStringLib/StringConverter.h"
#include "FBStringLib/StringLib.h"
#include "FBCommonHeaders/VectorMap.h"
#include "FBCommonHeaders/Factory.h"
#include "FBSystemLib/ModuleHandler.h"
#include "FBFileSystem/FileSystem.h"
#include "FBSceneManager/Scene.h"
#include "TinyXmlLib/tinyxml2.h"
#include <set>
namespace fastbird{
	ShaderPtr GetShaderFromExistings(IPlatformShaderPtr platformShader);
	TexturePtr GetTextureFromExistings(IPlatformTexturePtr platformTexture);
}
using namespace fastbird;

DECLARE_SMART_PTR(UI3DObj);
DECLARE_SMART_PTR(UIObject);
DECLARE_SMART_PTR(SkySphere);
DECLARE_SMART_PTR(DebugHud);
DECLARE_SMART_PTR(GeometryRenderer);
class Renderer::Impl{
public:
	typedef fastbird::Factory<IPlatformRenderer>::CreateCallback CreateCallback;
	typedef std::vector<RenderTargetWeakPtr> RenderTargets;

	Renderer* mObject;
	std::string mPlatformRendererType;
	IPlatformRendererPtr mPlatformRenderer;
	ModuleHandle mLoadedModule;

	VectorMap<HWindowId, HWindow> mWindowHandles;
	VectorMap<HWindow, HWindowId> mWindowIds;
	VectorMap<HWindowId, Vec2I> mWindowSizes;
	HWindowId mMainWindowId;
	VectorMap<HWindowId, RenderTargetPtr> mWindowRenderTargets;
	RenderTargetPtr mCurrentRenderTarget;	
	Vec2I mCurRTSize;
	std::vector<RenderTargetPtr> mRenderTargetPool;		
	RenderTargets mRenderTargetsEveryFrame;
	RenderTargets mRenderTargets;

	DirectionalLightPtr		mDirectionalLight[2];
	PointLightManagerPtr mPointLightMan;
	DebugHudPtr		mDebugHud;
	GeometryRendererPtr mGeomRenderer;
	VectorMap<int, FontPtr> mFonts;
	MaterialPtr mMaterials[DEFAULT_MATERIALS::COUNT];
	MaterialPtr mMissingMaterial;
	RasterizerStatePtr mDefaultRasterizerState;
	RasterizerStatePtr mFrontFaceCullRS;
	RasterizerStatePtr mOneBiasedDepthRS;
	BlendStatePtr mDefaultBlendState;
	BlendStatePtr mAdditiveBlendState;
	BlendStatePtr mAlphaBlendState;
	BlendStatePtr mMaxBlendState;

	DepthStencilStatePtr mNoDepthStencilState;
	DepthStencilStatePtr mNoDepthWriteLessEqualState;
	DepthStencilStatePtr mLessEqualDepthState;
	bool mLockDepthStencil;
	
	SamplerStatePtr mDefaultSamplers[SAMPLERS::NUM];

	TexturePtr mEnvironmentTexture;
	IPlatformTexturePtr mEnvironmentTextureOverride;

	CameraPtr mCamera;
	RENDERER_FRAME_PROFILER mFrameProfiler;
	ShaderPtr mBindedShader;
	InputLayoutPtr mBindedInputLayout;
	PRIMITIVE_TOPOLOGY mCurrentTopology;

	typedef std::map<INPUT_ELEMENT_DESCS, InputLayoutPtr > INPUTLAYOUT_MAP;
	INPUTLAYOUT_MAP mInputLayouts;


	INPUT_ELEMENT_DESCS mInputLayoutDescs[DEFAULT_INPUTS::COUNT];
	const int DEFAULT_DYN_VERTEX_COUNTS=100;
	VertexBufferPtr mDynVBs[DEFAULT_INPUTS::COUNT];

	ShaderPtr mFullscreenQuadVSNear;
	ShaderPtr mFullscreenQuadVSFar;

	VectorMap<Vec2I, TexturePtr> mTempDepthBuffers;

	// linear sampler
	ShaderPtr mCopyPS;
	ShaderPtr mCopyPSMS;

	// DepthPass Resources
	TexturePtr mDepthTarget;
	ShaderPtr mDepthWriteShader;
	ShaderPtr mCloudDepthWriteShader;
	ShaderPtr mDepthOnlyShader;
	BlendStatePtr mMinBlendState;

	// HDR resources.

	TexturePtr mToneMap[FB_NUM_TONEMAP_TEXTURES_NEW];
	TexturePtr mLuminanceMap[FB_NUM_LUMINANCE_TEXTURES];

	ShaderPtr mSampleLumInitialShader;
	ShaderPtr mSampleLumIterativeShader;
	ShaderPtr mSampleLumFinalShader;
	ShaderPtr mCalcAdaptedLumShader;
	ShaderPtr mToneMappingPS;
	ShaderPtr mBrightPassPS;
	ShaderPtr mBloomPS;

	// 1/4
	// x, y,    offset, weight;
	VectorMap< std::pair<DWORD, DWORD>, std::pair<std::vector<Vec4>, std::vector<Vec4> > > mGauss5x5;

	// Star
	ShaderPtr mBlur5x5;
	ShaderPtr mStarGlareShader;
	ShaderPtr mMergeTexture2;

	// GodRay resources.
	ShaderPtr mOccPrePassShader;
	ShaderPtr mOccPrePassGSShader;
	ShaderPtr mGodRayPS;
	InputLayoutPtr mPositionInputLayout;
	ShaderPtr mGlowPS;


	// for Cloud Volumes;
	TexturePtr mCloudVolumeDepth;
	BlendStatePtr mRedAlphaMaskBlend;

	BlendStatePtr mGreenAlphaMaskBlend;
	BlendStatePtr mGreenAlphaMaskMaxBlend;

	BlendStatePtr mRedAlphaMaskAddMinusBlend;
	BlendStatePtr mGreenAlphaMaskAddAddBlend;

	BlendStatePtr mRedAlphaMaskAddAddBlend;
	BlendStatePtr mGreenAlphaMaskAddMinusBlend;


	BlendStatePtr mGreenMaskBlend;
	BlendStatePtr mBlueMaskBlend;
	BlendStatePtr mNoColorWriteBlend;
	RasterizerStatePtr mRSCloudFar;
	RasterizerStatePtr mRSCloudNear;

	TexturePtr mNoiseMap;

	SkySpherePtr mNextEnvUpdateSkySphere;
	bool mLockBlendState;

	// shadow
	ShaderPtr mShadowMapShader;
	CameraPtr mCameraBackup;


	ShaderPtr mSilouetteShader;

	Real mMiddleGray;
	Real mStarPower;
	Real mBloomPower;

	RenderOptionsPtr mOptions;

	struct DebugRenderTarget
	{
		Vec2 mPos;
		Vec2 mSize;

		TexturePtr mTexture;
	};
	static const unsigned MaxDebugRenderTargets = 4;
	DebugRenderTarget mDebugRenderTargets[MaxDebugRenderTargets];

	bool mRefreshPointLight;
	bool mLuminanceOnCpu;
	bool mUseFilmicToneMapping;
	Real mLuminance;
	unsigned mFrameLuminanceCalced;
	Real mFadeAlpha;

	typedef VectorMap<HWindowId, std::vector<UIObjectPtr> > UI_OBJECTS;
	UI_OBJECTS mUIObjectsToRender;

	typedef VectorMap< std::pair<HWindowId, std::string>, std::vector<UIObjectPtr>> UI_3DOBJECTS;
	UI_3DOBJECTS mUI3DObjects;
	VectorMap<std::string, RenderTargetPtr> mUI3DObjectsRTs;
	VectorMap<std::string, UI3DObjPtr> mUI3DRenderObjs;

	bool m3DUIEnabled;

	// todo: generalize. layer 1~4.
	std::vector<IRenderablePtr> mMarkObjects;
	std::vector<IRenderablePtr> mHPBarObjects;

	std::vector<VideoPlayerPtr> mVideoPlayers;

	ShaderPtr mGGXGenShader;
	TexturePtr mGGXGenTarget;
	TimerPtr mTimer;
	bool mTakeScreenShot;

	VectorMap<SystemTextures::Enum, std::vector< TextureBinding > > mSystemTextureBindings;
	
	//-----------------------------------------------------------------------
	Impl(Renderer* renderer)
		: mObject(renderer)
		, mLoadedModule(0)
	{
		auto& envBindings = mSystemTextureBindings[SystemTextures::Environment];
		envBindings.push_back(TextureBinding{ BINDING_SHADER_PS, 4 });
		auto& depthBindings = mSystemTextureBindings[SystemTextures::Depth];
		depthBindings.push_back(TextureBinding{ BINDING_SHADER_GS, 5 });
		depthBindings.push_back(TextureBinding{ BINDING_SHADER_PS, 5 });
		auto& cloudBindings = mSystemTextureBindings[SystemTextures::CloudVolume];
		cloudBindings.push_back(TextureBinding{BINDING_SHADER_PS, 6});
		auto& noiseBindings = mSystemTextureBindings[SystemTextures::Noise];
		envBindings.push_back(TextureBinding{ BINDING_SHADER_PS, 7 });
		auto& shadowBindings = mSystemTextureBindings[SystemTextures::ShadowMap];
		shadowBindings.push_back(TextureBinding{ BINDING_SHADER_PS, 8 });
		auto& ggxBindings = mSystemTextureBindings[SystemTextures::GGXPrecalc];
		ggxBindings.push_back(TextureBinding{ BINDING_SHADER_PS, 9 });
	}

	void PrepareRenderEngine(const char* type){
		if (!type || strlen(type) == 0){
			Logger::Log(FB_DEFAULT_LOG_ARG, "Cannot prepare a render engine : invalid arg.");
			return;
		}
		if (mLoadedModule){
			Logger::Log(FB_DEFAULT_LOG_ARG, "Render engine is already prepared.");
			return;
		}

		mPlatformRendererType = type;		
		auto module = ModuleHandler::LoadModule(mPlatformRendererType.c_str());
		if (module){
			mLoadedModule = module;
			typedef fastbird::IPlatformRendererPtr(*Create)();
			Create createCallback = (Create)ModuleHandler::GetFunction(module, "CreateRenderEngine");
			if (createCallback){				
				mPlatformRenderer = createCallback();
				Logger::Log(FB_DEFAULT_LOG_ARG, "Render engine %s is prepared.", type);
			}
			else{
				Logger::Log(FB_DEFAULT_LOG_ARG, "Cannot find the entry point 'CreateRenderEngine()'");
			}
		}
	}

	bool InitCanvas(HWindowId id, HWindow window, int width, int height){
		bool mainCanvas = false;
		if (mWindowHandles.empty()){
			mMainWindowId = id;
			mainCanvas = true;
		}

		mWindowHandles[id] = window;
		mWindowIds[window] = id;
		mWindowSizes[id] = { width, height };		
		IPlatformTexturePtr colorTexture;
		IPlatformTexturePtr depthTexture;
		mPlatformRenderer->InitCanvas(id, window, width, height, colorTexture, depthTexture);
		if (colorTexture && depthTexture){
			RenderTargetParam param;
			param.mSize = { width, height };
			param.mWillCreateDepth = true;
			auto rt = CreateRenderTarget(param);
			
			rt->SetColorTexture(CreateTexture(colorTexture));
			rt->SetDepthTexture(CreateTexture(depthTexture));

			if (mainCanvas){
				OnMainCavasCreated();
			}
			return true;
		}
		else{
			Logger::Log(FB_ERROR_LOG_ARG, "Failed to create cavas");
			return false;
		}		
	}

	void OnMainCavasCreated(){
		mMaterials[DEFAULT_MATERIALS::QUAD] = CreateMaterial("es/materials/quad.material");
		mMaterials[DEFAULT_MATERIALS::QUAD_TEXTURE] = CreateMaterial("es/materials/QuadWithTexture.material");
		mMaterials[DEFAULT_MATERIALS::BILLBOARDQUAD] = CreateMaterial("es/materials/billboardQuad.material");

		// POSITION
		{
			INPUT_ELEMENT_DESC desc("POSITION", 0, INPUT_ELEMENT_FORMAT_FLOAT3, 0, 0, INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION].push_back(desc);
		}

		// POSITION_COLOR
		{
			INPUT_ELEMENT_DESC desc[] =
			{
				INPUT_ELEMENT_DESC("POSITION", 0, INPUT_ELEMENT_FORMAT_FLOAT3, 0, 0,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
				INPUT_ELEMENT_DESC("COLOR", 0, INPUT_ELEMENT_FORMAT_UBYTE4, 0, 12,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0)
			};
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_COLOR].push_back(desc[0]);
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_COLOR].push_back(desc[1]);
		}

		// POSITION_COLOR_TEXCOORD
		{
			INPUT_ELEMENT_DESC desc[] =
			{
				INPUT_ELEMENT_DESC("POSITION", 0, INPUT_ELEMENT_FORMAT_FLOAT3, 0, 0,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
				INPUT_ELEMENT_DESC("COLOR", 0, INPUT_ELEMENT_FORMAT_UBYTE4, 0, 12,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
				INPUT_ELEMENT_DESC("TEXCOORD", 0, INPUT_ELEMENT_FORMAT_FLOAT2, 0, 16,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
			};
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_COLOR_TEXCOORD].push_back(desc[0]);
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_COLOR_TEXCOORD].push_back(desc[1]);
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_COLOR_TEXCOORD].push_back(desc[2]);
		}

		// POSITION_HDR_COLOR
		{
			INPUT_ELEMENT_DESC desc[] =
			{
				INPUT_ELEMENT_DESC("POSITION", 0, INPUT_ELEMENT_FORMAT_FLOAT3, 0, 0,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
				INPUT_ELEMENT_DESC("COLOR", 0, INPUT_ELEMENT_FORMAT_FLOAT4, 0, 12,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0)
			};
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_HDR_COLOR].push_back(desc[0]);
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_HDR_COLOR].push_back(desc[1]);
		}

		// POSITION_NORMAL,
		{
			INPUT_ELEMENT_DESC desc[] =
			{
				INPUT_ELEMENT_DESC("POSITION", 0, INPUT_ELEMENT_FORMAT_FLOAT3, 0, 0,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
				INPUT_ELEMENT_DESC("NORMAL", 0, INPUT_ELEMENT_FORMAT_FLOAT3, 0, 12,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0)
			};
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_NORMAL].push_back(desc[0]);
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_NORMAL].push_back(desc[1]);
		}

		//POSITION_TEXCOORD,
		{
			INPUT_ELEMENT_DESC desc[] =
			{
				INPUT_ELEMENT_DESC("POSITION", 0, INPUT_ELEMENT_FORMAT_FLOAT3, 0, 0,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
				INPUT_ELEMENT_DESC("TEXCOORD", 0, INPUT_ELEMENT_FORMAT_FLOAT2, 0, 12,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
			};
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_TEXCOORD].push_back(desc[0]);
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_TEXCOORD].push_back(desc[1]);
		}
		//POSITION_COLOR_TEXCOORD_BLENDINDICES,
		{
			INPUT_ELEMENT_DESC desc[] =
			{
				INPUT_ELEMENT_DESC("POSITION", 0, INPUT_ELEMENT_FORMAT_FLOAT3, 0, 0,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
				INPUT_ELEMENT_DESC("COLOR", 0, INPUT_ELEMENT_FORMAT_UBYTE4, 0, 12,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
				INPUT_ELEMENT_DESC("TEXCOORD", 0, INPUT_ELEMENT_FORMAT_FLOAT2, 0, 16,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
				INPUT_ELEMENT_DESC("BLENDINDICES", 0, INPUT_ELEMENT_FORMAT_UBYTE4, 0, 24,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0)
			};
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_COLOR_TEXCOORD_BLENDINDICES]
				.push_back(desc[0]);
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_COLOR_TEXCOORD_BLENDINDICES]
				.push_back(desc[1]);
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_COLOR_TEXCOORD_BLENDINDICES]
				.push_back(desc[2]);
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_COLOR_TEXCOORD_BLENDINDICES]
				.push_back(desc[3]);
		}

		//POSITION_NORMAL_TEXCOORD,
		{
			INPUT_ELEMENT_DESC desc[] =
			{
				INPUT_ELEMENT_DESC("POSITION", 0, INPUT_ELEMENT_FORMAT_FLOAT3, 0, 0,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
				INPUT_ELEMENT_DESC("NORMAL", 0, INPUT_ELEMENT_FORMAT_FLOAT3, 0, 12,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
				INPUT_ELEMENT_DESC("TEXCOORD", 0, INPUT_ELEMENT_FORMAT_FLOAT2, 0, 24,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
			};
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_NORMAL_TEXCOORD].push_back(desc[0]);
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_NORMAL_TEXCOORD].push_back(desc[1]);
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_NORMAL_TEXCOORD].push_back(desc[2]);
		}

		// POSITION_VEC4,
		{
			INPUT_ELEMENT_DESC desc[] =
			{
				INPUT_ELEMENT_DESC("POSITION", 0, INPUT_ELEMENT_FORMAT_FLOAT3, 0, 0,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
				INPUT_ELEMENT_DESC("TEXCOORD", 0, INPUT_ELEMENT_FORMAT_FLOAT4, 0, 12,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
			};
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_VEC4].push_back(desc[0]);
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_VEC4].push_back(desc[1]);
		}

		// POSITION_VEC4_COLOR,
		{
			INPUT_ELEMENT_DESC desc[] =
			{
				INPUT_ELEMENT_DESC("POSITION", 0, INPUT_ELEMENT_FORMAT_FLOAT3, 0, 0,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
				INPUT_ELEMENT_DESC("TEXCOORD", 0, INPUT_ELEMENT_FORMAT_FLOAT4, 0, 12,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
				INPUT_ELEMENT_DESC("COLOR", 0, INPUT_ELEMENT_FORMAT_UBYTE4, 0, 28,
				INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0),
			};
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_VEC4].push_back(desc[0]);
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_VEC4].push_back(desc[1]);
			mInputLayoutDescs[DEFAULT_INPUTS::POSITION_VEC4].push_back(desc[2]);
		}

		//-----------------------------------------------------------------------
		mDynVBs[DEFAULT_INPUTS::POSITION] = CreateVertexBuffer(0, sizeof(DEFAULT_INPUTS::V_P),
			DEFAULT_DYN_VERTEX_COUNTS, BUFFER_USAGE_DYNAMIC, BUFFER_CPU_ACCESS_WRITE);
		mDynVBs[DEFAULT_INPUTS::POSITION_COLOR] = CreateVertexBuffer(0, sizeof(DEFAULT_INPUTS::V_PC),
			DEFAULT_DYN_VERTEX_COUNTS, BUFFER_USAGE_DYNAMIC, BUFFER_CPU_ACCESS_WRITE);
		mDynVBs[DEFAULT_INPUTS::POSITION_COLOR_TEXCOORD] = CreateVertexBuffer(0, sizeof(DEFAULT_INPUTS::V_PCT),
			DEFAULT_DYN_VERTEX_COUNTS, BUFFER_USAGE_DYNAMIC, BUFFER_CPU_ACCESS_WRITE);
		mDynVBs[DEFAULT_INPUTS::POSITION_NORMAL] = CreateVertexBuffer(0, sizeof(DEFAULT_INPUTS::V_PN),
			DEFAULT_DYN_VERTEX_COUNTS, BUFFER_USAGE_DYNAMIC, BUFFER_CPU_ACCESS_WRITE);
		mDynVBs[DEFAULT_INPUTS::POSITION_TEXCOORD] = CreateVertexBuffer(0, sizeof(DEFAULT_INPUTS::V_PT),
			DEFAULT_DYN_VERTEX_COUNTS, BUFFER_USAGE_DYNAMIC, BUFFER_CPU_ACCESS_WRITE);
		mDynVBs[DEFAULT_INPUTS::POSITION_COLOR_TEXCOORD_BLENDINDICES] =
			CreateVertexBuffer(0, sizeof(DEFAULT_INPUTS::V_PCTB),
			DEFAULT_DYN_VERTEX_COUNTS, BUFFER_USAGE_DYNAMIC, BUFFER_CPU_ACCESS_WRITE);
		mDynVBs[DEFAULT_INPUTS::POSITION_VEC4] = CreateVertexBuffer(0, sizeof(DEFAULT_INPUTS::V_PV4),
			DEFAULT_DYN_VERTEX_COUNTS, BUFFER_USAGE_DYNAMIC, BUFFER_CPU_ACCESS_WRITE);
		mDynVBs[DEFAULT_INPUTS::POSITION_VEC4_COLOR] = CreateVertexBuffer(0, sizeof(DEFAULT_INPUTS::V_PV4C),
			DEFAULT_DYN_VERTEX_COUNTS, BUFFER_USAGE_DYNAMIC, BUFFER_CPU_ACCESS_WRITE);

		//-----------------------------------------------------------------------
		static_assert(DEFAULT_INPUTS::COUNT == 10, "You may not define a new element of mInputLayoutDesc for the new description.");

		LuaObject multiFontSet(gFBEnv->pScriptSystem->GetLuaState(), "r_multiFont");
		if (multiFontSet.IsValid()){
			auto it = multiFontSet.GetSequenceIterator();
			LuaObject data;
			while (it.GetNext(data)){
				auto fontPath = data.GetString();
				if (!fontPath.empty()){
					SmartPtr<IFont> font = FB_NEW(Font);
					auto err = font->Init(fontPath.c_str());
					if (!err){
						font->SetTextEncoding(IFont::UTF16);
						int height = Round(font->GetHeight());
						mFonts[height] = font;
					}
				}
			}
		}
		else{
			SmartPtr<IFont> font = FB_NEW(Font);
			std::string fontPath = gFBEnv->pScriptSystem->GetStringVariable("r_font");
			if (fontPath.empty())
			{
				fontPath = "es/fonts/font22.fnt";
			}
			auto err = font->Init(fontPath.c_str());
			if (!err){
				font->SetTextEncoding(IFont::UTF16);
				int height = Round(font->GetHeight());
				mFonts[height] = font;
			}
		}

		mDebugHud = FB_NEW(DebugHud);
		mGeomRenderer = FB_NEW(GeometryRenderer);

		if (gFBEnv->pConsole)
			gFBEnv->pConsole->Init();

		mDefaultRasterizerState = CreateRasterizerState(RASTERIZER_DESC());
		mDefaultBlendState = CreateBlendState(BLEND_DESC());
		mDefaultDepthStencilState = CreateDepthStencilState(DEPTH_STENCIL_DESC());
		DEPTH_STENCIL_DESC ddesc;
		ddesc.DepthEnable = false;
		ddesc.DepthWriteMask = DEPTH_WRITE_MASK_ZERO;
		mNoDepthStencilState = CreateDepthStencilState(ddesc);


		IMaterial::SHADER_DEFINES emptyShaderDefines;
		mFullscreenQuadVSNear = CreateShader("es/shaders/fullscreenquadvs.hlsl", BINDING_SHADER_VS,
			emptyShaderDefines);
		IMaterial::SHADER_DEFINES shaderDefinesFar;
		shaderDefinesFar.push_back(IMaterial::ShaderDefine("_FAR_SIDE_QUAD", "1"));
		mFullscreenQuadVSFar = CreateShader("es/shaders/fullscreenquadvs.hlsl", BINDING_SHADER_VS,
			shaderDefinesFar);

		mCopyPS = CreateShader("es/shaders/copyps.hlsl", BINDING_SHADER_PS,
			emptyShaderDefines);
		IMaterial::SHADER_DEFINES multiSampleSD;
		multiSampleSD.push_back(IMaterial::ShaderDefine("_MULTI_SAMPLE", "1"));
		mCopyPSMS = CreateShader("es/shaders/copyps.hlsl", BINDING_SHADER_PS,
			multiSampleSD);

		SAMPLER_DESC sdesc;
		sdesc.Filter = TEXTURE_FILTER_MIN_MAG_MIP_POINT;
		mDefaultSamplers[SAMPLERS::POINT] = CreateSamplerState(sdesc);
		sdesc.Filter = TEXTURE_FILTER_MIN_MAG_MIP_LINEAR;
		mDefaultSamplers[SAMPLERS::LINEAR] = CreateSamplerState(sdesc);
		sdesc.Filter = TEXTURE_FILTER_ANISOTROPIC;
		mDefaultSamplers[SAMPLERS::ANISOTROPIC] = CreateSamplerState(sdesc);

		sdesc.Filter = TEXTURE_FILTER_COMPARISON_ANISOTROPIC;
		sdesc.AddressU = TEXTURE_ADDRESS_BORDER;
		sdesc.AddressV = TEXTURE_ADDRESS_BORDER;
		sdesc.AddressW = TEXTURE_ADDRESS_BORDER;
		for (int i = 0; i < 4; i++)
			sdesc.BorderColor[i] = 1.0f;
		sdesc.ComparisonFunc = COMPARISON_LESS_EQUAL;
		mDefaultSamplers[SAMPLERS::SHADOW] = CreateSamplerState(sdesc);

		sdesc.ComparisonFunc = COMPARISON_ALWAYS;
		sdesc.Filter = TEXTURE_FILTER_MIN_MAG_MIP_POINT;
		sdesc.AddressU = TEXTURE_ADDRESS_WRAP;
		sdesc.AddressV = TEXTURE_ADDRESS_WRAP;
		sdesc.AddressW = TEXTURE_ADDRESS_WRAP;
		mDefaultSamplers[SAMPLERS::POINT_WRAP] = CreateSamplerState(sdesc);
		sdesc.Filter = TEXTURE_FILTER_MIN_MAG_MIP_LINEAR;
		mDefaultSamplers[SAMPLERS::LINEAR_WRAP] = CreateSamplerState(sdesc);

		SAMPLER_DESC sdesc_border;
		sdesc_border.Filter = TEXTURE_FILTER_MIN_MAG_MIP_LINEAR;
		sdesc_border.AddressU = TEXTURE_ADDRESS_BORDER;
		sdesc_border.AddressV = TEXTURE_ADDRESS_BORDER;
		sdesc_border.AddressW = TEXTURE_ADDRESS_BORDER;
		for (int i = 0; i < 4; i++)
			sdesc_border.BorderColor[i] = 0;
		mDefaultSamplers[SAMPLERS::BLACK_BORDER] = CreateSamplerState(sdesc_border);
		sdesc_border.Filter = TEXTURE_FILTER_MIN_MAG_MIP_POINT;
		mDefaultSamplers[SAMPLERS::POINT_BLACK_BORDER] = CreateSamplerState(sdesc_border);



		for (int i = 0; i < SAMPLERS::NUM; ++i)
		{
			assert(mDefaultSamplers[i] != 0);
			SetSamplerState((SAMPLERS::Enum)i, BINDING_SHADER_PS, i);
		}

		SetSamplerState(SAMPLERS::POINT, BINDING_SHADER_VS, SAMPLERS::POINT);

		mMiddleGray = gFBEnv->pConsole->GetEngineCommand()->r_HDRMiddleGray;
		mStarPower = gFBEnv->pConsole->GetEngineCommand()->r_StarPower;
		mBloomPower = gFBEnv->pConsole->GetEngineCommand()->r_BloomPower;

		SkySphere::CreateSharedEnvRT();

		UpdateRareConstantsBuffer();

		const auto& rtSize = GetMainRTSize();
		for (auto it : mFonts){
			it.second->SetRenderTargetSize(rtSize);
		}
		if (mDebugHud){
			mDebugHud->SetRenderTargetSize(rtSize);
		}
		if (mGeomRenderer){
			mGeomRenderer->SetRenderTargetSize(rtSize);
		}

		if (gFBEnv->pConsole){
			gFBEnv->pConsole->SetRenderTargetSize(rtSize);
		}
	}

	void ReleaseCanvas(HWindowId id){
		auto it = mWindowHandles.Find(id);
		if (it == mWindowHandles.end()){
			Logger::Log(FB_ERROR_LOG_ARG, "Cannot find the window.");
			return;
		}
	}

	void SetTimer(TimerPtr timer){
		mTimer = timer;
	}

	TimerPtr GetTimer() const{
		return mTimer;
	}

	//-------------------------------------------------------------------
	// Resource Creation
	//-------------------------------------------------------------------
	RenderTargetPtr CreateRenderTarget(const RenderTargetParam& param){
		if (param.mUsePool){
			for (auto it = mRenderTargetPool.begin(); it != mRenderTargetPool.end(); it++)
			{
				if ((*it)->CheckOptions(param))
				{
					if (param.mEveryFrame)
						mRenderTargets.push_back(*it);
					auto rt = *it;
					mRenderTargetPool.erase(it);
					return rt;
				}
			}
		}

		auto rt = RenderResourceFactory::CreateResource<RenderTarget>();
		mRenderTargets.push_back(rt);
		rt->SetColorTextureDesc(param.mSize.x, param.mSize.y, param.mPixelFormat, param.mShaderResourceView,
				param.mMipmap, param.mCubemap);
		
		rt->SetUsePool(param.mUsePool);
		if (param.mEveryFrame)
		{
			mRenderTargetsEveryFrame.push_back(rt);
			return rt;
		}
		else
		{
			mRenderTargets.push_back(rt);
			return rt;
		}
		return rt;
	}

	void KeepRenderTargetInPool(RenderTargetPtr rt){
		if (!ValueExistsInVector(mRenderTargetPool, rt)){
			mRenderTargetPool.push_back(rt);
		}
	}

	static VectorMap<std::string, IPlatformTextureWeakPtr> sPlatformTextures;
	TexturePtr CreateTexture(const char* file, bool async){
		if (!ValidCString(file)){
			Logger::Log(FB_ERROR_LOG_ARG, "Invalid arg.");
			return 0;
		}
		std::string loweredFilepath(file);
		ToLowerCase(loweredFilepath);
		auto it = sPlatformTextures.Find(loweredFilepath);
		if (it != sPlatformTextures.end()){
			auto platformTexture = it->second.lock();
			if (platformTexture){
				auto texture = GetTextureFromExistings(platformTexture);
				if (texture){
					return texture;
				}
			}
		}

		IPlatformTexturePtr platformTexture = mPlatformRenderer->CreateTexture(file, async);
		if (!platformTexture){
			Logger::Log(FB_ERROR_LOG_ARG, FormatString("Platform renderer failed to load a texture(%s)", file).c_str());
			return 0;
		}
		sPlatformTextures[loweredFilepath] = platformTexture;
		return CreateTexture(platformTexture);
	}

	TexturePtr CreateTexture(void* data, int width, int height, PIXEL_FORMAT format,
		BUFFER_USAGE usage, int  buffer_cpu_access, int texture_type){
		auto platformTexture = mPlatformRenderer->CreateTexture(data, width, height, format, usage, buffer_cpu_access, texture_type);
		if (!platformTexture){
			Logger::Log(FB_ERROR_LOG_ARG, "Failed to create texture with data.");
			return 0;
		}
		auto texture = RenderResourceFactory::CreateResource<Texture>();
		texture->SetPlatformTexture(platformTexture);
		return texture;
	}

	TexturePtr CreateTexture(IPlatformTexturePtr platformTexture){
		auto texture = RenderResourceFactory::CreateResource<Texture>();
		texture->SetPlatformTexture(platformTexture);
		return texture;
	}

	VertexBufferPtr CreateVertexBuffer(void* data, unsigned stride,
		unsigned numVertices, BUFFER_USAGE usage, BUFFER_CPU_ACCESS_FLAG accessFlag) {
		auto platformBuffer = mPlatformRenderer->CreateVertexBuffer(data, stride, numVertices, usage, accessFlag);
		if (!platformBuffer){
			Logger::Log(FB_ERROR_LOG_ARG, "Platform renderer failed to create a vertex buffer");
			return 0;
		}
		auto vertexBuffer = RenderResourceFactory::CreateResource<VertexBuffer>();
		vertexBuffer->SetPlatformBuffer(platformBuffer);
		return vertexBuffer;
	}

	IndexBufferPtr CreateIndexBuffer(void* data, unsigned int numIndices,
		INDEXBUFFER_FORMAT format) {
		auto platformBuffer = mPlatformRenderer->CreateIndexBuffer(data, numIndices, format);
		if (!platformBuffer){
			Logger::Log(FB_ERROR_LOG_ARG, "Platform renderer failed to create a index buffer");
			return 0;
		}
		auto indexBuffer = RenderResourceFactory::CreateResource<IndexBuffer>();
		indexBuffer->SetPlatformBuffer(platformBuffer);
		return indexBuffer;
	}

	struct ShaderCreationInfo{
		ShaderCreationInfo(const char* path, int shaders, const SHADER_DEFINES& defines)
			: mFilepath(path)
			, mShaders(shaders)
			, mDefines(defines)
		{
			ToLowerCase(mFilepath);
			std::sort(mDefines.begin(), mDefines.end());
		}
		bool operator < (const ShaderCreationInfo& other) const{			
			if (mShaders < other.mShaders)
				return true;
			else if (mShaders == other.mShaders){
				if (mFilepath < other.mFilepath)
					return true;
				else if (mFilepath == other.mFilepath){
					return mDefines < other.mDefines;
				}
			}
		}
		int mShaders;
		std::string mFilepath;		
		SHADER_DEFINES mDefines;
	};
	VectorMap<ShaderCreationInfo, IPlatformShaderWeakPtr> sPlatformShaders;
	ShaderPtr CreateShader(const char* filepath, int shaders,
		const SHADER_DEFINES& defines) {
		SHADER_DEFINES sortedDefines(defines);
		std::sort(sortedDefines.begin(), sortedDefines.end());
		auto key = ShaderCreationInfo(filepath, shaders, defines);
		auto it = sPlatformShaders.Find(key);
		if (it != sPlatformShaders.end()){
			auto platformShader = it->second.lock();
			if (platformShader){
				auto shader = GetShaderFromExistings(platformShader);
				if (shader){
#if defined(_DEBUG)				
					if (shader->GetShaderDefines() != sortedDefines){
						Logger::Log(FB_ERROR_LOG_ARG, FormatString("Platform shader is same but define is different for shader file(%s)", filepath).c_str());
					}
#endif
					return shader;
				}
				else{
					auto shader = RenderResourceFactory::CreateResource<Shader>();
					shader->SetPlatformShader(platformShader);
					shader->SetShaderDefines(sortedDefines);
					return shader;
				}
			}
		}

		auto platformShader = mPlatformRenderer->CreateShader(filepath, shaders, sortedDefines);
		auto shader = RenderResourceFactory::CreateResource<Shader>();
		shader->SetPlatformShader(platformShader);
		shader->SetShaderDefines(sortedDefines);
		sPlatformShaders[key] = platformShader;
		return shader;
	}

	static VectorMap<std::string, MaterialWeakPtr> sLoadedMaterials;
	MaterialPtr CreateMaterial(const char* file){
		if (!ValidCString(file)){
			Logger::Log(FB_ERROR_LOG_ARG, "Invalid arg.");
			return 0;
		}
		std::string loweredPath(file);
		ToLowerCase(loweredPath);
		auto it = sLoadedMaterials.Find(loweredPath);
		if (it != sLoadedMaterials.end()){
			auto material = it->second.lock();
			if (material){
				return material->Clone();
			}
		}
		auto material = RenderResourceFactory::CreateResource<Material>();
		if (!material->LoadFromFile(file))
			return 0;

		sLoadedMaterials[loweredPath] = material;
		return material->Clone();

	}

	MaterialPtr GetMaterial(DEFAULT_MATERIALS::Enum type){
		assert(type < DEFAULT_MATERIALS::COUNT);
		return mMaterials[type];
	}

	MaterialPtr GetMissingMaterial(){
		static bool loaded = false;
		if (!loaded)
		{
			loaded = true;
			mMissingMaterial = CreateMaterial("es/materials/missing.material");
			if (!mMissingMaterial)
			{
				Logger::Log(FB_ERROR_LOG_ARG, "Fall-back material not found!");
			}
		}

		return mMissingMaterial;
	}
	
	static VectorMap<INPUT_ELEMENT_DESCS, InputLayoutWeakPtr> sInputLayouts;
	// use this if you are sure there is instance of the descs.
	InputLayoutPtr CreateInputLayout(const INPUT_ELEMENT_DESCS& descs, ShaderPtr shader){
		unsigned size;
		void* data = shader->GetVSByteCode(size);
		if (!data){
			Logger::Log(FB_ERROR_LOG_ARG, "Invalid shader");
			return 0;
		}

		auto it = sInputLayouts.Find(descs);
		if (it != sInputLayouts.end()){
			auto inputLayout = it->second.lock();
			if (inputLayout)
				return inputLayout;
		}
		auto inputLayout = mPlatformRenderer->CreateInputLayout(descs, data, size);
		sInputLayouts[descs] = inputLayout;
		return inputLayout;
	}

	InputLayoutPtr GetInputLayout(DEFAULT_INPUTS::Enum e, ShaderPtr shader){
		const auto& desc = GetInputElementDesc(e);
		return CreateInputLayout(desc, shader);
	}

	static VectorMap<RASTERIZER_DESC, RasterizerStateWeakPtr> sRasterizerStates;
	RasterizerStatePtr CreateRasterizerState(const RASTERIZER_DESC& desc){
		auto it = sRasterizerStates.Find(desc);
		if (it != sRasterizerStates.end()){
			auto state = it->second.lock();
			if (state){
				return state;
			}
		}
		auto raster = mPlatformRenderer->CreateRasterizerState(desc);
		sRasterizerStates[desc] = raster;
		return raster;
	}

	static VectorMap<BLEND_DESC, BlendStateWeakPtr> sBlendStates;
	BlendStatePtr CreateBlendState(const BLEND_DESC& desc){		
		auto it = sBlendStates.Find(desc);
		if (it != sBlendStates.end()){
			auto state = it->second.lock();
			if (state){
				return state;
			}
		}
		auto state = mPlatformRenderer->CreateBlendState(desc);
		sBlendStates[desc] = state;
		return state;
		
	}

	static VectorMap<DEPTH_STENCIL_DESC, DepthStencilStateWeakPtr> sDepthStates;
	DepthStencilStatePtr CreateDepthStencilState(const DEPTH_STENCIL_DESC& desc){
		auto it = sDepthStates.Find(desc);
		if (it != sDepthStates.end()){
			auto state = it->second.lock();
			if (state){
				return state;
			}
		}
		auto state = mPlatformRenderer->CreateDepthStencilState(desc);
		sDepthStates[desc] = state;
		return state;
	}

	static VectorMap<SAMPLER_DESC, SamplerStateWeakPtr> sSamplerStates;
	SamplerStatePtr CreateSamplerState(const SAMPLER_DESC& desc){
		auto it = sSamplerStates.Find(desc);
		if (it != sSamplerStates.end()){
			auto state = it->second.lock();
			if (state){
				return state;
			}
		}
		auto state = mPlatformRenderer->CreateSamplerState(desc);
		sSamplerStates[desc] = state;
		return state;

	}

	// holding strong pointer
	static VectorMap<std::string, TextureAtlasPtr> sTextureAtlas;
	TextureAtlasPtr GetTextureAtlas(const char* path){
		std::string filepath(path);
		ToLowerCase(filepath);
		auto it = sTextureAtlas.Find(filepath);
		if (it != sTextureAtlas.end()){
			return it->second;
		}

		tinyxml2::XMLDocument doc;
		doc.LoadFile(filepath.c_str());
		if (doc.Error())
		{
			const char* errMsg = doc.GetErrorStr1();
			if (ValidCString(errMsg)){
				Logger::Log(FB_ERROR_LOG_ARG, FormatString("%s(%s)", errMsg, path));
			}
			else{
				Logger::Log(FB_ERROR_LOG_ARG, FormatString("Cannot load texture atlas(%s)", path));
			}
			return 0;
		}

		tinyxml2::XMLElement* pRoot = doc.FirstChildElement("TextureAtlas");
		if (!pRoot)
		{
			Logger::Log(FB_ERROR_LOG_ARG, FormatString("Invalid Texture Atlas(%s)", path).c_str());
			return 0;
		}

		const char* szBuffer = pRoot->Attribute("file");
		TextureAtlasPtr pTextureAtlas;
		if (szBuffer)
		{			
			pTextureAtlas = RenderResourceFactory::CreateResource<TextureAtlas>();
			pTextureAtlas->SetPath(filepath.c_str());			
			pTextureAtlas->SetTexture(CreateTexture(szBuffer, true));
			sTextureAtlas[filepath] = pTextureAtlas;		
			if (!pTextureAtlas->GetTexture())
			{
				Logger::Log(FB_ERROR_LOG_ARG, FormatString("Texture for atlas(%s) is not found", szBuffer).c_str());
				return pTextureAtlas;
			}
		}
		else
		{
			Logger::Log(FB_ERROR_LOG_ARG, "Invalid TextureAtlas format! No Texture Defined.");
			return 0;
		}

		auto texture = pTextureAtlas->GetTexture();
		Vec2I textureSize = texture->GetSize();
		if (textureSize.x != 0 && textureSize.y != 0)
		{
			tinyxml2::XMLElement* pRegionElem = pRoot->FirstChildElement("region");
			while (pRegionElem)
			{
				szBuffer = pRegionElem->Attribute("name");
				if (!szBuffer)
				{
					Logger::Log(FB_DEFAULT_LOG_ARG, "No name for texture atlas region");
					continue;
				}

				auto pRegion = pTextureAtlas->AddRegion(szBuffer);				
				pRegion->mID = pRegionElem->UnsignedAttribute("id");
				pRegion->mStart.x = pRegionElem->IntAttribute("x");
				pRegion->mStart.y = pRegionElem->IntAttribute("y");
				pRegion->mSize.x = pRegionElem->IntAttribute("width");
				pRegion->mSize.y = pRegionElem->IntAttribute("height");
				Vec2 start((float)pRegion->mStart.x, (float)pRegion->mStart.y);
				Vec2 end(start.x + pRegion->mSize.x, start.y + pRegion->mSize.y);
				pRegion->mUVStart = start / textureSize;
				pRegion->mUVEnd = end / textureSize;
				pRegionElem = pRegionElem->NextSiblingElement();
			}
		}
		else
		{
			Logger::Log(FB_ERROR_LOG_ARG, "Texture size is 0, 0.");
		}

		return pTextureAtlas;
	}

	TextureAtlasRegionPtr GetTextureAtlasRegion(const char* path, const char* region){
		auto pTextureAtlas = GetTextureAtlas(path);
		if (pTextureAtlas)
		{
			return pTextureAtlas->GetRegion(region);
		}

		return 0;
	}

	TexturePtr GetTemporalDepthBuffer(const Vec2I& size){
		auto it = mTempDepthBuffers.Find(size);
		if (it == mTempDepthBuffers.end())
		{
			auto depthBuffer = CreateTexture(0, size.x, size.y, PIXEL_FORMAT_D32_FLOAT, BUFFER_USAGE_DEFAULT,
				BUFFER_CPU_ACCESS_NONE, TEXTURE_TYPE_DEPTH_STENCIL);
			mTempDepthBuffers.Insert(std::make_pair(size, depthBuffer));
			return depthBuffer;
		}
		return it->second;
	}

	PointLightPtr CreatePointLight(const Vec3& pos, Real range, const Vec3& color, Real intensity, Real lifeTime,
		bool manualDeletion){
		assert(mPointLightMan);
		RefreshPointLight();
		return mPointLightMan->CreatePointLight(pos, range, color, intensity, lifeTime, manualDeletion);
	}

	void RefreshPointLight()
	{
		mRefreshPointLight = true;
	}
	

	//-------------------------------------------------------------------
	// Resource Bindings
	//-------------------------------------------------------------------
	void SetRenderTarget(TexturePtr pRenderTargets[], size_t rtViewIndex[], int num,
		TexturePtr pDepthStencil, size_t dsViewIndex){
		static float time = 0;
		static std::set<TextureWeakPtr> usedRenderTargets;
		if (GetOptions()->r_numRenderTargets && mTimer)
		{
			for (int i = 0; i<num; i++)
			{
				usedRenderTargets.insert(pRenderTargets[i]);
			}
			if (mTimer->GetTime() - time > 5)
			{
				time = mTimer->GetTime();
				Logger::Log(FB_DEFAULT_LOG_ARG, FormatString("used RenderTargets", usedRenderTargets.size()).c_str());				
			}
		}
		if (pRenderTargets && num>0 && pRenderTargets[0])
		{
			mCurRTSize = pRenderTargets[0]->GetSize();
		}
		else
		{
			mCurRTSize = GetMainRTSize();
		}
		for (auto it : mFonts){
			it.second->SetRenderTargetSize(mCurRTSize);
		}

		UpdateRenderTargetConstantsBuffer();
	}

	void SetViewports(const Viewport viewports[], int num){
		mPlatformRenderer->SetViewports(viewports, num);
	}

	void SetScissorRects(const Rect rects[], int num){
		mPlatformRenderer->SetScissorRects(rects, num);
	}

	void SetVertexBuffer(unsigned int startSlot, unsigned int numBuffers,
		VertexBufferPtr pVertexBuffers[], unsigned int strides[], unsigned int offsets[]) {
		static const unsigned int numMaxVertexInputSlot = 32; //D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT (32)
		IPlatformVertexBufferPtr platformBuffers[numMaxVertexInputSlot];
		numBuffers = std::min(numMaxVertexInputSlot, numBuffers);		
		for (int i = 0; i < numBuffers; ++i){
			platformBuffers[i] = pVertexBuffers[i]->GetPlatformBuffer();
		}
		mPlatformRenderer->SetVertexBuffer(startSlot, numBuffers, platformBuffers, strides, offsets);
	}

	void SetPrimitiveTopology(PRIMITIVE_TOPOLOGY pt){
		mPlatformRenderer->SetPrimitiveTopology(pt);
	}

	void SetInputLayout(InputLayoutPtr pInputLayout){
		mPlatformRenderer->SetInputLayout(pInputLayout);
	}

	void SetIndexBuffer(IndexBufferPtr pIndexBuffer){
		mPlatformRenderer->SetIndexBuffer(pIndexBuffer->GetPlatformBuffer());
	}

	void SetShaders(ShaderPtr pShader){
		mPlatformRenderer->SetShaders(pShader->GetPlatformShader());
	}
	
	void SetTexture(TexturePtr pTexture, BINDING_SHADER shaderType, unsigned int slot){
		auto texture = pTexture->GetPlatformTexture();
		mPlatformRenderer->SetTexture(texture, shaderType, slot);
	}

	void SetTextures(TexturePtr pTextures[], int num, BINDING_SHADER shaderType, int startSlot){
		static const int maxBindableTextures = 20; // D3D11_REQ_RESOURCE_VIEW_COUNT_PER_DEVICE_2_TO_EXP(20)
		IPlatformTexturePtr textures[maxBindableTextures];
		num = std::min(num, maxBindableTextures);
		for (int i = 0; i < num; ++i){
			textures[i] = pTextures[i]->GetPlatformTexture();
		}
		mPlatformRenderer->SetTextures(textures, num, shaderType, startSlot);
	}

	void SetSystemTexture(SystemTextures::Enum type, TexturePtr texture){
		auto it = mSystemTextureBindings.Find(type);
		if (it == mSystemTextureBindings.end()){
			Logger::Log(FB_ERROR_LOG_ARG, FormatString("Cannot find the binding information for the system texture(%d)", type).c_str());
			return;
		}
		for (const auto& binding : it->second){
			SetTexture(texture, binding.mShader, binding.mSlot);
		}
	}

	void BindDepthTexture(bool set){
		auto mainRt = GetMainRenderTarget();
		if (mainRt){
			mainRt->BindDepthTexture(set);
		}
	}

	void SetDepthWriteShader(){
		if (!mDepthWriteShader)
		{
			mDepthWriteShader = CreateShader("es/shaders/depth.hlsl", BINDING_SHADER_VS | BINDING_SHADER_PS,
				IMaterial::SHADER_DEFINES());
			if (!mPositionInputLayout)
				mPositionInputLayout = GetInputLayout(DEFAULT_INPUTS::POSITION, mDepthWriteShader);
		}
		mPositionInputLayout->Bind();
		mDepthWriteShader->Bind();
	}
	void SetDepthWriteShaderCloud(){
		
	}
	void SetOccPreShader(){
		
	}
	void SetOccPreGSShader(){
		
	}
	void SetPositionInputLayout(){
		
	}

	void SetSystemTextureBindings(SystemTextures::Enum type, const TextureBindings& bindings){
		mSystemTextureBindings[type] = bindings;
	}

	const TextureBindings& GetSystemTextureBindings(SystemTextures::Enum type){
		auto it = mSystemTextureBindings.Find(type);
		if (it != mSystemTextureBindings.end())
			return it->second;
		static TextureBindings noBindingInfo;
		return noBindingInfo;
	}

	ShaderPtr GetGodRayPS(){
		
	}
	ShaderPtr GetGlowShader(){
		
	}
	void SetShadowMapShader(){
		
	}
	ShaderPtr GetSilouetteShader(){
		
	}
	ShaderPtr GetCopyPS(){
		
	}
	ShaderPtr GetCopyPSMS(){
		
	}
	TexturePtr GetToneMap(unsigned idx){
		
	}
	ShaderPtr GetSampleLumInitialShader(){
		
	}
	ShaderPtr GetSampleLumIterativeShader(){
		
	}
	ShaderPtr GetSampleLumFinalShader(){
		
	}
	void SwapLuminanceMap(){
		
	}
	TexturePtr GetLuminanceMap(unsigned idx){
		
	}
	ShaderPtr GetCalcAdapedLumShader(){
		
	}
	ShaderPtr GetBrightPassPS(){
		
	}
	ShaderPtr GetBlur5x5PS(){
		
	}
	ShaderPtr GetBloomPS(){
		
	}
	ShaderPtr GetStarGlareShader(){
		
	}
	ShaderPtr GetMergeTexturePS(){
		
	}
	ShaderPtr GetToneMappingPS(){
		
	}

	//-------------------------------------------------------------------
	// Device RenderStates
	//-------------------------------------------------------------------
	void RestoreRenderStates(){
		
	}
	void RestoreRasterizerState(){
		
	}
	void RestoreBlendState(){
		
	}
	void RestoreDepthStencilState(){
		
	}
	void LockDepthStencilState(){
		
	}
	void UnlockDepthStencilState(){
		
	}
	void LockBlendState(){
		
	}
	void UnlockBlendState(){
		
	}
	// blend
	void SetAlphaBlendState(){
		
	}
	void SetAdditiveBlendState(){
		
	}
	void SetMaxBlendState(){
		
	}
	void SetRedAlphaMask(){
		
	}
	void SetGreenAlphaMask(){
		
	}
	void SetGreenMask(){
		
	}
	void SetBlueMask(){
		
	}
	void SetGreenAlphaMaskMax(){
		
	}
	void SetGreenAlphaMaskAddAddBlend(){
		
	}
	void SetRedAlphaMaskAddMinusBlend(){
		
	}
	void SetGreenAlphaMaskAddMinusBlend(){
		
	}
	void SetRedAlphaMaskAddAddBlend(){
		
	}
	// depth
	void SetNoDepthWriteLessEqual(){
		
	}
	void SetLessEqualDepth(){
		
	}
	void SetNoDepthStencil(){
		
	}
	// raster
	void SetFrontFaceCullRS(){
		
	}
	// sampler
	void SetSamplerState(SAMPLERS::Enum s, BINDING_SHADER shader, int slot){
		
	}

	//-------------------------------------------------------------------
	// Resource Manipulation
	//-------------------------------------------------------------------
	MapData MapVertexBuffer(VertexBufferPtr pBuffer, UINT subResource,
		MAP_TYPE type, MAP_FLAG flag){
		
	}
	void UnmapVertexBuffer(VertexBufferPtr pBuffer, unsigned int subResource){
		
	}
	MapData MapTexture(TexturePtr pTexture, UINT subResource,
		MAP_TYPE type, MAP_FLAG flag){
		
	}
	void UnmapTexture(TexturePtr pTexture, UINT subResource){
		
	}
	void CopyToStaging(TexturePtr dst, UINT dstSubresource, UINT dstx, UINT dsty, UINT dstz,
		TexturePtr src, UINT srcSubresource, Box3D* pBox){
		
	}
	void SaveTextureToFile(TexturePtr texture, const char* filename){
		
	}


	//-------------------------------------------------------------------
	// GPU constants
	//-------------------------------------------------------------------
	void UpdateObjectConstantsBuffer(const void* pData, bool record = false){
		
	}
	void UpdatePointLightConstantsBuffer(const void* pData){
		
	}
	void UpdateFrameConstantsBuffer(){
		
	}
	void UpdateMaterialConstantsBuffer(const void* pData){
		
	}
	void UpdateCameraConstantsBuffer(){
		
	}
	void UpdateRenderTargetConstantsBuffer(){
		
	}
	void UpdateSceneConstantsBuffer(){
		
	}
	void UpdateRareConstantsBuffer(){
		
	}
	void UpdateRadConstantsBuffer(const void* pData){
		
	}
	void* MapMaterialParameterBuffer(){
		
	}
	void UnmapMaterialParameterBuffer(){
		
	}
	void* MapBigBuffer(){
		
	}
	void UnmapBigBuffer(){
		
	}
	//-------------------------------------------------------------------
	// GPU Manipulation
	//-------------------------------------------------------------------
	void DrawIndexed(unsigned indexCount, unsigned startIndexLocation, unsigned startVertexLocation){
		mPlatformRenderer->DrawIndexed(indexCount, startIndexLocation, startVertexLocation);
	}

	void Draw(unsigned int vertexCount, unsigned int startVertexLocation){
		mPlatformRenderer->Draw(vertexCount, startVertexLocation);
	}

	void SetClearColor(HWindowId id, const Color& color){
		auto it = mWindowRenderTargets.Find(id);
		if (it == mWindowRenderTargets.end()){
			Logger::Log(FB_ERROR_LOG_ARG, "Cannot find the render target.");
			return;
		}
		it->second->SetClearColor(color);
	}

	void SetClearDepthStencil(HWindowId id, Real z, UINT8 stencil){
		auto it = mWindowRenderTargets.Find(id);
		if (it == mWindowRenderTargets.end()){
			Logger::Log(FB_ERROR_LOG_ARG, "Cannot find the render target.");
			return;
		}
		it->second->SetClearDepthStencil(z, stencil);
	}

	void Clear(Real r, Real g, Real b, Real a, Real z, UINT8 stencil){
		mPlatformRenderer->Clear(r, g, b, a, z, stencil);
	}

	void Clear(Real r, Real g, Real b, Real a){
		mPlatformRenderer->Clear(r, g, b, a);
	}

	// Avoid to use
	void ClearState(){
		mPlatformRenderer->ClearState();
	}

	void BeginEvent(const char* name){
		mPlatformRenderer->BeginEvent(name);
	}

	void EndEvent(){
		mPlatformRenderer->EndEvent();
	}

	void TakeScreenshot(){
		auto filepath = GetNextScreenshotFile();
		mPlatformRenderer->TakeScreenshot(filepath.c_str());
	}

	std::string GetScreenhotFolder(){
		auto appData = FileSystem::GetAppDataFolder();
		const char* screenShotFolder = "./ScreenShot/";
		auto screenShotFolderFull = FileSystem::ConcatPath(appData.c_str(), screenShotFolder);
		return screenShotFolderFull;
	}
	std::string GetNextScreenshotFile(){
		auto screenShotFolder = GetScreenhotFolder();
		if (!FileSystem::Exists(screenShotFolder.c_str())){
			bool created = FileSystem::CreateDirectory(screenShotFolder.c_str());
			if (!created){
				Logger::Log(FB_ERROR_LOG_ARG, FormatString("Failed to create folder %s", screenShotFolder.c_str()).c_str());
				return "";
			}
		}
		auto it = FileSystem::GetDirectoryIterator(screenShotFolder.c_str(), false);
		
		while (it->HasNext()){
			const char* filename = it->GetNextFileName();
		}
	

		unsigned n = 0;		
		while (it->HasNext())
		{
			const char* filename = it->GetNextFileName();
			std::regex match("screenshot_([0-9]+)\\.bmp");
			std::smatch result;			
			if (std::regex_match(std::string(filename), result, match)){
				if (result.size() == 2){
					std::ssub_match subMatch = result[1];
					std::string matchNumber = subMatch.str();
					unsigned thisn = StringConverter::ParseUnsignedInt(matchNumber);
					if (thisn >= n){
						n = thisn + 1;
					}
				}
			}			
		}
		return FormatString("Screenshot/screenshot_%d.bmp", n);
	}

	//-------------------------------------------------------------------
	// Renderer State
	//-------------------------------------------------------------------
	void SetWireframe(bool enable){
		mPlatformRenderer->SetWireframe(enable);
	}

	bool GetWireframe() const{
		return mPlatformRenderer->GetWireframe();
	}

	RenderTargetPtr GetMainRenderTarget() const{
		auto it = mWindowRenderTargets.Find(mMainWindowId);
		if (it == mWindowRenderTargets.end()){
			Logger::Log(FB_ERROR_LOG_ARG, "No main window render target found.");
			return 0;
		}
		return it->second;
	}

	ScenePtr GetMainScene() const{
		auto rt = GetMainRenderTarget();
		if (rt){
			return rt->GetScene();
		}
		return 0;
	}
 // move to SceneManager
	const Vec2I& GetMainRTSize() const{
		auto rt = GetMainRenderTarget();
		if (rt)
		{
			return rt->GetSize();
		}
		return Vec2I::ZERO;
	}

	void SetCurrentRenderTarget(RenderTargetPtr renderTarget){
		mCurrentRenderTarget = renderTarget;
	}

	RenderTargetPtr GetCurrentRenderTarget() const{
		return mCurrentRenderTarget;
	}

	bool IsMainRenderTarget() const{
		return GetMainRenderTarget() == mCurrentRenderTarget;
	}

	const Vec2I& GetRenderTargetSize(HWindowId id = INVALID_HWND_ID) const{
		
		if (id != INVALID_HWND_ID){
			auto rt = GetRenderTarget(id);
			if (rt){
				return rt->GetSize();
			}
		}

		return mCurRTSize;
	}

	const Vec2I& GetRenderTargetSize(HWindow hwnd = 0) const{
		RenderTargetPtr rt = GetRenderTarget(hwnd);
		if (rt){
			return rt->GetSize();
		}
		return mCurRTSize;
	}

	

	void SetDirectionalLight(DirectionalLightPtr pLight, int idx){
		if (!(idx >= 0 && idx <= 1))
			return;
		mDirectionalLight[idx] = pLight;
	}

	DirectionalLightPtr GetDirectionalLight(int idx) const{
		if (!(idx >= 0 && idx <= 1))
			return 0;
		return mDirectionalLight[idx];
	}

	DirectionalLightPtr GetMainDirectionalLight(int idx) const{
		auto mainRT = GetMainRenderTarget();
		auto scene = mainRT ? mainRT->GetScene() : 0;
		if (scene){
			return scene->GetLight(idx);
		}

		return 0;
	}

	void InitFrameProfiler(Real dt){
		mFrameProfiler.Clear();
		mFrameProfiler.UpdateFrameRate(dt);
	}

	const RENDERER_FRAME_PROFILER& GetFrameProfiler() const{
		return mFrameProfiler;
	}

	inline FontPtr GetFont(Real fontHeight) const{
		if (mFonts.empty()){
			return 0;
		}

		if (mFonts.size() == 1){
			auto it = mFonts.begin();
			it->second->SetHeight(fontHeight);
			return it->second;
		}

		int requestedHeight = Round(fontHeight);
		int bestMatchHeight = mFonts.begin()->first;
		int curGap = std::abs(requestedHeight - bestMatchHeight);
		FontPtr bestFont = mFonts.begin()->second;
		for (auto it : mFonts){
			auto newGap = std::abs(requestedHeight - it.first);
			if (newGap < curGap){
				bestMatchHeight = it.first;
				curGap = newGap;
				bestFont = it.second;
			}
		}
		if (!bestFont){
			Logger::Log(FB_ERROR_LOG_ARG, FormatString("Font not found with size %f", fontHeight).c_str());			
		}
		else{
			bestFont->SetHeight(fontHeight);
		}
		return bestFont;
	}

	const INPUT_ELEMENT_DESCS& GetInputElementDesc(DEFAULT_INPUTS::Enum e){
		return mInputLayoutDescs[e];
	}

	void SetEnvironmentTexture(TexturePtr pTexture){
		mEnvironmentTexture = pTexture;
		mEnvironmentTexture->SetShaderStage(BINDING_SHADER_PS);
		mEnvironmentTexture->SetSlot(4); // hardcoded environment slot.
		mEnvironmentTexture->Bind();
	}

	void SetEnvironmentTextureOverride(TexturePtr texture){
	
	}

	void SetDebugRenderTarget(unsigned idx, const char* textureName){
	
	}

	void SetFadeAlpha(Real alpha){
	
	}

	PointLightManPtr GetPointLightMan() const{
	
	}

	void RegisterVideoPlayer(VideoPlayerPtr player){
	
	}

	void UnregisterVideoPlayer(VideoPlayerPtr player){
	
	}

	void GetSampleOffsets_GaussBlur5x5(DWORD texWidth, DWORD texHeight, Vec4** avTexCoordOffset, Vec4** avSampleWeight, Real fMultiplier){
	
	}

	void GetSampleOffsets_DownScale2x2(DWORD texWidth, DWORD texHeight, Vec4* avTexCoordOffset){
	
	}

	bool IsLuminanceOnCpu() const{
	
	}

	//-------------------------------------------------------------------
	// Queries
	//-------------------------------------------------------------------
	unsigned GetMultiSampleCount() const{
		
	}
	RenderTargetPtr GetRenderTarget(HWindowId id) const{
		auto it = mWindowIds.Find(hwnd);
		if (it == mWindowIds.end()){
			Logger::Log(FB_ERROR_LOG_ARG, "Failed to find window Id");
			return 0;
		}
		return GetRenderTarget(it->second);		
	}
	void SetCamera(CameraPtr pCamera){
		
	}
	CameraPtr GetCamera() const{
		
	} // this is for current carmera.
	CameraPtr GetMainCamera() const{
		
	}
	HWindow GetWindowHandle(RenderTargetId rtId){
		assert(0);
		return 0;
	}
	Vec2I ToSreenPos(HWindowId id, const Vec3& ndcPos) const{
		
	}
	Vec2 ToNdcPos(HWindowId id, const Vec2I& screenPos) const{
		
	}
	unsigned GetNumLoadingTexture() const{
		
	}
	Vec2I FindClosestSize(HWindowId id, const Vec2I& input){
		
	}
	bool GetResolutionList(unsigned& outNum, Vec2I* list){
		
	}
	RenderOptionsPtr GetOptions() const{
		return mOptions;
	}

};

//---------------------------------------------------------------------------
Renderer::Renderer()
	: mImpl(new Impl(this))
{

}

Renderer::~Renderer(){
}

static RendererWeakPtr sRenderer;

RendererPtr Renderer::CreateRenderer(){
	if (sRenderer.lock()){
		Logger::Log(FB_ERROR_LOG_ARG, "You can create only one renderer!");
		return 0;
	}
	auto renderer = RendererPtr(FB_NEW(Renderer), [](Renderer* obj){ delete obj; });
	renderer->mMe = renderer;
	sRenderer = renderer;
	return renderer;
}

RendererPtr Renderer::CreateRenderer(const char* renderEngineName){
	if (sRenderer.lock()){
		Logger::Log(FB_ERROR_LOG_ARG, "You can create only one renderer!");
		return 0;
	}
	auto renderer = RendererPtr(FB_NEW(Renderer), [](Renderer* obj){ delete obj; });
	renderer->mMe = renderer;
	renderer->PrepareRenderEngine(renderEngineName);
	sRenderer = renderer;
	return renderer;
}

RendererPtr Renderer::GetInstance(){
	return sRenderer.lock();
}

void Renderer::PrepareRenderEngine(const char* renderEngineName){
	mImpl->PrepareRenderEngine(renderEngineName);
}

bool Renderer::InitCanvas(HWindowId id, HWindow window, int width, int height){
	return mImpl->InitCanvas(id, window, width, height);
}

void Renderer::ReleaseCanvas(HWindowId id){
	mImpl->ReleaseCanvas(id);
}

void Renderer::SetTimer(TimerPtr timer){
	mImpl->SetTimer(timer);
}

TimerPtr Renderer::GetTimer() const{
	return mImpl->GetTimer();
}

RenderTargetPtr Renderer::CreateRenderTarget(const RenderTargetParam& param){
	return mImpl->CreateRenderTarget(param);
}

void Renderer::KeepRenderTargetInPool(RenderTargetPtr rt){
	mImpl->KeepRenderTargetInPool(rt);
}

TexturePtr Renderer::CreateTexture(const char* file, bool async){
	return mImpl->CreateTexture(file, async);
}


HWindow Renderer::GetWindowHandle(RenderTargetId rtId){
	return mImpl->GetWindowHandle(rtId);
}

//-------------------------------------------------------------------
// GPU Manipulation
//-------------------------------------------------------------------
void Renderer::DrawIndexed(unsigned indexCount, unsigned startIndexLocation, unsigned startVertexLocation){
	mImpl->DrawIndexed(indexCount, startIndexLocation, startVertexLocation);
}

void Renderer::Draw(unsigned int vertexCount, unsigned int startVertexLocation){
	mImpl->Draw(vertexCount, startVertexLocation);
}

void Renderer::SetClearColor(HWindowId id, const Color& color){
	mImpl->SetClearColor(id, color);
}

void Renderer::SetClearDepthStencil(HWindowId id, Real z, UINT8 stencil){
	mImpl->SetClearDepthStencil(id, z, stencil);
}

void Renderer::Clear(Real r, Real g, Real b, Real a, Real z, UINT8 stencil){
	mImpl->Clear(r, g, b, a, z, stencil);
}

void Renderer::Clear(Real r, Real g, Real b, Real a){
	mImpl->Clear(r, g, b, a);
}

void Renderer::ClearState(){
	mImpl->ClearState();
}

void Renderer::BeginEvent(const char* name){
	mImpl->BeginEvent(name);
}

void Renderer::EndEvent(){
	mImpl->EndEvent();
}

void Renderer::TakeScreenshot(){
	mImpl->TakeScreenshot();
}

//-------------------------------------------------------------------
// Renderer State
//-------------------------------------------------------------------
void Renderer::SetWireframe(bool enable){
	mImpl->SetWireframe(enable);
}

bool Renderer::GetWireframe() const{
	return mImpl->GetWireframe();
}

RenderTargetPtr Renderer::GetMainRenderTarget() const{
	return mImpl->GetMainRenderTarget();
}

ScenePtr Renderer::GetMainScene() const{
	return mImpl->GetMainScene();
}
 // move to SceneManager
const Vec2I& Renderer::GetMainRTSize() const{
	return mImpl->GetMainRTSize();
}

void Renderer::SetCurrentRenderTarget(RenderTargetPtr renderTarget){
	return mImpl->SetCurrentRenderTarget(renderTarget);
}

RenderTargetPtr Renderer::GetCurrentRenderTarget() const{
	return mImpl->GetCurrentRenderTarget();
}

bool Renderer::IsMainRenderTarget() const{
	return mImpl->IsMainRenderTarget();
}

const Vec2I& Renderer::GetRenderTargetSize(HWindowId id) const{
	return mImpl->GetRenderTargetSize(id);
}

const Vec2I& Renderer::GetRenderTargetSize(HWindow hwnd) const{
	return mImpl->GetRenderTargetSize(hwnd);
}

void Renderer::SetDirectionalLight(DirectionalLightPtr pLight, int idx){
	mImpl->SetDirectionalLight(pLight, idx);
}

DirectionalLightPtr Renderer::GetDirectionalLight(int idx) const{
	return mImpl->GetDirectionalLight(idx);
}

DirectionalLightPtr Renderer::GetMainDirectionalLight(int idx) const{
	return mImpl->GetMainDirectionalLight(idx);
}

void Renderer::InitFrameProfiler(Real dt){
	mImpl->InitFrameProfiler(dt);
}

const RENDERER_FRAME_PROFILER& Renderer::GetFrameProfiler() const{
	return mImpl->GetFrameProfiler();
}

inline FontPtr Renderer::GetFont(Real fontHeight) const{
	return mImpl->GetFont(fontHeight);
}

const INPUT_ELEMENT_DESCS& Renderer::GetInputElementDesc(DEFAULT_INPUTS::Enum e){
	return mImpl->GetInputElementDesc(e);
}

void Renderer::SetEnvironmentTexture(TexturePtr texture){
	mImpl->SetEnvironmentTexture(texture);
}

void Renderer::SetEnvironmentTextureOverride(TexturePtr texture){
	mImpl->SetEnvironmentTextureOverride(texture);
}

void Renderer::SetDebugRenderTarget(unsigned idx, const char* textureName){
	mImpl->SetDebugRenderTarget(idx, textureName);
}

void Renderer::SetFadeAlpha(Real alpha){
	mImpl->SetFadeAlpha(alpha);
}

PointLightManPtr Renderer::GetPointLightMan() const{
	return mImpl->GetPointLightMan();
}

void Renderer::RegisterVideoPlayer(VideoPlayerPtr player){
	mImpl->RegisterVideoPlayer(player);
}

void Renderer::UnregisterVideoPlayer(VideoPlayerPtr player){
	mImpl->UnregisterVideoPlayer(player);
}

void Renderer::GetSampleOffsets_GaussBlur5x5(DWORD texWidth, DWORD texHeight, Vec4** avTexCoordOffset, Vec4** avSampleWeight, Real fMultiplier){
	mImpl->GetSampleOffsets_GaussBlur5x5(texWidth, texHeight, avTexCoordOffset, avSampleWeight, fMultiplier);
}

void Renderer::GetSampleOffsets_DownScale2x2(DWORD texWidth, DWORD texHeight, Vec4* avTexCoordOffset){
	mImpl->GetSampleOffsets_DownScale2x2(texWidth, texHeight, avTexCoordOffset);
}

bool Renderer::IsLuminanceOnCpu() const{
	return mImpl->IsLuminanceOnCpu();
}
