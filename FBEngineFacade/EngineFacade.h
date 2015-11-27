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

#pragma once
#include "FBCommonHeaders/platform.h"
#include "FBCommonHeaders/Types.h"
#include "FBSceneManager/DirectionalLightIndex.h"
#include "RenderTargetParamEx.h"
#include "FBVideoPlayer/VideoPlayerType.h"
namespace fastbird{
	class ProfilerSimple;
	FB_DECLARE_SMART_PTR(Font);
	FB_DECLARE_SMART_PTR(IVideoPlayer);
	FB_DECLARE_SMART_PTR(Camera);
	FB_DECLARE_SMART_PTR(IFileChangeObserver);
	FB_DECLARE_SMART_PTR(IRendererObserver);
	FB_DECLARE_SMART_PTR(IInputInjector);
	FB_DECLARE_SMART_PTR(EngineOptions);
	FB_DECLARE_SMART_PTR(Scene);
	FB_DECLARE_SMART_PTR(EngineFacade);
	FB_DECLARE_SMART_PTR(RenderTarget);
	class FB_DLL_ENGINEFACADE EngineFacade{
		FB_DECLARE_PIMPL_NON_COPYABLE(EngineFacade);
		EngineFacade();
		~EngineFacade();

	public:		
		static EngineFacadePtr Create();
		static EngineFacade& GetInstance();
		static bool HasInstance();

		static const HWindowId INVALID_HWND_ID = (HWindowId)-1;
		
		HWindowId CreateEngineWindow(int x, int y, int width, int height,
			const char* wndClass, const char* title, unsigned style, unsigned exStyle,
			WNDPROC winProc);
		/// \param pluginName "FBRendererD3D11" is provided.
		bool InitRenderer(const char* pluginName);
		bool InitCanvas(HWindowId id, int width, int height);
		/// returning scene attache to the main render target.
		ScenePtr GetMainScene() const;
		void UpdateInput();
		void Update(TIME_PRECISION dt);
		void Render();
		EngineOptionsPtr GetEngineOptions() const;
		bool MainCameraExists() const;
		CameraPtr GetMainCamera() const;
		const Vec3& GetMainCameraPos() const;
		const Vec3& GetMainCameraDirection() const;
		const Ray3& GetWorldRayFromCursor();
		IInputInjectorPtr GetInputInjector();
		void AddDirectionalLightCoordinates(DirectionalLightIndex::Enum idx, Real phi, Real theta);
		void DrawProfileResult(const ProfilerSimple& profiler, const char* posVarName);
		void DrawProfileResult(const ProfilerSimple& profiler, const char* posVarName, int tab);
		/// for windows;
		intptr_t WinProc(HWindow window, unsigned msg, uintptr_t wp, uintptr_t lp);
		/** Register an input consumer.
		You need unregister when the consumer is destroyed or does not
		need to getinput information any more.
		\param consumer
		\param priority number one priority is handled first.
		i.e. the lowest value is handled first. Check the default
		priority at IInputConsumer::Priority
		*/
		void RegisterInputConsumer(IInputConsumerPtr consumer, int priority);
		void AddRendererObserver(int rendererObserverType, IRendererObserverPtr observer);
		void AddFileChangeObserver(int fileChangeObserverType, IFileChangeObserverPtr observer);
		const Vec2I& GetMainRenderTargetSize() const;
		IVideoPlayerPtr CreateVideoPlayer(VideoPlayerType::Enum type);
		void QueueDrawTextForDuration(float secs, const Vec2I& pos, const char* text, const Color& color);
		void QueueDrawTextForDuration(float secs, const Vec2I& pos, const char* text, const Color& color, float size);
		FontPtr GetFont(float fontHeight);
		unsigned GetNumLoadingTexture() const;
		RenderTargetPtr CreateRenderTarget(const RenderTargetParamEx& param);		
		ScenePtr CreateScene(const char* uniquename);
		void OverrideMainScene(ScenePtr scene);
	};
}