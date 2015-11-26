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
#include "EngineFacade.h"
#include "EngineOptions.h"
#include "FBTimer/Profiler.h"
#include "FBRenderer/RenderTarget.h"
#include "FBRenderer/Camera.h"
#include "FBRenderer/RendererOptions.h"
#include "FBSceneManager/SceneManager.h"
#include "FBSceneManager/Scene.h"
#include "FBSceneManager/DirectionalLight.h"
#include "FBSceneObjectFactory/SceneObjectFactory.h"
#include "FBThreadLib/TaskScheduler.h"
#include "FBConsole/Console.h"
#include "FBInputManager/InputManager.h"
#include "FBFileMonitor/FileMonitor.h"
#include "FBVideoPlayer/VideoPlayerOgg.h"
using namespace fastbird;
class EngineFacade::Impl{
public:
	static const int MainWindowId = 1;	
	std::map<HWindowId, HWindow> mWindowById;
	std::map<HWindow, HWindowId> mWindowIdByHandle;
	HWindowId mNextWindowId;
	lua_State* mL;
	
	// Engine Objects
	ConsolePtr mConsole;
	TaskSchedulerPtr mTaskSchedular;
	InputManagerPtr mInputManager;
	RendererPtr mRenderer;
	SceneManagerPtr mSceneManager;
	ScenePtr mMainScene;
	SceneObjectFactoryPtr mSceneObjectFactory;
	CameraPtr mMainCamera;
	EngineOptionsPtr mEngineOptions;
	bool mRayFromCursorCalced;

	//---------------------------------------------------------------------------
	Impl()
		: mL(0)
		, mNextWindowId(MainWindowId)
		, mRayFromCursorCalced(false)
		, mMainCamera(Camera::Create())
	{
		mL = LuaUtils::OpenLuaState();
		mConsole = Console::Create();
		mTaskSchedular = TaskScheduler::Create(6);
		mInputManager = InputManager::Create();
		mSceneManager = SceneManager::Create();
		if (!mSceneManager){
			Logger::Log(FB_ERROR_LOG_ARG, "Cannot create SceneManager.");
		}
		else{
			mMainScene = mSceneManager->CreateScene("MainScene");
			if (!mMainScene){
				Logger::Log(FB_ERROR_LOG_ARG, "Cannot create the main scene.");
			}
		}

		mSceneObjectFactory = SceneObjectFactory::Create();
		if (!mSceneObjectFactory){
			Logger::Log(FB_ERROR_LOG_ARG, "SceneObjectFactory is not initialized.");
		}

		mEngineOptions = EngineOptions::Create();

		mRenderer = Renderer::Create();
	}

	~Impl(){
		LuaUtils::CloseLuaState(mL);
	}

	HWindowId FindEmptyHwndId()
	{
		return mNextWindowId++;
	}

	HWindowId CreateEngineWindow(int x, int y, int width, int height, const char* wndClass, 
		const char* title, unsigned style, unsigned exStyle, WNDPROC winProc)
	{
		Vec2I resol(width, height);
		if (width == 0 || height == 0){
			// check the config
			resol = Renderer::GetInstance().GetRendererOptions()->r_resolution;
			width = resol.x;
			height = resol.y;
		}
		else{
			Renderer::GetInstance().GetRendererOptions()->r_resolution = resol;
		}

		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = width;
		rect.bottom = height;
		AdjustWindowRect(&rect, style, false);

		int eWidth = rect.right - rect.left;
		int eHeight = rect.bottom - rect.top;
		WNDCLASSEX wndclass = { sizeof(WNDCLASSEX), CS_DBLCLKS, winProc,
			0, 0, GetModuleHandle(0), LoadIcon(0, IDI_APPLICATION),
			LoadCursor(0, IDC_ARROW), HBRUSH(COLOR_WINDOW + 1),
			0, wndClass, LoadIcon(0, IDI_APPLICATION) };

		WNDCLASSEX classInfo;
		BOOL registered = GetClassInfoEx(GetModuleHandle(NULL), wndClass, &classInfo);
		if (!registered)
		{
			registered = RegisterClassEx(&wndclass);
		}
		if (registered)
		{
			auto hWnd = CreateWindowEx(exStyle, wndClass, title,
				style, x, y,
				eWidth, eHeight, 0, 0, GetModuleHandle(0), 0);
			auto id = FindEmptyHwndId();
			if (id == MainWindowId){
				Renderer::GetInstance().SetMainWindowStyle(style);
				InputManager::GetInstance().SetMainWindowHandle((HWindow)hWnd);
			}

			mWindowById[id] = (HWindow)hWnd;
			mWindowIdByHandle[(HWindow)hWnd] = id;			
			ShowWindow(hWnd, TRUE);
			return id;
		}
		return INVALID_HWND_ID;
	}

