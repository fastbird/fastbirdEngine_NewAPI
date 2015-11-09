#include "stdafx.h"
#include "Renderer.h"
#include "IRenderer.h"
#include "FBFactory.h"
using namespace fastbird;

class Renderer::RendererImpl{
	typedef fastbird::Factory<IRenderer>::CreateCallback CreateCallback;
	fastbird::Factory<IRenderer> mRendererFactory;
	
public:
	Renderer* CreateRenderer(const char* type){
		mRendererFactory.CreateProduct(type);
	}
	bool RegisterRenderer(const char* type, CreateCallback callback);
	void UnregisterRenderer(const char* type);
};

Renderer::Renderer()
	: mImpl(FB_NEW(RendererImpl))
{
	mImpl->RegisterRenderer("D3D11", )
}

Renderer::~Renderer(){
	FB_SAFE_DELETE(mImpl);
}