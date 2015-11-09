#include "stdafx.h"
#include "Renderer.h"
#include "IRenderer.h"
#include "FBCommonHeaders/Factory.h"
#include "FBSystemLib/ModuleHandler.h"
using namespace fastbird;

class Renderer::RendererImpl{
	typedef fastbird::Factory<IRenderer>::CreateCallback CreateCallback;
	fastbird::Factory<IRenderer> mRendererFactory;
	IRendererPtr mPlatformRender;
	typedef TString ModuleName;	
	typedef std::map<ModuleName, ModuleHandle> LoadedModules;
	LoadedModules mLoadedModules;
	
public:
	RendererImpl(){
#if defined(_PLATFORM_WINDOWS_)
		auto moduleName = _T("FBRendererD3D11.dll");
		auto module = ModuleHandler::LoadModule(moduleName);
		if (module){
			mLoadedModules[moduleName] = module;
			typedef fastbird::IRendererPtr (*Create)();
			Create createCallback = (Create)ModuleHandler::GetFunction(module, "Create");
			if (createCallback){
				mRendererFactory.RegisterProduct("D3D11", createCallback);
				Logger::Log(_T("Renderer %s is available."), _T("D3D11"));
			}
		}		
#else
#endif
	}
	Renderer* CreateRenderer(const char* type){
		mPlatformRender = IRendererPtr(mRendererFactory.CreateProduct(type));
	}
	bool RegisterRenderer(const char* type, CreateCallback callback);
	void UnregisterRenderer(const char* type);
};
#if defined(_PLATFORM_WINDOWS_)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
Renderer::Renderer()
	: mImpl(FB_NEW(RendererImpl))
{

}

Renderer::~Renderer(){
	FB_SAFE_DELETE(mImpl);
}