	bool InitRenderer(const char* pluginName){		
		return Renderer::GetInstance().PrepareRenderEngine(pluginName);		
	}

	bool InitCanvas(HWindowId id, int width, int height){
		if (!mRenderer){
			Logger::Log(FB_ERROR_LOG_ARG, "Renderer is not initialized.");
			return false;
		}
		auto window = mWindowById[id];		
		bool sucess = mRenderer->InitCanvas(id, window, width, height);
		if (!sucess){
			return INVALID_HWND_ID;
		}
		else{			
			if (id == MainWindowId){
				auto rt = mRenderer->GetRenderTarget(id);
				if (!rt){
					Logger::Log(FB_ERROR_LOG_ARG, "Main RenderTarget is not initialized.");
				}
				else{
					rt->RegisterScene(mMainScene);
					rt->SetCamera(mMainCamera);
				}
			}

			return true;
		}
	}

	ScenePtr GetMainScene() const{
		return mMainScene;
	}

	void UpdateInput(){
		mRayFromCursorCalced = false;
		mInputManager->Update();
	}

	void Update(TIME_PRECISION dt){
		mRenderer->Update(dt);
		mSceneManager->Update(dt);
		mSceneObjectFactory->Update(dt);
	}

	void Render(){
		mRenderer->Render();
	}

	const Ray3& GetWorldRayFromCursor(){
		static Ray3 worldRay(Vec3::ZERO, Vec3::UNIT_Y);
		if (mRayFromCursorCalced){
			return worldRay;
		}

		auto cam = mMainCamera;
		if (cam)
		{
			mRayFromCursorCalced = true;
			auto injector = mInputManager->GetInputInjector();
			long x, y;
			injector->GetMousePos(x, y);
			worldRay = cam->ScreenPosToRay(x, y);
		}
		else
		{
			Logger::Log(FB_ERROR_LOG_ARG, "No main camera found.");
		}

		return worldRay;
	}

	void AddDirectionalLightCoordinates(DirectionalLightIndex::Enum idx, Real phi, Real theta){
		if (!mMainScene){
			Logger::Log(FB_ERROR_LOG_ARG, "No main scene found.");
			return;
		}
		auto light = mMainScene->GetDirectionalLight(idx);
		assert(light);
		light->AddPhi(phi);
		light->AddTheta(theta);
	}
};

//---------------------------------------------------------------------------
static EngineFacadeWeakPtr sFacade;
EngineFacadePtr EngineFacade::Create(){
	if (sFacade.expired()){
		EngineFacadePtr p(new EngineFacade, [](EngineFacade* obj){ delete obj; });
		sFacade = p;
		return p;
	}
	return sFacade.lock();
}
FB_IMPLEMENT_STATIC_CREATE(EngineFacade);
EngineFacade& EngineFacade::GetInstance(){
	if (sFacade.expired()){
		Logger::Log(FB_ERROR_LOG_ARG, "EngineFacade is already deleted. Program will crash.");
	}
	return *sFacade.lock();
}

bool EngineFacade::HasInstance(){
	return !sFacade.expired();
}

EngineFacade::EngineFacade()
	:mImpl(new Impl)
{

}
EngineFacade::~EngineFacade(){

}

HWindowId EngineFacade::CreateEngineWindow(int x, int y, int width, int height,
	const char* wndClass, const char* title, unsigned style, unsigned exStyle,
	WNDPROC winProc){
	mImpl->CreateEngineWindow(x, y, width, height, wndClass, title, style, exStyle, winProc);
}

bool EngineFacade::InitRenderer(const char* pluginName) {
	return mImpl->InitRenderer(pluginName);
}

