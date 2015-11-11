#include "stdafx.h"
#include "Renderer.h"
#include "IRenderer.h"
#include "RendererEnums.h"
#include "RendererStructs.h"
#include "Texture.h"
#include "RenderResourceFactory.h"

#include "FBCommonHeaders/Factory.h"
#include "FBSystemLib/ModuleHandler.h"
using namespace fastbird;

class Renderer::RendererImpl{
public:
	typedef fastbird::Factory<IRenderer>::CreateCallback CreateCallback;
	std::string mRenderEngineType;
	IRendererPtr mRenderEngine;
	ModuleHandle mLoadedModule;
	RenderTarget* mCurrentRenderTarget;

	typedef std::vector<RenderTargetWeakPtr> RenderTargets;
	RenderTargets mRenderTargets;
	typedef std::vector<TextureWeakPtr> Textures;
	Textures mTextures;
	
	//-----------------------------------------------------------------------
	RendererImpl():
		mLoadedModule(0){
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
	bool RegisterRenderer(const char* type, CreateCallback callback);
	void UnregisterRenderer(const char* type);

	//-------------------------------------------------------------------
	// Resource Creation
	//-------------------------------------------------------------------
	RenderTargetPtr CreateRenderTarget(RendererPtr renderer){
		auto rt = RenderResourceFactory::CreateResource<RenderTarget>(renderer);
		mRenderTargets.push_back(rt);
		return rt;
	}

	TexturePtr CreateTexture(RendererPtr renderer, const char* file, bool async){
		auto texture = RenderResourceFactory::CreateResource<Texture>(renderer);
		texture->LoadFile(file);
		mTextures.push_back(texture);
		return texture;
	}

	TexturePtr CreateTexture(RendererPtr renderer, const char* file, Texture* reloading, bool async){
		auto texture = RenderResourceFactory::CreateResource<Texture>(renderer);
		mTextures.push_back(texture);
		return texture;
	}

	//-------------------------------------------------------------------
	// Resource Manipulation
	//-------------------------------------------------------------------
	void ReloadTexture(RendererPtr renderer, const char* texturePath){		
		std::string lower = texturePath;
		ToLowerCase(lower);
		for (auto it = mTextures.begin(); it != mTextures.end(); /**/){
			auto texture = it->lock();
			if (!texture){
				it = mTextures.erase(it);
				continue;
			}
			++it;

			if (!texture->GetAdamTexture())
			{
				if (texture->GetName() == lower)
				{
					renderer->CreateTexture(lower.c_str(), p);
				}
			}
		}		
	}
};

//---------------------------------------------------------------------------
Renderer::Renderer()
	: mImpl(new RendererImpl)
{

}

Renderer::~Renderer(){
	delete mImpl;
}

static RendererWeakPtr sRenderer;

RendererPtr Renderer::CreateRenderer(){
	if (sRenderer.lock()){
		Logger::Log(FB_DEFAULT_LOG_ARG, "(error) You can create only one renderer!");
		return;
	}
	auto renderer = RendererPtr(FB_NEW(Renderer), [](Renderer* obj){ delete obj; });
	renderer->mMe = renderer;
	sRenderer = renderer;
	return renderer;
}

RendererPtr Renderer::CreateRenderer(const char* renderEngineName){
	if (sRenderer.lock()){
		Logger::Log(FB_DEFAULT_LOG_ARG, "(error) You can create only one renderer!");
		return;
	}
	auto renderer = RendererPtr(FB_NEW(Renderer), [](Renderer* obj){ delete obj; });
	renderer->mMe = renderer;
	renderer->PrepareRenderEngine(renderEngineName);
	sRenderer = renderer;
	return renderer;
}

RendererPtr Renderer::GetRenderer(){
	return sRenderer.lock();
}

void Renderer::PrepareRenderEngine(const char* renderEngineName){
	mImpl->PrepareRenderEngine(renderEngineName);
}

RenderTargetPtr Renderer::CreateRenderTarget(){
	return mImpl->CreateRenderTarget(mMe.lock());
}

TexturePtr Renderer::CreateTexture(const char* file, bool async){
	return RenderResourceFactory::CreateResource<Texture>(mMe.lock(), file, async);
}

TexturePtr Renderer::CreateTexture(const char* file, Texture* reloading, bool async){
	return RenderResourceFactory::CreateResource<Texture>(mMe.lock(), file, reloading, async);
}

//-------------------------------------------------------------------
// Resource Management
//-------------------------------------------------------------------
void Renderer::ReloadTexture(const char* texturePath){
	mImpl->ReloadTexture(this, texturePath);
	std::string lower = texturePath;
	ToLowerCase(lower);
	for (auto texture : mTextures)
	{
		Texture* p = *it;
		if (!p->GetAdamTexture())
		{
			if (p->GetName() == lower)
			{
				gFBEnv->pRenderer->CreateTexture(lower.c_str(), p);
			}
		}
	}
}

//-------------------------------------------------------------------
void Renderer::SetCurrentRenderTarget(RenderTarget* renderTarget){
	mImpl->mCurrentRenderTarget = renderTarget;
}

HWindow Renderer::GetWindowHandle(RenderTargetId rtId){

}