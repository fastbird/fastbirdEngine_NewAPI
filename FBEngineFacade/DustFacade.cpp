#include "stdafx.h"
#include "DustFacade.h"
#include "FBSceneObjectFactory/SceneObjectFactory.h"
#include "FBSceneObjectFactory/DustRenderer.h"
#include "FBEngineFacade/EngineFacade.h"
#include "FBSceneManager/Scene.h"
using namespace fastbird;
class DustFacade::Impl{
public:
	DustRendererPtr mDustRenderer;

	//---------------------------------------------------------------------------
	Impl()
		:mDustRenderer(SceneObjectFactory::GetInstance().CreateDustRenderer())
	{
		
	}


};

std::vector<DustFacadeWeakPtr> sDustFacades;
DustFacadePtr DustFacade::Create(){
	DustFacadePtr p(new DustFacade, [](DustFacade* obj){delete obj; });
	sDustFacades.push_back(p);
	return p;
}

DustFacade::DustFacade()
	: mImpl(new Impl)
{

}
DustFacade::~DustFacade(){
	for (auto it = sDustFacades.begin(); it != sDustFacades.end(); /**/){
		IteratingWeakContainer(sDustFacades, it, dust);
	}
}

void DustFacade::SetMaterial(const char* path){
	mImpl->mDustRenderer->SetMaterial(path, PASS_NORMAL);
}

void DustFacade::InitDustRenderer(const Vec3& min, const Vec3& max, size_t count,
	const Color& cmin, const Color& cmax, float normalizeDist){
	mImpl->mDustRenderer->Initialize(min, max, count, cmin, cmax, normalizeDist);
}

void DustFacade::AttachToScene(){
	auto scene = EngineFacade::GetInstance().GetMainScene();
	if (scene)
		scene->AttachObjectFB(mImpl->mDustRenderer);
}

void DustFacade::DetachFromScene(){
	mImpl->mDustRenderer->DetachFromScene();
}

const Vec3& DustFacade::GetMin() const{
	return mImpl->mDustRenderer->GetMin();
}