bool EngineFacade::InitCanvas(HWindowId id, int width, int height) {
	return mImpl->InitCanvas(id, width, height);
}

ScenePtr EngineFacade::GetMainScene() const {
	return mImpl->GetMainScene();
}

void EngineFacade::UpdateInput(){
	mImpl->UpdateInput();
}

void EngineFacade::Update(TIME_PRECISION dt) {
	mImpl->Update(dt);
}

void EngineFacade::Render() {
	mImpl->Render();
}

EngineOptionsPtr EngineFacade::GetEngineOptions() const{
	return mImpl->mEngineOptions;
}

bool EngineFacade::MainCameraExists() const{
	return Renderer::GetInstance().GetMainCamera() != 0;
}

ICameraPtr EngineFacade::GetMainCamera() const{
	return Renderer::GetInstance().GetMainCamera();
}

const Vec3& EngineFacade::GetMainCameraPos() const{
	return Renderer::GetInstance().GetMainCamera()->GetPosition();
}

const Vec3& EngineFacade::GetMainCameraDirection() const{
	return Renderer::GetInstance().GetMainCamera()->GetDirection();
}

const Ray3& EngineFacade::GetWorldRayFromCursor(){
	return mImpl->GetWorldRayFromCursor();
}

IInputInjectorPtr EngineFacade::GetInputInjector(){
	return InputManager::GetInstance().GetInputInjector();
}

void EngineFacade::AddDirectionalLightCoordinates(DirectionalLightIndex::Enum idx, Real phi, Real theta){
	mImpl->AddDirectionalLightCoordinates(idx, phi, theta);
}

void EngineFacade::DrawProfileResult(const ProfilerSimple& profiler, const char* posVarName){
	DrawProfileResult(profiler, posVarName, 0);
}

void EngineFacade::DrawProfileResult(const ProfilerSimple& profiler, const char* posVarName, int tab){
	wchar_t buf[256];
	std::wstring tapString;
	while (tab--)
	{
		tapString.push_back('\t');
	}
	swprintf_s(buf, L"%s%s : %f", tapString.c_str(), profiler.GetName(), profiler.GetDT());
	Vec2I pos = LuaUtils::GetLuaVarAsVec2I(posVarName);
	Renderer::GetInstance().QueueDrawText(pos, buf, Color::White);
}

