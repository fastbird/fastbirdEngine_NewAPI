#include "stdafx.h"
#include "SkyFacade.h"
#include "FBSceneObjectFactory/SceneObjectFactory.h"
#include "FBSceneObjectFactory/SkySphere.h"
#include "FBEngineFacade/EngineFacade.h"
#include "FBSceneManager/Scene.h"
using namespace fastbird;
class SkyFacade::Impl{
public:
	SkySpherePtr mSkySphere;

	Impl(){
		mSkySphere = SceneObjectFactory::GetInstance().CreateSkySphere();
	}


};

//---------------------------------------------------------------------------
std::vector<SkyFacadeWeakPtr> sSkySpheres;
SkyFacadePtr SkyFacade::Create(){
	SkyFacadePtr p(new SkyFacade, [](SkyFacade* obj){ delete obj; });
	sSkySpheres.push_back(p);
	return p;
}

SkyFacadePtr SkyFacade::GetMain(){
	auto scene = EngineFacade::GetInstance().GetMainScene();
	if (!scene)
		return 0;
	auto mainSky = scene->GetSkySphere();
	if (!mainSky)
		return 0;
	for (auto it = sSkySpheres.begin(); it != sSkySpheres.end(); /**/){
		IteratingWeakContainer(sSkySpheres, it, sky);
		if (sky->mImpl->mSkySphere == mainSky)
			return sky;
	}
	return 0;
}

SkyFacade::SkyFacade()
	: mImpl(new Impl)
{

}

SkyFacade::~SkyFacade(){

}

void SkyFacade::SetMaterial(const char* path, RENDER_PASS pass){
	mImpl->mSkySphere->SetMaterial(path, pass);
}

void SkyFacade::AttachToScene(){
	EngineFacade::GetInstance().GetMainScene()->AttachObjectFB(mImpl->mSkySphere);
}

void SkyFacade::AttachToScene(IScenePtr scene){
	if (scene)
		scene->AttachObjectFB(mImpl->mSkySphere);
}

void SkyFacade::DetachFromScene(){
	mImpl->mSkySphere->DetachFromScene();
}

MaterialPtr SkyFacade::GetMaterial() const{
	return mImpl->mSkySphere->GetMaterial(0);
}

void SkyFacade::UpdateEnvironmentMap(const Vec3& pos){
	mImpl->mSkySphere->UpdateEnvironmentMap(pos);
}

void SkyFacade::AttachToBlend(){
	EngineFacade::GetInstance().GetMainScene()->GetSkySphere()->AttachBlendingSky(mImpl->mSkySphere);
}

void SkyFacade::SetAlpha(float alpha){
	mImpl->mSkySphere->SetAlpha(alpha);
}

void SkyFacade::PrepareInterpolation(float time, SkyFacadePtr startFrom){
	mImpl->mSkySphere->PrepareInterpolation(time, startFrom->mImpl->mSkySphere);
}

void SkyFacade::AttachBlendingSky(SkyFacadePtr blending){
	mImpl->mSkySphere->AttachBlendingSky(blending->mImpl->mSkySphere);
}

void SkyFacade::SetInterpolationData(unsigned index, const Vec4& data){
	mImpl->mSkySphere->SetInterpolationData(index, data);
}