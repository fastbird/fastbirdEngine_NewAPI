/*
 -----------------------------------------------------------------------------
 This source file is part of fastbird engine
 For the latest info, see http://www.jungwan.net/
 
 Copyright (c) 2013-2015 Jungwan Byun
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "Renderer.h"
#include "IPlatformRenderer.h"
#include "RendererEnums.h"
#include "RendererStructs.h"
#include "Texture.h"
#include "RenderTarget.h"
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
#include "DebugHud.h"
#include "GeometryRenderer.h"
#include "Console.h"
#include "RenderStates.h"
#include "IVideoPlayer.h"
#include "FBStringLib/StringConverter.h"
#include "FBStringLib/StringLib.h"
#include "FBCommonHeaders/VectorMap.h"
#include "FBCommonHeaders/Factory.h"
#include "FBSystemLib/ModuleHandler.h"
#include "FBFileSystem/FileSystem.h"
#include "FBSceneManager/Scene.h"
#include "FBLua/LuaObject.h"
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
static const int defaultFontSize = 20;
const HWindow Renderer::INVALID_HWND = (HWindow)-1;
Timer* fastbird::gpTimer = 0;
class Renderer::Impl{
public:
	typedef fastbird::Factory<IPlatformRenderer>::CreateCallback CreateCallback;
	typedef std::vector<RenderTargetWeakPtr> RenderTargets;

	RendererWeakPtr mSelf;
	Renderer* mObject;
	lua_State* mL;
	ConsolePtr mConsole;
	std::string mPlatformRendererType;
	IPlatformRendererPtr mPlatformRenderer;
	ModuleHandle mLoadedModule;

	VectorMap<HWindowId, HWindow> mWindowHandles;
	VectorMap<HWindow, HWindowId> mWindowIds;
	VectorMap<HWindowId, Vec2I> mWindowSizes;
	HWindowId mMainWindowId;
	VectorMap<HWindowId, RenderTargetPtr> mWindowRenderTargets;
	RenderTargetPtr mCurrentRenderTarget;
	std::vector<TexturePtr> mCurrentRTTextures;
	std::vector<size_t> mCurrentViewIndices;
	TexturePtr mCurrentDSTexture;
	size_t mCurrentDSViewIndex;
	Vec2I mCurrentRTSize;
	
	std::vector<RenderTargetPtr> mRenderTargetPool;
	RenderTargets mRenderTargets;
	RenderTargets mRenderTargetsEveryFrame;	

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
	RasterizerStatePtr mWireFrameRS;
	bool mForcedWireframe;
	BlendStatePtr mDefaultBlendState;	
	BlendStatePtr mAdditiveBlendState;
	BlendStatePtr mAlphaBlendState;
	BlendStatePtr mMaxBlendState;

	DepthStencilStatePtr mDefaultDepthStencilState;
	DepthStencilStatePtr mNoDepthStencilState;
	DepthStencilStatePtr mNoDepthWriteLessEqualState;
	DepthStencilStatePtr mLessEqualDepthState;
	bool mLockDepthStencil;
	
	SamplerStatePtr mDefaultSamplers[SAMPLERS::NUM];

	TexturePtr mEnvironmentTexture;
	TexturePtr mEnvironmentTextureOverride;

	CameraPtr mCamera;
	RENDERER_FRAME_PROFILER mFrameProfiler;
	PRIMITIVE_TOPOLOGY mCurrentTopology;

	INPUT_ELEMENT_DESCS mInputLayoutDescs[DEFAULT_INPUTS::COUNT];
	const int DEFAULT_DYN_VERTEX_COUNTS=100;
	VertexBufferPtr mDynVBs[DEFAULT_INPUTS::COUNT];

	VectorMap<Vec2I, TexturePtr> mTempDepthBuffers;

	ShaderPtr mFullscreenQuadVSNear;
	ShaderPtr mFullscreenQuadVSFar;
	// linear sampler
	ShaderPtr mCopyPS;
	ShaderPtr mCopyPSMS;

	// DepthPass Resources	
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

	SkySphereWeakPtr mNextEnvUpdateSkySphere;
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

	std::vector<IVideoPlayerPtr> mVideoPlayers;

	ShaderPtr mGGXGenShader;
	TexturePtr mGGXGenTarget;
	bool mTakeScreenShot;

	VectorMap<SystemTextures::Enum, std::vector< TextureBinding > > mSystemTextureBindings;

	FRAME_CONSTANTS			mFrameConstants;
	CAMERA_CONSTANTS		mCameraConstants;
	RENDERTARGET_CONSTANTS	mRenderTargetConstants;
	SCENE_CONSTANTS			mSceneConstants;
	
	//-----------------------------------------------------------------------
	Impl(Renderer* renderer)
		: mObject(renderer)
		, mLoadedModule(0)
		, mCurrentTopology(PRIMITIVE_TOPOLOGY_UNKNOWN)
		, mForcedWireframe(false)
		, mUseFilmicToneMapping(true)
		, mFadeAlpha(0.)
		, mLuminance(0.5f)
		, mLuminanceOnCpu(false)
	{
		gpTimer = Timer::GetMainTimer().get();
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

	~Impl(){
		//for (int i = 0; i < DEFAULT_MATERIALS::COUNT; ++i){
		//	mMaterials[i] = 0;
		//}
		//mMissingMaterial = 0;

		//mDefaultRasterizerState = 0;
		//mFrontFaceCullRS = 0;
		//mOneBiasedDepthRS = 0;
		//mWireFrameRS = 0;

		//mDefaultBlendState = 0;
		//mAdditiveBlendState = 0;
		//mAlphaBlendState = 0;
		//mMaxBlendState = 0;

		//mDefaultDepthStencilState = 0;
		//mNoDepthStencilState = 0;
		//mNoDepthWriteLessEqualState = 0;
		//mLessEqualDepthState = 0;

		//for (int i = 0; i < SAMPLERS::NUM; ++i){
		//	mDefaultSamplers[i] = 0;
		//}

		//mEnvironmentTexture = 0;
		//mEnvironmentTextureOverride = 0;

		//mCamera = 0;

		//for (int i = 0; i < DEFAULT_INPUTS::COUNT; i++)
		//	mDynVBs[i] = 0;

		//mFullscreenQuadVSNear = 0;
		//mFullscreenQuadVSFar = 0;

		//mTempDepthBuffers.clear();

		//mCopyPS = 0;
		//mCopyPSMS = 0;

		//// DepthPass Resources
		//mDepthTarget = 0;
		//mDepthWriteShader = 0;
		//mCloudDepthWriteShader = 0;
		//mDepthOnlyShader = 0;
		//mMinBlendState = 0;

		//for (int i = 0; i < FB_NUM_TONEMAP_TEXTURES_NEW; ++i)
		//	mToneMap[FB_NUM_TONEMAP_TEXTURES_NEW] = 0;
		//for (int i = 0; i < FB_NUM_LUMINANCE_TEXTURES; ++i)
		//	mLuminanceMap[FB_NUM_LUMINANCE_TEXTURES] = 0;

		//mSampleLumInitialShader = 0;
		//mSampleLumIterativeShader = 0;
		//mSampleLumFinalShader = 0;
		//mCalcAdaptedLumShader = 0;
		//mToneMappingPS = 0;
		//mBrightPassPS = 0;
		//mBloomPS = 0;

		//// Star
		//mBlur5x5 = 0;
		//mStarGlareShader = 0;
		//mMergeTexture2 = 0;

		//// GodRay resources.
		//mOccPrePassShader = 0;
		//mOccPrePassGSShader = 0;
		//mGodRayPS = 0;
		//mPositionInputLayout = 0;
		//mGlowPS = 0;

		//// for Cloud Volumes;
		//mCloudVolumeDepth = 0;
		//mRedAlphaMaskBlend = 0;

		//mGreenAlphaMaskBlend = 0;
		//mGreenAlphaMaskMaxBlend = 0;

		//mRedAlphaMaskAddMinusBlend = 0;
		//mGreenAlphaMaskAddAddBlend = 0;

		//mRedAlphaMaskAddAddBlend = 0;
		//mGreenAlphaMaskAddMinusBlend = 0;

		//mGreenMaskBlend = 0;
		//mBlueMaskBlend = 0;
		//mNoColorWriteBlend = 0;
		//mRSCloudFar = 0;
		//mRSCloudNear = 0;
		//mNoiseMap = 0;		

		//mShadowMapShader = 0;
		//mCameraBackup = 0;
		//mSilouetteShader = 0;

		//for (unsigned i = 0; i < MaxDebugRenderTargets; ++i)
		//	mDebugRenderTargets[i].mTexture = 0;

		//mUIObjectsToRender.clear();
		//mUI3DObjects.clear();
		//mUI3DObjectsRTs.clear();
		//mHPBarObjects.clear();
		//mVideoPlayers.clear();
		//mGGXGenShader = 0;
		//mGGXGenTarget = 0;
		//
		//mCurrentRTTextures.clear();		
		//mMarkObjects.clear();
		//mUI3DRenderObjs.clear();
		//mCurrentDSTexture = 0;
		//mCurrentRenderTarget = 0;
		//mRenderTargetPool.clear();
		//mDebugHud = 0;
		//mGeomRenderer = 0;
		//mPointLightMan = 0;
		//for (int i = 0; i < 2; i++)
		//	mDirectionalLight[i] = 0;
		//mWindowRenderTargets.clear();		
		//mFonts.clear();
		//mPlatformRenderer = 0;

		//mConsole = 0;
	}

	void SetLuaState(lua_State* L){
		mL = L;
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
		mPlatformRenderer->InitCanvas(id, window, width, height, false, colorTexture, depthTexture);
		if (colorTexture && depthTexture){
			RenderTargetParam param;
			param.mSize = { width, height };
			param.mWillCreateDepth = true;
			auto rt = CreateRenderTarget(param);
			
			rt->SetColorTexture(CreateTexture(colorTexture));
			rt->SetDepthTexture(CreateTexture(depthTexture));

			mRenderTargetConstants.gScreenSize = Vec2(width, height);
			mRenderTargetConstants.gScreenRatio = width / (float)height;
			mRenderTargetConstants.rendertarget_dummy = 0;
			mPlatformRenderer->UpdateShaderConstants(ShaderConstants::RenderTarget, &mRenderTargetConstants, sizeof(RENDERTARGET_CONSTANTS));
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

		LuaObject multiFontSet(mL, "r_multiFont");
		if (multiFontSet.IsValid()){
			auto it = multiFontSet.GetSequenceIterator();
			LuaObject data;
			while (it.GetNext(data)){
				auto fontPath = data.GetString();
				if (!fontPath.empty()){
					FontPtr font = Font::Create();
					auto err = font->Init(fontPath.c_str());
					if (!err){
						font->SetTextEncoding(Font::UTF16);
						int height = Round(font->GetHeight());
						mFonts[height] = font;
					}
				}
			}
		}
		else{
			FontPtr font = Font::Create();
			LuaObject r_font(mL, "r_font");
			std::string fontPath = r_font.GetString();
			if (fontPath.empty())
			{
				fontPath = "es/fonts/font22.fnt";
			}
			auto err = font->Init(fontPath.c_str());
			if (!err){
				font->SetTextEncoding(Font::UTF16);
				int height = Round(font->GetHeight());
				mFonts[height] = font;
			}
		}

		mDebugHud = DebugHud::Create();
		mGeomRenderer = GeometryRenderer::Create();	

		mDefaultRasterizerState = CreateRasterizerState(RASTERIZER_DESC());
		mDefaultBlendState = CreateBlendState(BLEND_DESC());
		mDefaultDepthStencilState = CreateDepthStencilState(DEPTH_STENCIL_DESC());
		DEPTH_STENCIL_DESC ddesc;
		ddesc.DepthEnable = false;
		ddesc.DepthWriteMask = DEPTH_WRITE_MASK_ZERO;
		mNoDepthStencilState = CreateDepthStencilState(ddesc);

		SHADER_DEFINES emptyShaderDefines;
		mFullscreenQuadVSNear = CreateShader("es/shaders/fullscreenquadvs.hlsl", BINDING_SHADER_VS,
			emptyShaderDefines);
		SHADER_DEFINES shaderDefinesFar;
		shaderDefinesFar.push_back(ShaderDefine("_FAR_SIDE_QUAD", "1"));
		mFullscreenQuadVSFar = CreateShader("es/shaders/fullscreenquadvs.hlsl", BINDING_SHADER_VS,
			shaderDefinesFar);

		mCopyPS = CreateShader("es/shaders/copyps.hlsl", BINDING_SHADER_PS,
			emptyShaderDefines);
		SHADER_DEFINES multiSampleSD;
		multiSampleSD.push_back(ShaderDefine("_MULTI_SAMPLE", "1"));
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

		mMiddleGray = mOptions->r_HDRMiddleGray;
		mStarPower = mOptions->r_StarPower;
		mBloomPower = mOptions->r_BloomPower;

		UpdateRareConstantsBuffer();

		const auto& rtSize = GetMainRenderTargetSize();
		for (auto it : mFonts){
			it.second->SetRenderTargetSize(rtSize);
		}
		if (mDebugHud){
			mDebugHud->SetRenderTargetSize(rtSize);
		}
		if (mGeomRenderer){
			mGeomRenderer->SetRenderTargetSize(rtSize);
		}
		if (mConsole){
			mConsole->SetRenderTargetSize(rtSize);
		}
	}

	void ReleaseCanvas(HWindowId id){
		auto it = mWindowHandles.Find(id);
		if (it == mWindowHandles.end()){
			Logger::Log(FB_ERROR_LOG_ARG, "Cannot find the window.");
			return;
		}
	}

	void Render3DUIsToTexture()
	{
		/*if (!m3DUIEnabled)
			return;

		RenderEventMarker mark("Render3DUIsToTexture");
		for (auto scIt : mWindowRenderTargets) {
			for (auto rtIt : mUI3DObjectsRTs) {
				if (!rtIt.second->GetEnable())
					continue;
				auto& uiObjectsIt = mUI3DObjects.Find(std::make_pair(scIt.first, rtIt.first));
				if (uiObjectsIt != mUI3DObjects.end()){
					auto& uiObjects = uiObjectsIt->second;
					auto& rt = rtIt.second;
					rt->Bind();

					for (auto& uiobj : uiObjects)
					{
						uiobj->PreRender();
						uiobj->Render();
						uiobj->PostRender();
					}

					rt->Unbind();
					rt->GetRenderTargetTexture()->GenerateMips();
				}
			}
		}*/
	}

	void RenderMarks()
	{
		/*D3DEventMarker mark("Render Marks / HPBar");
		FB_FOREACH(it, mMarkObjects)
		{
			(*it)->PreRender();
			(*it)->Render();
			(*it)->PostRender();
		}

		for (auto it : mHPBarObjects)
		{
			it->PreRender();
			it->Render();
			it->PostRender();
		}*/
	}

	void RenderUI(HWindowId hwndId)
	{
		//D3DEventMarker mark("RenderUI");
		//auto& uiobjects = mUIObjectsToRender[hwndId];
		//auto it = uiobjects.begin(), itEnd = uiobjects.end();
		//for (; it != itEnd; it++)
		//{
		//	(*it)->PreRender(); // temporary :)
		//	(*it)->Render();
		//	(*it)->PostRender();
		//}
	}

	void RenderDebugRenderTargets()
	{
		auto rt = GetMainRenderTarget();
		assert(rt);
		const auto& size = rt->GetSize();
		for (int i = 0; i < MaxDebugRenderTargets; i++)
		{
			if (mDebugRenderTargets[i].mTexture)
			{
				Vec2 pixelPos = mDebugRenderTargets[i].mPos * Vec2((Real)size.x, (Real)size.y);
				Vec2 pixelSize = mDebugRenderTargets[i].mSize * Vec2((Real)size.x, (Real)size.y);
				DrawQuadWithTexture(Round(pixelPos), Round(pixelSize), Color(1, 1, 1, 1),
					mDebugRenderTargets[i].mTexture);
			}
		}
	}

	void RenderFade()
	{
		if (mFadeAlpha <= 0)
			return;
		auto mainRT = GetMainRenderTarget();
		assert(mainRT);
		DrawQuad(Vec2I(0, 0), mainRT->GetSize(), Color(0, 0, 0, mFadeAlpha));
	}

	void Render(TIME_PRECISION dt){
		InitFrameProfiler(dt);
		UpdateFrameConstantsBuffer();

		for (auto pRT : mRenderTargets)
		{
			auto rt = pRT.lock();
			rt->Render();
		}

		Render3DUIsToTexture();		
		auto mainRT = GetMainRenderTarget();
		for (auto it : mWindowRenderTargets)
		{
			RenderEventMarker mark(FormatString("Processing render target for %u", it.first).c_str());
			auto hwndId = it.first;
			auto rt = (RenderTarget*)it.second.get();
			assert(rt);
			bool rendered = rt->Render();
			if (rendered) {
				if (rt == mainRT.get()) {
					RenderMarks();
				}

				for (auto it : mObject->mObservers_[IRendererObserver::DefaultRenderEvent])	{
					auto observer = it.lock();
					if (observer)
						observer->BeforeUIRendering(hwndId, GetWindowHandle(hwndId));
				}
				RenderUI(hwndId);

				for (auto it : mObject->mObservers_[IRendererObserver::DefaultRenderEvent]){
					auto observer = it.lock();
					if (observer)
						observer->AfterUIRendered(hwndId, GetWindowHandle(hwndId));
				}
			}
		}
		mainRT->BindTargetOnly(false);

		for (auto& it : mVideoPlayers){
			it->Update(dt);
		}

		RenderDebugHud();
		RenderDebugRenderTargets();

		RenderFade();
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

		auto rt = RenderTarget::Create();
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

	VectorMap<std::string, IPlatformTextureWeakPtr> sPlatformTextures;
	TexturePtr CreateTexture(const char* file, bool async){
		if (!ValidCStringLength(file)){
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
		auto texture = CreateTexture(platformTexture);
		texture->SetFilePath(loweredFilepath.c_str());
		return texture;
	}

	TexturePtr CreateTexture(void* data, int width, int height, PIXEL_FORMAT format,
		BUFFER_USAGE usage, int  buffer_cpu_access, int texture_type){
		auto platformTexture = mPlatformRenderer->CreateTexture(data, width, height, format, usage, buffer_cpu_access, texture_type);
		if (!platformTexture){
			Logger::Log(FB_ERROR_LOG_ARG, "Failed to create texture with data.");
			return 0;
		}
		auto texture = Texture::Create();
		texture->SetPlatformTexture(platformTexture);
		return texture;
	}

	TexturePtr CreateTexture(IPlatformTexturePtr platformTexture){
		auto texture = Texture::Create();
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
		auto vertexBuffer = VertexBuffer::Create(stride, numVertices);
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
		auto indexBuffer = IndexBuffer::Create(numIndices, format);
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
			return false;
		}

		bool operator == (const ShaderCreationInfo& other) const{
			return mShaders == other.mShaders && mFilepath == other.mFilepath && mDefines == other.mDefines;
		}

		int mShaders;
		std::string mFilepath;		
		SHADER_DEFINES mDefines;
	};
	VectorMap<ShaderCreationInfo, IPlatformShaderWeakPtr> sPlatformShaders;
	IPlatformShaderPtr FindPlatformShader(const ShaderCreationInfo& key){
		auto it = sPlatformShaders.Find(key);
		if (it != sPlatformShaders.end()){
			auto platformShader = it->second.lock();
			if (platformShader){
				return platformShader;
			}
		}
		return 0;
	}
	ShaderPtr CreateShader(const char* filepath, int shaders) {
		CreateShader(filepath, shaders, SHADER_DEFINES());
	}
	ShaderPtr CreateShader(const char* filepath, int shaders,
		const SHADER_DEFINES& defines) {
		if (!ValidCStringLength(filepath)){
			Logger::Log(FB_ERROR_LOG_ARG, "Invalid arg.");
			return 0;
		}

		SHADER_DEFINES sortedDefines(defines);
		std::string loweredPath(filepath);		
		std::sort(sortedDefines.begin(), sortedDefines.end());
		auto key = ShaderCreationInfo(loweredPath.c_str(), shaders, sortedDefines);

		auto platformShader = FindPlatformShader(key);
		if (platformShader){
			auto shader = GetShaderFromExistings(platformShader);
			if (shader && shader->GetShaderDefines() == sortedDefines){				
				return shader;
			}
		}
		if (!platformShader)
			platformShader = mPlatformRenderer->CreateShader(filepath, shaders, sortedDefines);
		if (platformShader){
			auto shader = Shader::Create();
			shader->SetPlatformShader(platformShader);
			shader->SetShaderDefines(sortedDefines);
			shader->SetPath(filepath);
			shader->SetBindingShaders(shaders);
			sPlatformShaders[key] = platformShader;
			return shader;
		}
		Logger::Log(FB_ERROR_LOG_ARG, FormatString("Failed to create a shader(%s)", filepath).c_str());
		return 0;
	}

	bool ReapplyShaderDefines(Shader* shader){
		if (!shader)
			return false;
		auto filepath = shader->GetPath();
		std::string loweredPath = filepath;
		if (loweredPath.empty()){
			Logger::Log(FB_ERROR_LOG_ARG, "Path is empty.");
			return false;
		}
		ToLowerCase(loweredPath);
		int bindingShaders = shader->GetBindingShaders();
		auto sortedDefines = shader->GetShaderDefines();
		std::sort(sortedDefines.begin(), sortedDefines.end());
		auto key = ShaderCreationInfo(loweredPath.c_str(), bindingShaders, sortedDefines);
		auto platformShader = FindPlatformShader(key);
		if (platformShader){
			shader->SetPlatformShader(platformShader);
			return true;
		}
		platformShader = mPlatformRenderer->CreateShader(filepath, bindingShaders, sortedDefines);
		if (platformShader){
			shader->SetPlatformShader(platformShader);
			return true;
		}
		Logger::Log(FB_ERROR_LOG_ARG, FormatString("Failed to reapply shader defines(%s)", filepath).c_str());
		return false;
	}

	VectorMap<std::string, MaterialWeakPtr> sLoadedMaterials;
	MaterialPtr CreateMaterial(const char* file){
		if (!ValidCStringLength(file)){
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
		auto material = Material::Create();
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
	
	VectorMap<INPUT_ELEMENT_DESCS, InputLayoutWeakPtr> sInputLayouts;
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
		auto platformInputLayout = mPlatformRenderer->CreateInputLayout(descs, data, size);
		auto inputLayout = InputLayout::Create();
		inputLayout->SetPlatformInputLayout(platformInputLayout);
		sInputLayouts[descs] = inputLayout;
		return inputLayout;
	}

	InputLayoutPtr GetInputLayout(DEFAULT_INPUTS::Enum e, ShaderPtr shader){
		const auto& desc = GetInputElementDesc(e);
		return CreateInputLayout(desc, shader);
	}

	VectorMap<RASTERIZER_DESC, RasterizerStateWeakPtr> sRasterizerStates;
	RasterizerStatePtr CreateRasterizerState(const RASTERIZER_DESC& desc){
		auto it = sRasterizerStates.Find(desc);
		if (it != sRasterizerStates.end()){
			auto state = it->second.lock();
			if (state){
				return state;
			}
		}
		auto platformRaster = mPlatformRenderer->CreateRasterizerState(desc);
		auto raster = RasterizerState::Create();
		raster->SetPlatformState(platformRaster);
		sRasterizerStates[desc] = raster;
		return raster;
	}

	VectorMap<BLEND_DESC, BlendStateWeakPtr> sBlendStates;
	BlendStatePtr CreateBlendState(const BLEND_DESC& desc){		
		auto it = sBlendStates.Find(desc);
		if (it != sBlendStates.end()){
			auto state = it->second.lock();
			if (state){
				return state;
			}
		}
		auto platformState = mPlatformRenderer->CreateBlendState(desc);
		auto state = BlendState::Create();
		state->SetPlatformState(platformState);
		sBlendStates[desc] = state;
		return state;
		
	}

	VectorMap<DEPTH_STENCIL_DESC, DepthStencilStateWeakPtr> sDepthStates;
	DepthStencilStatePtr CreateDepthStencilState(const DEPTH_STENCIL_DESC& desc){
		auto it = sDepthStates.Find(desc);
		if (it != sDepthStates.end()){
			auto state = it->second.lock();
			if (state){
				return state;
			}
		}
		auto platformState = mPlatformRenderer->CreateDepthStencilState(desc);
		auto state = DepthStencilState::Create();
		state->SetPlatformState(platformState);
		sDepthStates[desc] = state;
		return state;
	}

	VectorMap<SAMPLER_DESC, SamplerStateWeakPtr> sSamplerStates;
	SamplerStatePtr CreateSamplerState(const SAMPLER_DESC& desc){
		auto it = sSamplerStates.Find(desc);
		if (it != sSamplerStates.end()){
			auto state = it->second.lock();
			if (state){
				return state;
			}
		}
		auto platformState = mPlatformRenderer->CreateSamplerState(desc);
		auto state = SamplerState::Create();
		state->SetPlatformState(platformState);
		sSamplerStates[desc] = state;
		return state;

	}

	// holding strong pointer
	VectorMap<std::string, TextureAtlasPtr> sTextureAtlas;
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
			if (ValidCStringLength(errMsg)){
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
			pTextureAtlas = TextureAtlas::Create();
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
	

	//-------------------------------------------------------------------
	// Resource Bindings
	//-------------------------------------------------------------------	
	void SetRenderTarget(TexturePtr pRenderTargets[], size_t rtViewIndex[], int num,
		TexturePtr pDepthStencil, size_t dsViewIndex){
		assert(num <= 4);
		if (mCurrentDSTexture == pDepthStencil && mCurrentDSViewIndex == dsViewIndex){
			if (mCurrentRTTextures.size() == num && mCurrentViewIndices.size() == num){
				bool same = true;
				for (int i = 0; i < num; ++i){
					if (mCurrentRTTextures[i] != pRenderTargets[i] || mCurrentViewIndices[i] != rtViewIndex[i]){
						same = false;
					}
				}
				if (same){
					return;
				}
			}
		}
		static TIME_PRECISION time = 0;
		static std::set<TextureWeakPtr, std::owner_less<TextureWeakPtr>> usedRenderTargets;
		if (GetOptions()->r_numRenderTargets && gpTimer)
		{
			for (int i = 0; i<num; i++)
			{
				usedRenderTargets.insert(pRenderTargets[i]);
			}
			if (gpTimer->GetTime() - time > 5)
			{
				time = gpTimer->GetTime();
				Logger::Log(FB_DEFAULT_LOG_ARG, FormatString("used RenderTargets", usedRenderTargets.size()).c_str());				
			}
		}
		mCurrentRTTextures.clear();
		mCurrentViewIndices.clear();		
		std::copy(pRenderTargets, pRenderTargets + num, std::back_inserter(mCurrentRTTextures));
		std::copy(rtViewIndex, rtViewIndex + num, std::back_inserter(mCurrentViewIndices));
		mCurrentDSTexture = pDepthStencil;
		mCurrentDSViewIndex = dsViewIndex;
		IPlatformTexturePtr platformRTs[4] = { 0 };
		for (int i = 0; i < num; ++i){
			platformRTs[i] = pRenderTargets[i]->GetPlatformTexture();
		}

		mPlatformRenderer->SetRenderTarget(platformRTs, rtViewIndex, num,
			pDepthStencil->GetPlatformTexture(), dsViewIndex);

		if (pRenderTargets && num>0 && pRenderTargets[0])
		{
			mCurrentRTSize = pRenderTargets[0]->GetSize();
		}
		else
		{
			mCurrentRTSize = GetMainRenderTargetSize();
		}

		for (auto it : mFonts){
			it.second->SetRenderTargetSize(mCurrentRTSize);
		}

		UpdateRenderTargetConstantsBuffer();
	}

	void UnbindRenderTarget(TexturePtr renderTargetTexture){
		auto rtTextures = mCurrentRTTextures;
		auto viewIndices = mCurrentViewIndices;
		auto size = rtTextures.size();
		bool removed = false;
		for (unsigned i = 0; i < size; i++){
			if (rtTextures[i] == renderTargetTexture){
				rtTextures[i] = 0;
				viewIndices[i] = 0;
				removed = true;
			}
		}
		auto dsTexture = mCurrentDSTexture;
		auto dsViewIndex = mCurrentDSViewIndex;
		if (dsTexture == renderTargetTexture)
		{
			removed = true;
			dsTexture = 0;
			dsViewIndex = 0;
		}
		if (removed){
			SetRenderTarget(&rtTextures[0], &viewIndices[0], size, dsTexture, dsViewIndex);
		}
	}

	void SetViewports(const Viewport viewports[], int num){
		mPlatformRenderer->SetViewports(viewports, num);
	}

	void SetScissorRects(const Rect rects[], int num){
		mPlatformRenderer->SetScissorRects(rects, num);
	}

	void SetVertexBuffers(unsigned int startSlot, unsigned int numBuffers,
		VertexBufferPtr pVertexBuffers[], unsigned int strides[], unsigned int offsets[]) {
		static const unsigned int numMaxVertexInputSlot = 32; //D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT (32)
		IPlatformVertexBufferPtr platformBuffers[numMaxVertexInputSlot];
		numBuffers = std::min(numMaxVertexInputSlot, numBuffers);		
		for (unsigned i = 0; i < numBuffers; ++i){
			platformBuffers[i] = pVertexBuffers[i]->GetPlatformBuffer();
		}
		mPlatformRenderer->SetVertexBuffers(startSlot, numBuffers, platformBuffers, strides, offsets);
	}

	void SetPrimitiveTopology(PRIMITIVE_TOPOLOGY pt){
		if (mCurrentTopology == pt)
			return;
		mPlatformRenderer->SetPrimitiveTopology(pt);
		mCurrentTopology = pt;
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
			texture->Bind(binding.mShader, binding.mSlot);
		}
	}

	void UnbindTexture(BINDING_SHADER shader, int slot){
		mPlatformRenderer->UnbindTexture(shader, slot);
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
			mDepthWriteShader = CreateShader("es/shaders/depth.hlsl", BINDING_SHADER_VS | BINDING_SHADER_PS, SHADER_DEFINES());
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
		if (!mGodRayPS)
		{
			mGodRayPS = CreateShader("es/shaders/GodRayPS.hlsl", BINDING_SHADER_PS, SHADER_DEFINES());
			mOccPrePassShader = CreateShader("es/shaders/OccPrePass.hlsl", BINDING_SHADER_VS | BINDING_SHADER_PS,
				SHADER_DEFINES());
			mOccPrePassGSShader = CreateShader("es/shaders/OccPrePassGS.hlsl", BINDING_SHADER_VS | BINDING_SHADER_PS | BINDING_SHADER_GS,
				SHADER_DEFINES());
		}
		return mGodRayPS;
	}

	ShaderPtr GetGlowShader(){
		if (!mGlowPS)
		{
			SHADER_DEFINES shaderDefines;
			if (GetMultiSampleCount() != 1)
			{
				shaderDefines.push_back(ShaderDefine());
				shaderDefines.back().name = "_MULTI_SAMPLE";
				shaderDefines.back().value = "1";
			}
			mGlowPS = CreateShader("es/shaders/BloomPS.hlsl", BINDING_SHADER_PS, shaderDefines);
			mGlowPS->SetDebugName("GlowPS");
		}
		return mGlowPS;
	}

	void SetShadowMapShader(){
		if (!mShadowMapShader)
		{
			mShadowMapShader = CreateShader("es/shaders/shadowdepth.hlsl", BINDING_SHADER_VS | BINDING_SHADER_PS,
				SHADER_DEFINES());
		}
		mShadowMapShader->Bind();
	}

	ShaderPtr GetSilouetteShader(){
		if (!mSilouetteShader)
		{
			mSilouetteShader = CreateShader("es/shaders/silouette.hlsl", BINDING_SHADER_PS,
				SHADER_DEFINES());
		}
		return mSilouetteShader;
	}

	ShaderPtr GetCopyPS(){
		assert(mCopyPS);
		return mCopyPS;
	}

	ShaderPtr GetCopyPSMS(){
		assert(mCopyPSMS);
		return mCopyPSMS;
	}

	TexturePtr GetToneMap(unsigned idx){
		if (mToneMap[0] == 0)
		{
			int nSampleLen = 1;
			for (int i = 0; i < FB_NUM_TONEMAP_TEXTURES_NEW; i++)
			{
				// 1, 3, 9, 27, 81
				mToneMap[i] = CreateTexture(0, nSampleLen, nSampleLen, PIXEL_FORMAT_R16_FLOAT, BUFFER_USAGE_DEFAULT,
					BUFFER_CPU_ACCESS_NONE, TEXTURE_TYPE_RENDER_TARGET_SRV);
				char buff[255];
				sprintf_s(buff, "ToneMap(%d)", nSampleLen);
				mToneMap[i]->SetDebugName(buff);
				nSampleLen *= 3;
			}
			for (int i = 0; i < FB_NUM_LUMINANCE_TEXTURES; i++)
			{
				mLuminanceMap[i] = CreateTexture(0, 1, 1, PIXEL_FORMAT_R16_FLOAT, BUFFER_USAGE_DEFAULT,
					BUFFER_CPU_ACCESS_NONE, TEXTURE_TYPE_RENDER_TARGET_SRV);
			}
			TexturePtr textures[] = { mLuminanceMap[0], mLuminanceMap[1] };
			size_t index[] = { 0, 0 };
			SetRenderTarget(textures, index, 2, 0, 0);
			Clear(0, 0, 0, 1);

			SHADER_DEFINES shaderDefines;
			if (GetMultiSampleCount() != 1)
			{
				shaderDefines.push_back(ShaderDefine());
				shaderDefines.back().name = "_MULTI_SAMPLE";
				shaderDefines.back().value = "1";
			}

			mSampleLumInitialShader = CreateShader("es/shaders/SampleLumInitialNew.hlsl", BINDING_SHADER_PS, shaderDefines);
			mSampleLumIterativeShader = CreateShader("es/shaders/SampleLumIterativeNew.hlsl", BINDING_SHADER_PS);
			mSampleLumFinalShader = CreateShader("es/shaders/SampleLumFinalNew.hlsl", BINDING_SHADER_PS);
			mCalcAdaptedLumShader = CreateShader("es/shaders/CalculateAdaptedLum.hlsl", BINDING_SHADER_PS);
		}

		assert(idx < FB_NUM_TONEMAP_TEXTURES_NEW);
		assert(mToneMap[idx] != 0);
		return mToneMap[idx];
	}

	ShaderPtr GetSampleLumInitialShader(){
		assert(mSampleLumInitialShader);
		return mSampleLumInitialShader;
	}

	ShaderPtr GetSampleLumIterativeShader(){
		assert(mSampleLumIterativeShader);
		return mSampleLumIterativeShader;
	}

	ShaderPtr GetSampleLumFinalShader(){
		assert(mSampleLumFinalShader);
		return mSampleLumFinalShader;
	}

	void SwapLuminanceMap(){
		std::swap(mLuminanceMap[0], mLuminanceMap[1]);
	}

	TexturePtr GetLuminanceMap(unsigned idx){
		assert(idx < FB_NUM_LUMINANCE_TEXTURES);
		return mLuminanceMap[idx];
	}

	ShaderPtr GetCalcAdapedLumShader(){
		assert(mCalcAdaptedLumShader);
		return mCalcAdaptedLumShader;
	}

	ShaderPtr GetBrightPassPS(){
		if (!mBrightPassPS)
		{
			const char* bpPath = "es/shaders/brightpassps.hlsl";
			SHADER_DEFINES shaderDefines;
			if (GetMultiSampleCount() != 1)
			{
				shaderDefines.push_back(ShaderDefine());
				shaderDefines.back().name = "_MULTI_SAMPLE";
				shaderDefines.back().value = "1";
			}
			mBrightPassPS = CreateShader(bpPath, BINDING_SHADER_PS, shaderDefines);
		}
		assert(mBrightPassPS);
		return mBrightPassPS;
	}

	ShaderPtr GetBlur5x5PS(){
		if (!mBlur5x5)
		{
			SHADER_DEFINES shaderDefines;
			if (GetMultiSampleCount() != 1)
			{
				shaderDefines.push_back(ShaderDefine());
				shaderDefines.back().name = "_MULTI_SAMPLE";
				shaderDefines.back().value = "1";
			}
			mBlur5x5 = CreateShader("es/shaders/gaussblur5x5.hlsl", BINDING_SHADER_PS, shaderDefines);
		}
		return mBlur5x5;
	}

	ShaderPtr GetBloomPS(){
		if (!mBloomPS)
		{
			const char* blPath = "es/shaders/bloomps.hlsl";
			mBloomPS = CreateShader(blPath, BINDING_SHADER_PS, SHADER_DEFINES());
		}
		return mBloomPS;
	}

	ShaderPtr GetStarGlareShader(){
		if (!mStarGlareShader)
			mStarGlareShader = CreateShader("es/shaders/starglare.hlsl", BINDING_SHADER_PS, SHADER_DEFINES());

		return mStarGlareShader;
	}

	ShaderPtr GetMergeTexturePS(){
		if (!mMergeTexture2)
			mMergeTexture2 = CreateShader("es/shaders/mergetextures2ps.hlsl", BINDING_SHADER_PS, SHADER_DEFINES());
		return mMergeTexture2;
	}

	ShaderPtr GetToneMappingPS(){
		if (!mToneMappingPS)
			CreateToneMappingShader();
		return mToneMappingPS;
	}

	void CreateToneMappingShader()
	{
		SHADER_DEFINES shaderDefines;
		if (GetMultiSampleCount() != 1)
			shaderDefines.push_back(ShaderDefine("_MULTI_SAMPLE", "1"));

		if (mLuminanceOnCpu)
			shaderDefines.push_back(ShaderDefine("_CPU_LUMINANCE", "1"));

		if (mUseFilmicToneMapping)
			shaderDefines.push_back(ShaderDefine("_FILMIC_TONEMAP", "1"));

		mToneMappingPS = CreateShader("es/shaders/tonemapping.hlsl", BINDING_SHADER_PS, shaderDefines);
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
	void SetNoColorWriteState(){

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
	void SetDepthOnlyShader(){

	}
	// raster
	void SetFrontFaceCullRS(){
		
	}
	void SetOneBiasedDepthRS(){

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
		mRenderTargetConstants.gScreenSize.x = mCurrentRTSize.x;
		mRenderTargetConstants.gScreenSize.y = mCurrentRTSize.y;
		mRenderTargetConstants.gScreenRatio = mCurrentRTSize.x / (float)mCurrentRTSize.y;
		mRenderTargetConstants.rendertarget_dummy = 0.f;
		mPlatformRenderer->UpdateShaderConstants(ShaderConstants::RenderTarget, &mRenderTargetConstants, sizeof(RENDERTARGET_CONSTANTS));
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

	void DrawFullscreenQuad(ShaderPtr pixelShader, bool farside){
		// vertex buffer

		if (farside)
			mFullscreenQuadVSFar->BindVS();
		else
			mFullscreenQuadVSNear->BindVS();

		if (pixelShader)
			pixelShader->BindPS();

		UnbindInputLayout();
		UnbindShader(BINDING_SHADER_GS);
		// draw
		// using full screen triangle : http://blog.naver.com/jungwan82/220108100698
		SetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Draw(3, 0);
	}

	void DrawTriangle(const Vec3& a, const Vec3& b, const Vec3& c, const Vec4& color, MaterialPtr mat){
		VertexBuffer* pVB = mDynVBs[DEFAULT_INPUTS::POSITION].get();
		assert(pVB);
		MapData mapped = pVB->Map(0, MAP_TYPE_WRITE_DISCARD, MAP_FLAG_NONE);
		DEFAULT_INPUTS::V_P* data = (DEFAULT_INPUTS::V_P*)mapped.pData;
		data[0].p = a;
		data[1].p = b;
		data[2].p = c;
		pVB->Unmap(0);
		pVB->Bind();
		SetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mat->SetMaterialParameters(0, color);
		mat->Bind(true);
		Draw(3, 0);
	}

	void DrawQuad(const Vec2I& pos, const Vec2I& size, const Color& color, bool updateRs = true){
		// vertex buffer
		auto rtSize = GetCurrentRenderTargetSize();
		Mat44 screenToProj(2.f / rtSize.x, 0, 0, -1.f,
			0.f, -2.f / rtSize.y, 0, 1.f,
			0, 0, 1.f, 0.f,
			0, 0, 0, 1.f);

		MapData mapped = mDynVBs[DEFAULT_INPUTS::POSITION_COLOR]->Map(0, MAP_TYPE_WRITE_DISCARD, MAP_FLAG_NONE);
		DEFAULT_INPUTS::V_PC data[4] = {
			DEFAULT_INPUTS::V_PC(Vec3((float)pos.x, (float)pos.y, 0.f), color.Get4Byte()),
			DEFAULT_INPUTS::V_PC(Vec3((float)pos.x + size.x, (float)pos.y, 0.f), color.Get4Byte()),
			DEFAULT_INPUTS::V_PC(Vec3((float)pos.x, (float)pos.y + size.y, 0.f), color.Get4Byte()),
			DEFAULT_INPUTS::V_PC(Vec3((float)pos.x + size.x, (float)pos.y + size.y, 0.f), color.Get4Byte()),
		};
		for (int i = 0; i < 4; i++){
			data[i].p = (screenToProj * Vec4(data[i].p, 1.0)).GetXYZ();
		}
		if (mapped.pData)
		{
			memcpy(mapped.pData, data, sizeof(data));
			mDynVBs[DEFAULT_INPUTS::POSITION_COLOR]->Unmap(0);
		}

		if (updateRs){
			// set primitive topology
			SetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			// set material
			mMaterials[DEFAULT_MATERIALS::QUAD]->Bind(true);
		}


		// set vertex buffer
		mDynVBs[DEFAULT_INPUTS::POSITION_COLOR]->Bind();
		// draw
		Draw(4, 0);
	}

	void DrawQuadWithTexture(const Vec2I& pos, const Vec2I& size, const Color& color, TexturePtr texture, MaterialPtr materialOverride = 0){
		DrawQuadWithTextureUV(pos, size, Vec2(0, 0), Vec2(1, 1), color, texture, materialOverride);
	}

	void DrawQuadWithTextureUV(const Vec2I& pos, const Vec2I& size, const Vec2& uvStart, const Vec2& uvEnd,
		const Color& color, TexturePtr texture, MaterialPtr materialOverride = 0){
		// vertex buffer
		MapData mapped = mDynVBs[DEFAULT_INPUTS::POSITION_COLOR_TEXCOORD]->Map(0, MAP_TYPE_WRITE_DISCARD, MAP_FLAG_NONE);
		DEFAULT_INPUTS::V_PCT data[4] = {
			DEFAULT_INPUTS::V_PCT(Vec3((float)pos.x, (float)pos.y, 0.f), color.Get4Byte(), Vec2(uvStart.x, uvStart.y)),
			DEFAULT_INPUTS::V_PCT(Vec3((float)pos.x + size.x, (float)pos.y, 0.f), color.Get4Byte(), Vec2(uvEnd.x, uvStart.y)),
			DEFAULT_INPUTS::V_PCT(Vec3((float)pos.x, (float)pos.y + size.y, 0.f), color.Get4Byte(), Vec2(uvStart.x, uvEnd.y)),
			DEFAULT_INPUTS::V_PCT(Vec3((float)pos.x + size.x, (float)pos.y + size.y, 0.f), color.Get4Byte(), Vec2(uvEnd.x, uvEnd.y)),
		};
		auto& rtSize = GetCurrentRenderTargetSize();
		Mat44 screenToProj(2.f / rtSize.x, 0, 0, -1.f,
			0.f, -2.f / rtSize.y, 0, 1.f,
			0, 0, 1.f, 0.f,
			0, 0, 0, 1.f);

		for (int i = 0; i < 4; ++i){
			data[i].p = (screenToProj * Vec4(data[i].p, 1.)).GetXYZ();
		}

		if (mapped.pData)
		{
			memcpy(mapped.pData, data, sizeof(data));
			mDynVBs[DEFAULT_INPUTS::POSITION_COLOR_TEXCOORD]->Unmap(0);
		}

		SetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		if (materialOverride)
			materialOverride->Bind(true);
		else
			mMaterials[DEFAULT_MATERIALS::QUAD_TEXTURE]->Bind(true);
		texture->Bind(BINDING_SHADER_PS, 0);
		mDynVBs[DEFAULT_INPUTS::POSITION_COLOR_TEXCOORD]->Bind();
		Draw(4, 0);
	}

	void DrawBillboardWorldQuad(const Vec3& pos, const Vec2& size, const Vec2& offset,
		DWORD color, MaterialPtr pMat){
		VertexBuffer* pVB = mDynVBs[DEFAULT_INPUTS::POSITION_VEC4_COLOR].get();
		assert(pVB);
		MapData mapped = pVB->Map(0, MAP_TYPE_WRITE_DISCARD, MAP_FLAG_NONE);
		DEFAULT_INPUTS::POSITION_VEC4_COLOR_V* data = (DEFAULT_INPUTS::POSITION_VEC4_COLOR_V*)mapped.pData;
		data->p = pos;
		data->v4 = Vec4(size.x, size.y, offset.x, offset.y);
		data->color = color;
		pVB->Unmap(0);
		pVB->Bind();
		SetPrimitiveTopology(PRIMITIVE_TOPOLOGY_POINTLIST);
		pMat->Bind(true);
		Draw(1, 0);
	}

	void QueueDrawText(const Vec2I& pos, WCHAR* text, const Color& color, Real size){
		if (mDebugHud)
			mDebugHud->DrawText(pos, text, color, size);
	}

	void QueueDrawText(const Vec2I& pos, const char* text, const Color& color, Real size){
		QueueDrawText(pos, AnsiToWide(text, strlen(text)), color, size);
	}

	void QueueDraw3DText(const Vec3& worldpos, WCHAR* text, const Color& color, Real size){
		if (mDebugHud)
			mDebugHud->Draw3DText(worldpos, text, color, size);
	}

	void QueueDraw3DText(const Vec3& worldpos, const char* text, const Color& color, Real size){
		QueueDraw3DText(worldpos, AnsiToWide(text), color, size);
	}

	void QueueDrawTextForDuration(Real secs, const Vec2I& pos, WCHAR* text,
		const Color& color, Real size){
		if (mDebugHud)
			mDebugHud->DrawTextForDuration(secs, pos, text, color, size);
	}

	void QueueDrawTextForDuration(Real secs, const Vec2I& pos, const char* text,
		const Color& color, Real size){
		QueueDrawTextForDuration(secs, pos, AnsiToWide(text, strlen(text)), color, size);
	}

	void ClearDurationTexts(){
		if (mDebugHud)
			mDebugHud->ClearDurationTexts();
	}

	void QueueDrawLine(const Vec3& start, const Vec3& end,
		const Color& color0, const Color& color1){
		if (mDebugHud)
			mDebugHud->DrawLine(start, end, color0, color1);
	}

	void QueueDrawLine(const Vec2I& start, const Vec2I& end,
		const Color& color0, const Color& color1){
		if (mDebugHud)
			mDebugHud->DrawLine(start, end, color0, color1);
	}

	void QueueDrawLineBeforeAlphaPass(const Vec3& start, const Vec3& end,
		const Color& color0, const Color& color1){
		if (mDebugHud)
			mDebugHud->DrawLineBeforeAlphaPass(start, end, color0, color1);
	}

	void QueueDrawQuad(const Vec2I& pos, const Vec2I& size, const Color& color){
		if (mDebugHud){
			mDebugHud->DrawQuad(pos, size, color);
		}
	}

	void QueueDrawTexturedThickLine(const Vec3& start, const Vec3& end, const Color& color0, const Color& color1, Real thickness,
		const char* texture, bool textureFlow){
		if (mGeomRenderer)
			mGeomRenderer->DrawTexturedThickLine(start, end, color0, color1, thickness, texture, textureFlow);
	}

	void QueueDrawSphere(const Vec3& pos, Real radius, const Color& color){
		if (mGeomRenderer)
			mGeomRenderer->DrawSphere(pos, radius, color);
	}

	void QueueDrawBox(const Vec3& boxMin, const Vec3& boxMax, const Color& color, Real alpha){
		if (mGeomRenderer)
			mGeomRenderer->DrawBox(boxMin, boxMax, color, alpha);
	}

	void QueueDrawTriangle(const Vec3& a, const Vec3& b, const Vec3& c, const Color& color, Real alpha){
		if (mGeomRenderer)
			mGeomRenderer->DrawTriangle(a, b, c, color, alpha);
	}

	void QueueDrawQuadLine(const Vec2I& pos, const Vec2I& size, const Color& color){
		int left = pos.x - 1;
		int top = pos.y - 1;
		int right = pos.x + size.x + 1;
		int bottom = pos.y + size.y + 1;
		QueueDrawLine(Vec2I(left, top), Vec2I(right, top), color, color);
		QueueDrawLine(Vec2I(left, top), Vec2I(left, bottom), color, color);
		QueueDrawLine(Vec2I(right, top), Vec2I(right, bottom), color, color);
		QueueDrawLine(Vec2I(left, bottom), Vec2I(right, bottom), color, color);
	}

	//-------------------------------------------------------------------
	// Internal
	//-------------------------------------------------------------------
	void GatherPointLightData(const BoundingVolume* aabb, const Transformation& transform, POINT_LIGHT_CONSTANTS* plConst){
		mPointLightMan->GatherPointLightData(aabb, transform, plConst);
	}
	void RefreshPointLight(){
		mRefreshPointLight = true;
	}
	bool NeedToRefreshPointLight() const{
		return mRefreshPointLight;
	}

	void RenderDebugHud(){
		if (!mDebugHud)
			return;
		RenderEventMarker devent("RenderDebugHud");
		for (auto it : mObject->mObservers_[IRendererObserver::DefaultRenderEvent])
		{
			auto observer = it.lock();
			if (observer){
				observer->BeforeDebugHudRendered( mMainWindowId, GetMainWindowHandle() );
			}
		}

		RestoreRenderStates();
		RenderParam param;
		param.mRenderPass = PASS_NORMAL;
		mDebugHud->Render(param, 0);
		//SetWireframe(backup);
		for (auto it : mObject->mObservers_[IRendererObserver::DefaultRenderEvent])
		{
			auto observer = it.lock();
			if (observer){
				observer->AfterDebugHudRendered(mMainWindowId, GetMainWindowHandle());
			}
		}
	}

	//-------------------------------------------------------------------
	// GPU Manipulation
	//-------------------------------------------------------------------
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

	void UnbindInputLayout(){
		mPlatformRenderer->UnbindInputLayout();
	}
	
	void UnbindShader(BINDING_SHADER shader){
		mPlatformRenderer->UnbindShader(shader);
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
	// FBRenderer State
	//-------------------------------------------------------------------
	void SetForcedWireFrame(bool enable){
		if (enable && !mWireFrameRS){
			RASTERIZER_DESC RasterizerDesc;
			RasterizerDesc.FillMode = FILL_MODE_WIREFRAME;
			RasterizerDesc.CullMode = CULL_MODE_NONE;
			RasterizerDesc.FrontCounterClockwise = false;
			RasterizerDesc.DepthBias = 0;
			RasterizerDesc.DepthBiasClamp = 0.0f;
			RasterizerDesc.SlopeScaledDepthBias = 0.0f;
			RasterizerDesc.DepthClipEnable = true;
			RasterizerDesc.ScissorEnable = false;
			RasterizerDesc.MultisampleEnable = false;
			RasterizerDesc.AntialiasedLineEnable = false;
			mWireFrameRS = CreateRasterizerState(RasterizerDesc);
		}
		if (mForcedWireframe != enable){
			if (enable)
			{
				mWireFrameRS->Bind();
				mForcedWireframe = true;
			}
			else
			{
				mForcedWireframe = false;
				mDefaultRasterizerState->Bind();
			}
		}
	}

	bool GetForcedWireFrame() const{
		return mForcedWireframe;
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
	const Vec2I& GetMainRenderTargetSize() const{
		auto rt = GetMainRenderTarget();
		if (rt)
		{
			return rt->GetSize();
		}
		return Vec2I::ZERO;
	}

	const Vec2I& GetCurrentRenderTargetSize() const{
		return mCurrentRenderTarget->GetSize();
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

		return mCurrentRTSize;
	}

	const Vec2I& GetRenderTargetSize(HWindow hwnd = 0) const{
		RenderTargetPtr rt = GetRenderTarget(hwnd);
		if (rt){
			return rt->GetSize();
		}
		return mCurrentRTSize;
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
		SetSystemTexture(SystemTextures::Environment, mEnvironmentTexture);
	}

	void SetEnvironmentTextureOverride(TexturePtr texture){
	
	}

	void SetDebugRenderTarget(unsigned idx, const char* textureName){
	
	}

	void SetFadeAlpha(Real alpha){
	
	}

	PointLightManagerPtr GetPointLightMan() const{
		return mPointLightMan;
	}

	void RegisterVideoPlayer(IVideoPlayerPtr player){
		if (!ValueExistsInVector(mVideoPlayers, player)){
			mVideoPlayers.push_back(player);
		}
	}

	void UnregisterVideoPlayer(IVideoPlayerPtr player){
		DeleteValuesInVector(mVideoPlayers, player);
	}

	void GetSampleOffsets_GaussBlur5x5(DWORD texWidth, DWORD texHeight, Vec4** avTexCoordOffset, Vec4** avSampleWeight, Real fMultiplier){
	
	}

	void GetSampleOffsets_DownScale2x2(DWORD texWidth, DWORD texHeight, Vec4* avTexCoordOffset){
	
	}

	bool IsLuminanceOnCpu() const{
		return mLuminanceOnCpu;
	}

	//-------------------------------------------------------------------
	// Queries
	//-------------------------------------------------------------------
	unsigned GetMultiSampleCount() const{
		return 1;
	}

	bool GetFilmicToneMapping() const{
		return mUseFilmicToneMapping;
	}

	bool GetLuminanaceOnCPU() const{
		return mLuminanceOnCpu;
	}

	RenderTargetPtr GetRenderTarget(HWindowId id) const{
		auto it = mWindowRenderTargets.Find(id);
		if (it != mWindowRenderTargets.end()){
			return it->second;
		}
		Logger::Log(FB_ERROR_LOG_ARG, FormatString("Failed to find the render target for the window id(%u)", id).c_str());
		return 0;
	}

	RenderTargetPtr GetRenderTarget(HWindow hwnd) const{
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

	HWindow GetMainWindowHandle(){
		auto it = mWindowHandles.Find(mMainWindowId);
		if (it != mWindowHandles.end())
			return it->second;
		Logger::Log(FB_ERROR_LOG_ARG, "Cannot find maint window handle.");
		return INVALID_HWND;
	}

	HWindowId GetMainWindowHandleId() const{
		return mMainWindowId;
	}

	HWindow GetWindowHandle(HWindowId windowId){
		auto it = mWindowHandles.Find(windowId);
		if (it != mWindowHandles.end())
			return it->second;
		return INVALID_HWND;
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
	RenderOptions* GetOptions() const{
		return mOptions.get();
	}

	lua_State* GetLua() const{
		return mL;
	}

	//-------------------------------------------------------------------
	// ISceneObserver
	//-------------------------------------------------------------------
	void OnAfterMakeVisibleSet(Scene* scene, const RenderParam& renderParam, RenderParamOut* renderParamOut){

	}

	void OnBeforeRenderingOpaques(Scene* scene, const RenderParam& renderParam, RenderParamOut* renderParamOut){
		
	}

	void OnBeforeRenderingTransparents(Scene* scene, const RenderParam& renderParam, RenderParamOut* renderParamOut){
		if (mDebugHud){
			mDebugHud->OnBeforeRenderingTransparents(scene, renderParam, renderParamOut);
		}

		if (mGeomRenderer){
			mGeomRenderer->Render(renderParam, renderParamOut);
		}
	}
	
	//-------------------------------------------------------------------
	// IInputConsumer
	//-------------------------------------------------------------------
	void ConsumeInput(IInputInjectorPtr injector){
		mConsole->ConsumeInput(injector);
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
	if (sRenderer.expired()){
		auto renderer = RendererPtr(FB_NEW(Renderer), [](Renderer* obj){ FB_DELETE(obj); });
		renderer->mImpl->mSelf = renderer;
		sRenderer = renderer;
	}		
	return sRenderer.lock();
}

RendererPtr Renderer::CreateRenderer(const char* renderEngineName, lua_State* L){
	if (sRenderer.expired()){
		auto renderer = CreateRenderer();		
		renderer->SetLuaState(L);
		renderer->PrepareRenderEngine(renderEngineName);
		return renderer;
	}
	else{
		Logger::Log(FB_ERROR_LOG_ARG, "You can create only one renderer!");
		return sRenderer.lock();
	}
}

Renderer& Renderer::GetInstance(){
	return *sRenderer.lock();
}

bool Renderer::DoesExists(){
	return sRenderer.lock() != 0;
}

RendererPtr Renderer::GetInstancePtr(){
	return sRenderer.lock();
}

//-------------------------------------------------------------------
void Renderer::SetLuaState(lua_State* L) {
	mImpl->SetLuaState(L);
}

void Renderer::PrepareRenderEngine(const char* rendererPlugInName) {
	mImpl->PrepareRenderEngine(rendererPlugInName);
}

//-------------------------------------------------------------------
// Canvas & System
//-------------------------------------------------------------------
bool Renderer::InitCanvas(HWindowId id, HWindow window, int width, int height) {
	return mImpl->InitCanvas(id, window, width, height);
}

void Renderer::ReleaseCanvas(HWindowId id) {
	mImpl->ReleaseCanvas(id);
}

void Renderer::Render(TIME_PRECISION dt) {
	mImpl->Render(dt);
}

//-------------------------------------------------------------------
// Resource Creation
//-------------------------------------------------------------------
RenderTargetPtr Renderer::CreateRenderTarget(const RenderTargetParam& param) {
	return mImpl->CreateRenderTarget(param);
}

void Renderer::KeepRenderTargetInPool(RenderTargetPtr rt) {
	mImpl->KeepRenderTargetInPool(rt);
}

TexturePtr Renderer::CreateTexture(const char* file){
	return mImpl->CreateTexture(file, true);
}

TexturePtr Renderer::CreateTexture(const char* file, bool async) {
	return mImpl->CreateTexture(file, async);
}

TexturePtr Renderer::CreateTexture(void* data, int width, int height, PIXEL_FORMAT format, BUFFER_USAGE usage, int  buffer_cpu_access, int texture_type) {
	return mImpl->CreateTexture(data, width, height, format, usage, buffer_cpu_access, texture_type);
}

VertexBufferPtr Renderer::CreateVertexBuffer(void* data, unsigned stride, unsigned numVertices, BUFFER_USAGE usage, BUFFER_CPU_ACCESS_FLAG accessFlag) {
	return mImpl->CreateVertexBuffer(data, stride, numVertices, usage, accessFlag);
}

IndexBufferPtr Renderer::CreateIndexBuffer(void* data, unsigned int numIndices, INDEXBUFFER_FORMAT format) {
	return mImpl->CreateIndexBuffer(data, numIndices, format);
}

ShaderPtr Renderer::CreateShader(const char* filepath, int shaders){
	return mImpl->CreateShader(filepath, shaders, SHADER_DEFINES());
}

ShaderPtr Renderer::CreateShader(const char* filepath, int shaders, const SHADER_DEFINES& defines) {
	return mImpl->CreateShader(filepath, shaders, defines);
}

bool Renderer::ReapplyShaderDefines(Shader* shader) {
	return mImpl->ReapplyShaderDefines(shader);
}

MaterialPtr Renderer::CreateMaterial(const char* file) {
	return mImpl->CreateMaterial(file);
}

MaterialPtr Renderer::GetMissingMaterial() {
	return mImpl->GetMissingMaterial();
}

// use this if you are sure there is instance of the descs.
InputLayoutPtr Renderer::CreateInputLayout(const INPUT_ELEMENT_DESCS& descs, ShaderPtr shader) {
	return mImpl->CreateInputLayout(descs, shader);
}

InputLayoutPtr Renderer::GetInputLayout(DEFAULT_INPUTS::Enum e, ShaderPtr shader) {
	return mImpl->GetInputLayout(e, shader);
}

RasterizerStatePtr Renderer::CreateRasterizerState(const RASTERIZER_DESC& desc) {
	return mImpl->CreateRasterizerState(desc);
}

BlendStatePtr Renderer::CreateBlendState(const BLEND_DESC& desc) {
	return mImpl->CreateBlendState(desc);
}

DepthStencilStatePtr Renderer::CreateDepthStencilState(const DEPTH_STENCIL_DESC& desc) {
	return mImpl->CreateDepthStencilState(desc);
}

SamplerStatePtr Renderer::CreateSamplerState(const SAMPLER_DESC& desc) {
	return mImpl->CreateSamplerState(desc);
}

TextureAtlasPtr Renderer::GetTextureAtlas(const char* path) {
	return mImpl->GetTextureAtlas(path);
}

TextureAtlasRegionPtr Renderer::GetTextureAtlasRegion(const char* path, const char* region) {
	return mImpl->GetTextureAtlasRegion(path, region);
}

TexturePtr Renderer::GetTemporalDepthBuffer(const Vec2I& size) {
	return mImpl->GetTemporalDepthBuffer(size);
}

PointLightPtr Renderer::CreatePointLight(const Vec3& pos, Real range, const Vec3& color, Real intensity, Real lifeTime, bool manualDeletion) {
	return mImpl->CreatePointLight(pos, range, color, intensity, lifeTime, manualDeletion);
}

MaterialPtr Renderer::GetMaterial(DEFAULT_MATERIALS::Enum type) {
	return mImpl->GetMaterial(type);
}

//-------------------------------------------------------------------
// Hot reloading
//-------------------------------------------------------------------
//bool Renderer::ReloadShader(ShaderPtr shader) {
//	return mImpl->ReloadShader(shader);
//}
//
//bool Renderer::ReloadTexture(ShaderPtr shader) {
//	return mImpl->ReloadTexture(shader);
//}

//-------------------------------------------------------------------
// Resource Bindings
//-------------------------------------------------------------------
void Renderer::SetRenderTarget(TexturePtr pRenderTargets[], size_t rtViewIndex[], int num, TexturePtr pDepthStencil, size_t dsViewIndex) {
	mImpl->SetRenderTarget(pRenderTargets, rtViewIndex,  num, pDepthStencil, dsViewIndex);
}

void Renderer::UnbindRenderTarget(TexturePtr renderTargetTexture) {
	mImpl->UnbindRenderTarget(renderTargetTexture);
}

void Renderer::SetViewports(const Viewport viewports[], int num) {
	mImpl->SetViewports(viewports, num);
}

void Renderer::SetScissorRects(Rect rects[], int num) {
	mImpl->SetScissorRects(rects, num);
}

void Renderer::SetVertexBuffers(unsigned int startSlot, unsigned int numBuffers, VertexBufferPtr pVertexBuffers[], unsigned int strides[], unsigned int offsets[]) {
	mImpl->SetVertexBuffers(startSlot, numBuffers, pVertexBuffers, strides, offsets);
}

void Renderer::SetPrimitiveTopology(PRIMITIVE_TOPOLOGY pt) {
	mImpl->SetPrimitiveTopology(pt);
}

void Renderer::SetTextures(TexturePtr pTextures[], int num, BINDING_SHADER shaderType, int startSlot) {
	mImpl->SetTextures(pTextures, num, shaderType, startSlot);
}

void Renderer::SetSystemTexture(SystemTextures::Enum type, TexturePtr texture) {
	mImpl->SetSystemTexture(type, texture);
}

void Renderer::UnbindTexture(BINDING_SHADER shader, int slot) {
	mImpl->UnbindTexture(shader, slot);
}

// pre defined
void Renderer::BindDepthTexture(bool set) {
	mImpl->BindDepthTexture(set);
}

void Renderer::SetDepthWriteShader() {
	mImpl->SetDepthWriteShader();
}

void Renderer::SetDepthWriteShaderCloud() {
	mImpl->SetDepthWriteShaderCloud();
}

void Renderer::SetOccPreShader() {
	mImpl->SetOccPreShader();
}

void Renderer::SetOccPreGSShader() {
	mImpl->SetOccPreGSShader();
}

void Renderer::SetPositionInputLayout() {
	mImpl->SetPositionInputLayout();
}

void Renderer::SetSystemTextureBindings(SystemTextures::Enum type, const TextureBindings& bindings) {
	mImpl->SetSystemTextureBindings(type, bindings);
}

const TextureBindings& Renderer::GetSystemTextureBindings(SystemTextures::Enum type) const {
	return mImpl->GetSystemTextureBindings(type);
}

ShaderPtr Renderer::GetGodRayPS() {
	return mImpl->GetGodRayPS();
}

ShaderPtr Renderer::GetGlowShader() {
	return mImpl->GetGlowShader();
}

void Renderer::SetShadowMapShader() {
	mImpl->SetShadowMapShader();
}

ShaderPtr Renderer::GetSilouetteShader() {
	return mImpl->GetSilouetteShader();
}

ShaderPtr Renderer::GetCopyPS() {
	return mImpl->GetCopyPS();
}

ShaderPtr Renderer::GetCopyPSMS() {
	return mImpl->GetCopyPSMS();
}

TexturePtr Renderer::GetToneMap(unsigned idx) {
	return mImpl->GetToneMap(idx);
}

ShaderPtr Renderer::GetSampleLumInitialShader() {
	return mImpl->GetSampleLumInitialShader();
}

ShaderPtr Renderer::GetSampleLumIterativeShader() {
	return mImpl->GetSampleLumIterativeShader();
}

ShaderPtr Renderer::GetSampleLumFinalShader() {
	return mImpl->GetSampleLumFinalShader();
}

void Renderer::SwapLuminanceMap() {
	mImpl->SwapLuminanceMap();
}

TexturePtr Renderer::GetLuminanceMap(unsigned idx) {
	return mImpl->GetLuminanceMap(idx);
}

ShaderPtr Renderer::GetCalcAdapedLumShader() {
	return mImpl->GetCalcAdapedLumShader();
}

ShaderPtr Renderer::GetBrightPassPS() {
	return mImpl->GetBrightPassPS();
}

ShaderPtr Renderer::GetBlur5x5PS() {
	return mImpl->GetBlur5x5PS();
}

ShaderPtr Renderer::GetBloomPS() {
	return mImpl->GetBloomPS();
}

ShaderPtr Renderer::GetStarGlareShader() {
	return mImpl->GetStarGlareShader();
}

ShaderPtr Renderer::GetMergeTexturePS() {
	return mImpl->GetMergeTexturePS();
}

ShaderPtr Renderer::GetToneMappingPS() {
	return mImpl->GetToneMappingPS();
}

//-------------------------------------------------------------------
// Device RenderStates
//-------------------------------------------------------------------
void Renderer::RestoreRenderStates() {
	mImpl->RestoreRenderStates();
}

void Renderer::RestoreRasterizerState() {
	mImpl->RestoreRasterizerState();
}

void Renderer::RestoreBlendState() {
	mImpl->RestoreBlendState();
}

void Renderer::RestoreDepthStencilState() {
	mImpl->RestoreDepthStencilState();
}

void Renderer::LockDepthStencilState() {
	mImpl->LockDepthStencilState();
}

void Renderer::UnlockDepthStencilState() {
	mImpl->UnlockDepthStencilState();
}

void Renderer::LockBlendState() {
	mImpl->LockBlendState();
}

void Renderer::UnlockBlendState() {
	mImpl->UnlockBlendState();
}

// blend
void Renderer::SetAlphaBlendState() {
	mImpl->SetAlphaBlendState();
}

void Renderer::SetAdditiveBlendState() {
	mImpl->SetAdditiveBlendState();
}

void Renderer::SetMaxBlendState() {
	mImpl->SetMaxBlendState();
}

void Renderer::SetRedAlphaMask() {
	mImpl->SetRedAlphaMask();
}

void Renderer::SetGreenAlphaMask() {
	mImpl->SetGreenAlphaMask();
}

void Renderer::SetGreenMask() {
	mImpl->SetGreenMask();
}

void Renderer::SetBlueMask() {
	mImpl->SetBlueMask();
}

void Renderer::SetNoColorWriteState() {
	mImpl->SetNoColorWriteState();
}

void Renderer::SetGreenAlphaMaskMax() {
	mImpl->SetGreenAlphaMaskMax();
}

void Renderer::SetGreenAlphaMaskAddAddBlend() {
	mImpl->SetGreenAlphaMaskAddAddBlend();
}

void Renderer::SetRedAlphaMaskAddMinusBlend() {
	mImpl->SetRedAlphaMaskAddMinusBlend();
}

void Renderer::SetGreenAlphaMaskAddMinusBlend() {
	mImpl->SetGreenAlphaMaskAddMinusBlend();
}

void Renderer::SetRedAlphaMaskAddAddBlend() {
	mImpl->SetRedAlphaMaskAddAddBlend();
}

// depth
void Renderer::SetNoDepthWriteLessEqual() {
	mImpl->SetNoDepthWriteLessEqual();
}

void Renderer::SetLessEqualDepth() {
	mImpl->SetLessEqualDepth();
}

void Renderer::SetNoDepthStencil() {
	mImpl->SetNoDepthStencil();
}

void Renderer::SetDepthOnlyShader() {
	mImpl->SetDepthOnlyShader();
}

// raster
void Renderer::SetFrontFaceCullRS() {
	mImpl->SetFrontFaceCullRS();
}

void Renderer::SetOneBiasedDepthRS() {
	mImpl->SetOneBiasedDepthRS();
}

// sampler
void Renderer::SetSamplerState(SAMPLERS::Enum s, BINDING_SHADER shader, int slot) {
	mImpl->SetSamplerState(s, shader, slot);
}

//-------------------------------------------------------------------
// Resource Manipulation
//-------------------------------------------------------------------
MapData Renderer::MapVertexBuffer(VertexBufferPtr pBuffer, UINT subResource, MAP_TYPE type, MAP_FLAG flag) {
	return mImpl->MapVertexBuffer(pBuffer, subResource, type, flag);
}

void Renderer::UnmapVertexBuffer(VertexBufferPtr pBuffer, unsigned int subResource) {
	mImpl->UnmapVertexBuffer(pBuffer, subResource);
}

MapData Renderer::MapTexture(TexturePtr pTexture, UINT subResource, MAP_TYPE type, MAP_FLAG flag) {
	return mImpl->MapTexture(pTexture, subResource, type, flag);
}

void Renderer::UnmapTexture(TexturePtr pTexture, UINT subResource) {
	mImpl->UnmapTexture(pTexture, subResource);
}

void Renderer::CopyToStaging(TexturePtr dst, UINT dstSubresource, UINT dstx, UINT dsty, UINT dstz, TexturePtr src, UINT srcSubresource, Box3D* pBox) {
	mImpl->CopyToStaging(dst, dstSubresource, dstx, dsty, dstz, src, srcSubresource, pBox);
}

void Renderer::SaveTextureToFile(TexturePtr texture, const char* filename) {
	mImpl->SaveTextureToFile(texture, filename);
}

//-------------------------------------------------------------------
// GPU constants
//-------------------------------------------------------------------
void Renderer::UpdateObjectConstantsBuffer(const void* pData, bool record) {
	mImpl->UpdateObjectConstantsBuffer(pData, record);
}

void Renderer::UpdatePointLightConstantsBuffer(const void* pData) {
	mImpl->UpdatePointLightConstantsBuffer(pData);
}

void Renderer::UpdateFrameConstantsBuffer() {
	mImpl->UpdateFrameConstantsBuffer();
}

void Renderer::UpdateMaterialConstantsBuffer(const void* pData) {
	mImpl->UpdateMaterialConstantsBuffer(pData);
}

void Renderer::UpdateCameraConstantsBuffer() {
	mImpl->UpdateCameraConstantsBuffer();
}

void Renderer::UpdateRenderTargetConstantsBuffer() {
	mImpl->UpdateRenderTargetConstantsBuffer();
}

void Renderer::UpdateSceneConstantsBuffer() {
	mImpl->UpdateSceneConstantsBuffer();
}

void Renderer::UpdateRareConstantsBuffer() {
	mImpl->UpdateRareConstantsBuffer();
}

void Renderer::UpdateRadConstantsBuffer(const void* pData) {
	mImpl->UpdateRadConstantsBuffer(pData);
}

void* Renderer::MapMaterialParameterBuffer() {
	return mImpl->MapMaterialParameterBuffer();
}

void Renderer::UnmapMaterialParameterBuffer() {
	mImpl->UnmapMaterialParameterBuffer();
}

void* Renderer::MapBigBuffer() {
	return mImpl->MapBigBuffer();
}

void Renderer::UnmapBigBuffer() {
	mImpl->UnmapBigBuffer();
}

//-------------------------------------------------------------------
// GPU Manipulation
//-------------------------------------------------------------------
void Renderer::SetClearColor(HWindowId id, const Color& color) {
	mImpl->SetClearColor(id, color);
}

void Renderer::SetClearDepthStencil(HWindowId id, Real z, UINT8 stencil) {
	mImpl->SetClearDepthStencil(id, z, stencil);
}

void Renderer::Clear(Real r, Real g, Real b, Real a, Real z, UINT8 stencil) {
	mImpl->Clear(r, g, b, a, z, stencil);
}

void Renderer::Clear(Real r, Real g, Real b, Real a) {
	mImpl->Clear(r, g, b, a);
}

// Avoid to use
void Renderer::ClearState() {
	mImpl->ClearState();
}

void Renderer::BeginEvent(const char* name) {
	mImpl->BeginEvent(name);
}

void Renderer::EndEvent() {
	mImpl->EndEvent();
}

void Renderer::TakeScreenshot() {
	mImpl->TakeScreenshot();
}

//-------------------------------------------------------------------
// FBRenderer State
//-------------------------------------------------------------------
void Renderer::SetForcedWireFrame(bool enable) {
	mImpl->SetForcedWireFrame(enable);
}

bool Renderer::GetForcedWireFrame() const {
	return mImpl->GetForcedWireFrame();
}

RenderTargetPtr Renderer::GetMainRenderTarget() const {
	return mImpl->GetMainRenderTarget();
}

ScenePtr Renderer::GetMainScene() const {
	return mImpl->GetMainScene();
}

const Vec2I& Renderer::GetMainRenderTargetSize() const {
	return mImpl->GetMainRenderTargetSize();
}

void Renderer::SetCurrentRenderTarget(RenderTargetPtr renderTarget) {
	mImpl->SetCurrentRenderTarget(renderTarget);
}

RenderTargetPtr Renderer::GetCurrentRenderTarget() const {
	return mImpl->GetCurrentRenderTarget();
}

bool Renderer::IsMainRenderTarget() const {
	return mImpl->IsMainRenderTarget();
}

const Vec2I& Renderer::GetRenderTargetSize(HWindowId id) const {
	return mImpl->GetRenderTargetSize(id);
}

const Vec2I& Renderer::GetRenderTargetSize(HWindow hwnd) const {
	return mImpl->GetRenderTargetSize(hwnd);
}

void Renderer::SetDirectionalLight(DirectionalLightPtr pLight, int idx) {
	mImpl->SetDirectionalLight(pLight, idx);
}

DirectionalLightPtr Renderer::GetDirectionalLight(int idx) const {
	return mImpl->GetDirectionalLight(idx);
}

DirectionalLightPtr Renderer::GetMainDirectionalLight(int idx) const {
	return mImpl->GetMainDirectionalLight(idx);
}

void Renderer::InitFrameProfiler(Real dt) {
	mImpl->InitFrameProfiler(dt);
}

const RENDERER_FRAME_PROFILER& Renderer::GetFrameProfiler() const {
	return mImpl->GetFrameProfiler();
}

inline FontPtr Renderer::GetFont(Real fontHeight) const {
	return mImpl->GetFont(fontHeight);
}

const INPUT_ELEMENT_DESCS& Renderer::GetInputElementDesc(DEFAULT_INPUTS::Enum e) {
	return mImpl->GetInputElementDesc(e);
}

void Renderer::SetEnvironmentTexture(TexturePtr pTexture) {
	mImpl->SetEnvironmentTexture(pTexture);
}

void Renderer::SetEnvironmentTextureOverride(TexturePtr texture) {
	mImpl->SetEnvironmentTextureOverride(texture);
}

void Renderer::SetDebugRenderTarget(unsigned idx, const char* textureName) {
	mImpl->SetDebugRenderTarget(idx, textureName);
}

void Renderer::SetFadeAlpha(Real alpha) {
	mImpl->SetFadeAlpha(alpha);
}

PointLightManagerPtr Renderer::GetPointLightMan() const {
	return mImpl->GetPointLightMan();
}

void Renderer::RegisterVideoPlayer(IVideoPlayerPtr player) {
	mImpl->RegisterVideoPlayer(player);
}

void Renderer::UnregisterVideoPlayer(IVideoPlayerPtr player) {
	mImpl->UnregisterVideoPlayer(player);
}

void Renderer::GetSampleOffsets_GaussBlur5x5(DWORD texWidth, DWORD texHeight, Vec4** avTexCoordOffset, Vec4** avSampleWeight, Real fMultiplier) {
	mImpl->GetSampleOffsets_GaussBlur5x5(texWidth, texHeight, avTexCoordOffset, avSampleWeight, fMultiplier);
}

void Renderer::GetSampleOffsets_DownScale2x2(DWORD texWidth, DWORD texHeight, Vec4* avTexCoordOffset) {
	mImpl->GetSampleOffsets_DownScale2x2(texWidth, texHeight, avTexCoordOffset);
}

bool Renderer::IsLuminanceOnCpu() const {
	return mImpl->IsLuminanceOnCpu();
}

//-------------------------------------------------------------------
// Queries
//-------------------------------------------------------------------
unsigned Renderer::GetMultiSampleCount() const {
	return mImpl->GetMultiSampleCount();
}

bool Renderer::GetFilmicToneMapping() const{
	return mImpl->GetFilmicToneMapping();
}

bool Renderer::GetLuminanaceOnCPU() const{
	return mImpl->GetLuminanaceOnCPU();
}

RenderTargetPtr Renderer::GetRenderTarget(HWindowId id) const {
	return mImpl->GetRenderTarget(id);
}

void Renderer::SetCamera(CameraPtr pCamera) {
	mImpl->SetCamera(pCamera);
}

CameraPtr Renderer::GetCamera() const {
	return mImpl->GetCamera();
}

CameraPtr Renderer::GetMainCamera() const {
	return mImpl->GetMainCamera();
}

HWindow Renderer::GetMainWindowHandle() const {
	return mImpl->GetMainWindowHandle();
}

HWindowId Renderer::GetMainWindowHandleId(){
	return mImpl->GetMainWindowHandleId();
}

HWindow Renderer::GetWindowHandle(RenderTargetId rtId) {
	return mImpl->GetWindowHandle(rtId);
}

Vec2I Renderer::ToSreenPos(HWindowId id, const Vec3& ndcPos) const {
	return mImpl->ToSreenPos(id, ndcPos);
}

Vec2 Renderer::ToNdcPos(HWindowId id, const Vec2I& screenPos) const {
	return mImpl->ToNdcPos(id, screenPos);
}

unsigned Renderer::GetNumLoadingTexture() const {
	return mImpl->GetNumLoadingTexture();
}

Vec2I Renderer::FindClosestSize(HWindowId id, const Vec2I& input) {
	return mImpl->FindClosestSize(id, input);
}

bool Renderer::GetResolutionList(unsigned& outNum, Vec2I* list) {
	return mImpl->GetResolutionList(outNum, list);
}

RenderOptions* Renderer::GetOptions() const {
	return mImpl->GetOptions();
}

lua_State* Renderer::GetLua() const {
	return mImpl->GetLua();
}

//-------------------------------------------------------------------
// Drawing
//-------------------------------------------------------------------
void Renderer::DrawIndexed(unsigned indexCount, unsigned startIndexLocation, unsigned startVertexLocation) {
	mImpl->DrawIndexed(indexCount, startIndexLocation, startVertexLocation);
}

void Renderer::Draw(unsigned int vertexCount, unsigned int startVertexLocation) {
	mImpl->Draw(vertexCount, startVertexLocation);
}

void Renderer::DrawFullscreenQuad(ShaderPtr pixelShader, bool farside) {
	mImpl->DrawFullscreenQuad(pixelShader, farside);
}

void Renderer::DrawTriangle(const Vec3& a, const Vec3& b, const Vec3& c, const Vec4& color, MaterialPtr mat) {
	mImpl->DrawTriangle(a, b, c, color, mat);
}

void Renderer::DrawQuad(const Vec2I& pos, const Vec2I& size, const Color& color){
	mImpl->DrawQuad(pos, size, color, true);
}

void Renderer::DrawQuad(const Vec2I& pos, const Vec2I& size, const Color& color, bool updateRs) {
	mImpl->DrawQuad(pos, size, color, updateRs);
}

void Renderer::DrawQuadWithTexture(const Vec2I& pos, const Vec2I& size, const Color& color, TexturePtr texture, MaterialPtr materialOverride) {
	mImpl->DrawQuadWithTexture(pos, size, color, texture, materialOverride);
}

void Renderer::DrawQuadWithTextureUV(const Vec2I& pos, const Vec2I& size, const Vec2& uvStart, const Vec2& uvEnd, const Color& color, TexturePtr texture, MaterialPtr materialOverride) {
	mImpl->DrawQuadWithTextureUV(pos, size, uvStart, uvEnd, color, texture, materialOverride);
}

void Renderer::DrawBillboardWorldQuad(const Vec3& pos, const Vec2& size, const Vec2& offset, DWORD color, MaterialPtr pMat) {
	mImpl->DrawBillboardWorldQuad(pos, size, offset, color, pMat);
}

void Renderer::QueueDrawText(const Vec2I& pos, WCHAR* text, const Color& color){
	mImpl->QueueDrawText(pos, text, color, defaultFontSize);
}

void Renderer::QueueDrawText(const Vec2I& pos, WCHAR* text, const Color& color, Real size) {
	mImpl->QueueDrawText(pos, text, color, size);
}

void Renderer::QueueDrawText(const Vec2I& pos, const char* text, const Color& color){
	mImpl->QueueDrawText(pos, text, color, defaultFontSize);
}

void Renderer::QueueDrawText(const Vec2I& pos, const char* text, const Color& color, Real size) {
	mImpl->QueueDrawText(pos, text, color, size);
}

void Renderer::QueueDraw3DText(const Vec3& worldpos, WCHAR* text, const Color& color){
	mImpl->QueueDraw3DText(worldpos, text, color, defaultFontSize);
}

void Renderer::QueueDraw3DText(const Vec3& worldpos, WCHAR* text, const Color& color, Real size) {
	mImpl->QueueDraw3DText(worldpos, text, color, size);
}

void Renderer::QueueDraw3DText(const Vec3& worldpos, const char* text, const Color& color){
	mImpl->QueueDraw3DText(worldpos, text, color, defaultFontSize);
}

void Renderer::QueueDraw3DText(const Vec3& worldpos, const char* text, const Color& color, Real size) {
	mImpl->QueueDraw3DText(worldpos, text, color, size);
}

void Renderer::QueueDrawTextForDuration(Real secs, const Vec2I& pos, WCHAR* text, const Color& color){
	mImpl->QueueDrawTextForDuration(secs, pos, text, color, defaultFontSize);
}

void Renderer::QueueDrawTextForDuration(Real secs, const Vec2I& pos, WCHAR* text, const Color& color, Real size) {
	mImpl->QueueDrawTextForDuration(secs, pos, text, color, size);
}

void Renderer::QueueDrawTextForDuration(Real secs, const Vec2I& pos, const char* text, const Color& color){
	mImpl->QueueDrawTextForDuration(secs, pos, text, color, defaultFontSize);
}

void Renderer::QueueDrawTextForDuration(Real secs, const Vec2I& pos, const char* text, const Color& color, Real size) {
	mImpl->QueueDrawTextForDuration(secs, pos, text, color, size);
}

void Renderer::ClearDurationTexts() {
	mImpl->ClearDurationTexts();
}

void Renderer::QueueDrawLine(const Vec3& start, const Vec3& end, const Color& color0, const Color& color1) {
	mImpl->QueueDrawLine(start, end, color0, color1);
}

void Renderer::QueueDrawLine(const Vec2I& start, const Vec2I& end, const Color& color0, const Color& color1) {
	mImpl->QueueDrawLine(start, end, color0, color1);
}

void Renderer::QueueDrawLineBeforeAlphaPass(const Vec3& start, const Vec3& end, const Color& color0, const Color& color1) {
	mImpl->QueueDrawLineBeforeAlphaPass(start, end, color0, color1);
}

void Renderer::QueueDrawQuad(const Vec2I& pos, const Vec2I& size, const Color& color) {
	mImpl->QueueDrawQuad(pos, size, color);
}

void Renderer::QueueDrawTexturedThickLine(const Vec3& start, const Vec3& end, const Color& color0, const Color& color1, Real thickness, const char* texture, bool textureFlow) {
	mImpl->QueueDrawTexturedThickLine(start, end, color0, color1, thickness, texture, textureFlow);
}

void Renderer::QueueDrawSphere(const Vec3& pos, Real radius, const Color& color) {
	mImpl->QueueDrawSphere(pos, radius, color);
}

void Renderer::QueueDrawBox(const Vec3& boxMin, const Vec3& boxMax, const Color& color, Real alpha) {
	mImpl->QueueDrawBox(boxMin, boxMax, color, alpha);
}

void Renderer::QueueDrawTriangle(const Vec3& a, const Vec3& b, const Vec3& c, const Color& color, Real alpha) {
	mImpl->QueueDrawTriangle(a, b, c, color, alpha);
}

void Renderer::QueueDrawQuadLine(const Vec2I& pos, const Vec2I& size, const Color& color) {
	mImpl->QueueDrawQuadLine(pos, size, color);
}

//-------------------------------------------------------------------
// Internal
//-------------------------------------------------------------------
void Renderer::GatherPointLightData(const BoundingVolume* aabb, const Transformation& transform, POINT_LIGHT_CONSTANTS* plConst) {
	mImpl->GatherPointLightData(aabb, transform, plConst);
}

void Renderer::RefreshPointLight() {
	mImpl->RefreshPointLight();
}

bool Renderer::NeedToRefreshPointLight() const {
	return mImpl->NeedToRefreshPointLight();
}

void Renderer::RenderDebugHud() {
	mImpl->RenderDebugHud();
}

//-------------------------------------------------------------------
// ISceneObserver
//-------------------------------------------------------------------
void Renderer::OnAfterMakeVisibleSet(Scene* scene, const RenderParam& renderParam, RenderParamOut* renderParamOut) {
	mImpl->OnAfterMakeVisibleSet(scene, renderParam, renderParamOut);
}

void Renderer::OnBeforeRenderingOpaques(Scene* scene, const RenderParam& renderParam, RenderParamOut* renderParamOut) {
	mImpl->OnBeforeRenderingOpaques(scene, renderParam, renderParamOut);
}

void Renderer::OnBeforeRenderingTransparents(Scene* scene, const RenderParam& renderParam, RenderParamOut* renderParamOut) {
	mImpl->OnBeforeRenderingTransparents(scene, renderParam, renderParamOut);
}

//-------------------------------------------------------------------
// ISceneObserver
//-------------------------------------------------------------------
void Renderer::ConsumeInput(IInputInjectorPtr injector) {
	mImpl->ConsumeInput(injector);
}

 /// inject to main camera