#include "stdafx.h"
#include "Renderer.h"
#include "IRenderer.h"
#include "RendererEnums.h"
#include "RendererStructs.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "RenderResourceFactory.h"
#include "IRenderable.h"
#include "ITexture.h"
#include "FBCommonHeaders/VectorMap.h"
#include "FBCommonHeaders/Factory.h"
#include "FBSystemLib/ModuleHandler.h"
#include "FBFileSystem/FileSystem.h"

using namespace fastbird;

DECLARE_SMART_PTR(UI3DObj);
DECLARE_SMART_PTR(UIObject);
DECLARE_SMART_PTR(SkySphere);
DECLARE_SMART_PTR(DebugHud);
DECLARE_SMART_PTR(GeometryRenderer);
class Renderer::RendererImpl{
public:
	typedef fastbird::Factory<IRenderer>::CreateCallback CreateCallback;
	typedef std::vector<RenderTargetWeakPtr> RenderTargets;

	Renderer* mObject;
	std::string mRenderEngineType;
	IRendererPtr mRenderEngine;
	ModuleHandle mLoadedModule;

	VectorMap<HWindowId, HWindow> mWindowHandles;
	VectorMap<HWindow, HWindowId> mWindowIds;
	VectorMap<HWindowId, Vec2I> mWindowSizes;
	HWindowId mMainWindowId;
	VectorMap<HWindowId, RenderTargetPtr> mCanvasRenderTargets;
	RenderTarget* mCurrentRenderTarget;	
	Vec2I mCurRTSize;
	std::vector<RenderTargetPtr> mRenderTargetPool;		
	RenderTargets mRenderTargetsEveryFrame;
	RenderTargets mRenderTargets;

	typedef VectorMap<std::string, TextureWeakPtr> Textures;
	Textures mTextures;	

	DirectionalLightPtr		mDirectionalLight[2];
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
	ITexturePtr mEnvironmentTextureOverride;

	bool					mForcedWireframe;

	CameraPtr				mCamera;
	RENDERER_FRAME_PROFILER		mFrameProfiler;
	ShaderPtr mBindedShader;
	InputLayoutPtr mBindedInputLayout;
	PRIMITIVE_TOPOLOGY mCurrentTopology;

	typedef std::map<INPUT_ELEMENT_DESCS, InputLayoutPtr > INPUTLAYOUT_MAP;
	INPUTLAYOUT_MAP mInputLayouts;


	INPUT_ELEMENT_DESCS mInputLayoutDescs[DEFAULT_INPUTS::COUNT];
	const int DEFAULT_DYN_VERTEX_COUNTS=100;
	VertexBufferPtr mDynVBs[DEFAULT_INPUTS::COUNT];

	typedef VectorMap< std::string, TexturePtr > TextureCache;
	TextureCache mTextureCache;

	typedef std::vector< TextureAtlasPtr > TextureAtlasCache;
	TextureAtlasCache mTextureAtalsCache;

	typedef VectorMap< std::string, MaterialPtr > MaterialCache;
	MaterialCache mMaterialCache;
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

	float mMiddleGray;
	float mStarPower;
	float mBloomPower;

	struct DebugRenderTarget
	{
		Vec2 mPos;
		Vec2 mSize;

		TexturePtr mTexture;
	};
	static const unsigned MaxDebugRenderTargets = 4;
	DebugRenderTarget mDebugRenderTargets[MaxDebugRenderTargets];

	PointLightMan* mPointLightMan;
	bool mRefreshPointLight;
	bool mLuminanceOnCpu;
	bool mUseFilmicToneMapping;
	float mLuminance;
	unsigned mFrameLuminanceCalced;
	float mFadeAlpha;

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

	bool mTakeScreenShot;
	