intptr_t EngineFacade::WinProc(HWindow window, unsigned msg, uintptr_t wp, uintptr_t lp){
#if defined(_PLATFORM_WINDOWS_)
	switch (msg)
	{
	case WM_PAINT:
	{
		auto hwndId = Renderer::GetInstance().GetWindowHandleId(window);
		auto rt = Renderer::GetInstance().GetRenderTarget(hwndId);
		if (rt)
		{
			rt->TriggerDrawEvent();
		}
	}
	break;


	case WM_INPUT:
	{
		UINT dwSize = 40;
		static BYTE lpb[40];

		GetRawInputData((HRAWINPUT)lp, RID_INPUT,
			lpb, &dwSize, sizeof(RAWINPUTHEADER));

		RAWINPUT* raw = (RAWINPUT*)lpb;
		if (InputManager::HasInstance()){
			switch (raw->header.dwType)
			{
			case RIM_TYPEMOUSE:
			{
				InputManager::GetInstance().PushMouseEvent(window, *((MouseEvent*)&raw->data.mouse), gpTimer->GetTime());
			}
			return 0;
			case RIM_TYPEKEYBOARD:
			{
				InputManager::GetInstance().PushKeyEvent(window, *((KeyboardEvent*)&raw->data.keyboard));
			}
			return 0;
			}
		}
	}
	break;

	case WM_CHAR:
	{
		if (InputManager::HasInstance())
		{
			InputManager::GetInstance().PushChar(window, wp, gpTimer->GetTime());
		}
	}
	return 0; // processed

	case WM_SETFOCUS:
	{
		if (InputManager::HasInstance())
		{
			InputManager::GetInstance().OnSetFocus(window);
		}		
	}
	return 0;

	case WM_KILLFOCUS:
	{
		if (InputManager::HasInstance()){
			InputManager::GetInstance().OnKillFocus();
		}		
	}
	return 0;

	case WM_SIZE:
	{
		RECT rc;
		GetClientRect((HWND)window, &rc);
		auto width = rc.right - rc.left;
		auto height = rc.bottom - rc.top;
		auto hwndId = Renderer::GetInstance().GetWindowHandleId(window);
		if (hwndId != INVALID_HWND_ID && Renderer::HasInstance()){
			Renderer::GetInstance().OnWindowSizeChanged(window, Vec2I(width, height));
		}
		return 0;
	}

	case WM_SIZING:
	{
		if (Renderer::HasInstance()){
			RECT* pRect = (RECT*)lp;
			RECT prevRect = *pRect;
			auto hwndId = Renderer::GetInstance().GetWindowHandleId(window);
			Vec2I size = Renderer::GetInstance().FindClosestSize(hwndId, Vec2I(prevRect.right - prevRect.left, prevRect.bottom - prevRect.top));
			pRect->right = pRect->left + size.x;
			pRect->bottom = pRect->top + size.y;
			AdjustWindowRect(pRect, GetWindowLongPtr((HWND)window, GWL_STYLE), FALSE);
			RECT adjustedRect = *pRect;
			unsigned eWidth = adjustedRect.right - adjustedRect.left;
			unsigned eHeight = adjustedRect.bottom - adjustedRect.top;

			if (wp == WMSZ_RIGHT || wp == WMSZ_BOTTOMRIGHT || wp == WMSZ_TOPRIGHT || wp == WMSZ_TOP || wp == WMSZ_BOTTOM){
				pRect->left = prevRect.left;
				pRect->right = prevRect.left + eWidth;
			}
			else{
				pRect->right = prevRect.right;
				pRect->left = prevRect.right - eWidth;
			}


			if (wp == WMSZ_BOTTOM || wp == WMSZ_BOTTOMLEFT || wp == WMSZ_BOTTOMRIGHT || wp == WMSZ_RIGHT || wp == WMSZ_LEFT){
				pRect->top = prevRect.top;
				pRect->bottom = prevRect.top + eHeight;
			}
			else{
				pRect->bottom = prevRect.bottom;
				pRect->top = pRect->bottom - eHeight;
			}


			return 1;
		}
		break;
	}

	/*	case WM_ACTIVATE:
	{
	if (WA_ACTIVE==LOWORD(wp))
	{
	BringWindowToTop(mWindowHandles);
	}
	}
	return 0; // processed
	*/
	}


	return DefWindowProc((HWND)window, msg, wp, lp); // not processed
#endif
}

void EngineFacade::RegisterInputConsumer(IInputConsumerPtr consumer, int priority){
	InputManager::GetInstance().RegisterInputConsumer(consumer, priority);
}

void EngineFacade::AddRendererObserver(int rendererObserverType, IRendererObserverPtr observer){
	Renderer::GetInstance().AddObserver(rendererObserverType, observer);
}

void EngineFacade::AddFileChangeObserver(int fileChangeObserverType, IFileChangeObserverPtr observer){
	FileMonitor::GetInstance().AddObserver(fileChangeObserverType, observer);
}

const Vec2I& EngineFacade::GetMainRenderTargetSize() const{
	return Renderer::GetInstance().GetMainRenderTargetSize();
}

IVideoPlayerPtr EngineFacade::CreateVideoPlayer(VideoPlayerType::Enum type){
	switch (type){
	case VideoPlayerType::OGG:
		return VideoPlayerOgg::Create();
	}
	Logger::Log(FB_ERROR_LOG_ARG, "Not implemented type.");
	return 0;
}

void EngineFacade::QueueDrawTextForDuration(float secs, const Vec2I& pos, const char* text, const Color& color){
	QueueDrawTextForDuration(secs, pos, text, color, 20.f);
}

void EngineFacade::QueueDrawTextForDuration(float secs, const Vec2I& pos, const char* text, const Color& color, float size){
	if (Renderer::HasInstance())
		Renderer::GetInstance().QueueDrawTextForDuration(secs, pos, text, color, size);
}

FontPtr EngineFacade::GetFont(float fontHeight){
	if (Renderer::HasInstance())
		Renderer::GetInstance().GetFont(fontHeight);
	Logger::Log(FB_ERROR_LOG_ARG, "Renderer is deleted.");
	return 0;
}