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
#include "FBStringLib/StringConverter.h"
#include "FBCommonHeaders/VectorMap.h"
#include "FBCommonHeaders/Factory.h"
#include "FBSystemLib/ModuleHandler.h"
#include "FBFileSystem/FileSystem.h"
#include "FBSceneManager/Scene.h"

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

	Real mMiddleGray;
	Real mStarPower;
	Real mBloomPower;

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

	bool mTakeScreenShot;
	
	//-----------------------------------------------------------------------
	Impl(Renderer* renderer)
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
		IPlatformTexturePtr platformTexture = mPlatformRenderer->CreateTexture(file, async);
		if (!platformTexture){
			Logger::Log(FB_ERROR_LOG_ARG, FormatString("Platform renderer failed to load a texture(%s)", file).c_str());

		}
		return CreateTexture(platformTexture);		
	}

	TexturePtr CreateTexture(IPlatformTexturePtr platformTexture){
		auto texture = RenderResourceFactory::CreateResource<Texture>();
		texture->SetPlatformTexture(platformTexture);
		return texture;
	}

	HWindow GetWindowHandle(RenderTargetId rtId){
		assert(0);
		return 0;
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

	RenderTargetPtr GetRenderTarget(HWindowId id) const{
		auto rtit = mWindowRenderTargets.Find(id);
		if (rtit == mWindowRenderTargets.end()){
			Logger::Log(FB_ERROR_LOG_ARG, FormatString("Failed to find the RenderTarget with window id(%d)", id).c_str());
			return 0;
		}
		return rtit->second;
	}

	RenderTargetPtr GetRenderTarget(HWindow hwnd) const{
		auto it = mWindowIds.Find(hwnd);
		if (it == mWindowIds.end()){
			Logger::Log(FB_ERROR_LOG_ARG, "Failed to find window Id");
			return 0;
		}
		return GetRenderTarget(it->second);		
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
		IFont* bestFont = mFonts.begin()->second;
		for (auto it : mFonts){
			auto newGap = std::abs(requestedHeight - it.first);
			if (newGap < curGap){
				bestMatchHeight = it.first;
				curGap = newGap;
				bestFont = it.second;
			}
		}
		if (!bestFont){
			Error("Font not found with size %f", fontHeight);
		}
		else{
			bestFont->SetHeight(fontHeight);
		}
		return bestFont;
	}

	const INPUT_ELEMENT_DESCS& GetInputElementDesc(
		DEFAULT_INPUTS::Enum e){
	
	}

	void SetEnvironmentTexture(TexturePtr pTexture){
	
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

const Vec2I& Renderer::GetRenderTargetSize(HWindow hwnd = 0) const{
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
	mImpl->IsLuminanceOnCpu();
}