	//-----------------------------------------------------------------------
	RendererImpl(Renderer* renderer)
		: mObject(renderer)
		, mLoadedModule(0)
	{
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

		mRenderEngineType = type;		
		auto module = ModuleHandler::LoadModule(mRenderEngineType.c_str());
		if (module){
			mLoadedModule = module;
			typedef fastbird::IRendererPtr(*Create)();
			Create createCallback = (Create)ModuleHandler::GetFunction(module, "CreateRenderEngine");
			if (createCallback){				
				mRenderEngine = createCallback();
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
		}

		mWindowHandles[id] = window;
		mWindowIds[window] = id;
		mWindowSizes[id] = { width, height };		
		ITexturePtr colorTexture;
		ITexturePtr depthTexture;
		mRenderEngine->InitCanvas(id, window, width, height, colorTexture, depthTexture);
		if (colorTexture && depthTexture){
			RenderTargetParam param;
			param.mSize = { width, height };
			param.mWillCreateDepth = true;
			auto rt = CreateRenderTarget(param);
			
			rt->SetColorTexture(CreateTexture(colorTexture));
			rt->SetDepthTexture(CreateTexture(depthTexture));
			return true;
		}
		else{
			Logger::Log(FB_ERROR_LOG_ARG, "Failed to create cavas");
			return false;
		}		
	}

	void ReleaseCanvas(HWindowId id){
		auto it = mWindowHandles.Find(id);
		if (it == mWindowHandles.end()){
			Logger::Log(FB_ERROR_LOG_ARG, "Cannot find the window.");
			return;
		}

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

	TexturePtr CreateTexture(const char* file, bool async){
		return CreateTexture(file, 0, async);
	}

	TexturePtr CreateTexture(const char* file, Texture* reloading, bool async){
		if (!file || !strlen(file)){
			Logger::Log(FB_ERROR_LOG_ARG, "invalid arg.");
			return 0;
		}
		std::string filepath(file);
		ToLowerCase(filepath);
		if (!reloading){
			
			auto it = mTextures.Find(filepath);
			if (it != mTextures.end()){
				auto texture = it->second.lock();
				if (texture){
					return texture->Clone();
				}
			}
		}
		if (!FileSystem::Exists(file)){
			Logger::Log(FB_ERROR_LOG_ARG, FormatString("File(%s) not found", file));
			return 0;
		}
		auto texture = RenderResourceFactory::CreateResource<Texture>();
		texture->LoadFile(file, async);
		mTextures[filepath] = texture;
		return texture;
	}

	TexturePtr CreateTexture(ITexturePtr platformTexture){
		auto texture = RenderResourceFactory::CreateResource<Texture>();
		texture->SetPlatformTexture(platformTexture);
		return texture;
	}

	//-------------------------------------------------------------------
	// Resource Manipulation
	//-------------------------------------------------------------------
	void ReloadTexture(const char* texturePath){		
		std::string lower = texturePath;
		ToLowerCase(lower);
		for (auto it = mTextures.begin(); it != mTextures.end(); /**/){
			auto texture = it->second.lock();
			if (!texture){
				it = mTextures.erase(it);
				continue;
			}
			++it;

			if (!texture->GetAdamTexture())
			{
				if (texture->GetName() == lower)
				{
					mObject->CreateTexture(lower.c_str(), texture.get(), true);
				}
			}
		}		
	}

	HWindow GetWindowHandle(RenderTargetId rtId){
		assert(0);
		return 0;
	}
};

//---------------------------------------------------------------------------
Renderer::Renderer()
	: mImpl(new RendererImpl(this))
{

}

Renderer::~Renderer(){
	delete mImpl;
}

static RendererWeakPtr sRenderer;

RendererPtr Renderer::CreateRenderer(){
	if (sRenderer.lock()){
		Logger::Log(FB_DEFAULT_LOG_ARG, "(error) You can create only one renderer!");
		return 0;
	}
	auto renderer = RendererPtr(FB_NEW(Renderer), [](Renderer* obj){ delete obj; });
	renderer->mMe = renderer;
	sRenderer = renderer;
	return renderer;
}

RendererPtr Renderer::CreateRenderer(const char* renderEngineName){
	if (sRenderer.lock()){
		Logger::Log(FB_DEFAULT_LOG_ARG, "(error) You can create only one renderer!");
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

RenderTargetPtr Renderer::CreateRenderTarget(const RenderTargetParam& param){
	return mImpl->CreateRenderTarget(param);
}

void Renderer::KeepRenderTargetInPool(RenderTargetPtr rt){
	mImpl->KeepRenderTargetInPool(rt);
}

TexturePtr Renderer::CreateTexture(const char* file, bool async){
	return mImpl->CreateTexture(file, async);
}

TexturePtr Renderer::CreateTexture(const char* file, Texture* reloading, bool async){
	return mImpl->CreateTexture(file, reloading, async);
}

//-------------------------------------------------------------------
// Resource Management
//-------------------------------------------------------------------
void Renderer::ReloadTexture(const char* texturePath){
	mImpl->ReloadTexture(texturePath);
}

//-------------------------------------------------------------------
void Renderer::SetCurrentRenderTarget(RenderTarget* renderTarget){
	mImpl->mCurrentRenderTarget = renderTarget;
}

HWindow Renderer::GetWindowHandle(RenderTargetId rtId){
	return mImpl->GetWindowHandle(rtId);
}