#include "stdafx.h"
#include "FacadeTrail.h"
#include "EngineFacade.h"
#include "FBRenderer/Material.h"
#include "FBSceneManager/Scene.h"
#include "FBSceneObjectFactory/TrailObject.h"
using namespace fastbird;
class FacadeTrail::Impl{
public:
	TrailObjectPtr mTrail;
	Transformation mOffset;

	//---------------------------------------------------------------------------
	Impl()
	{
		mTrail = TrailObject::Create();
	}

	void SetOffset(const Transformation& offset){
		mOffset = offset;
	}

	void AddPoint(const Transformation& parentLocation){
		mTrail->AddPoint((parentLocation * mOffset).GetTranslation());
	}

	void SetWidth(float width){
		mTrail->SetWidth(width);
	}

	bool AttachToScene(){
		auto scene = EngineFacade::GetInstance().GetMainScene();
		if (!scene)
			return false;
		return scene->AttachObjectFB(mTrail);
	}

	bool DetachFromScene(){
		auto scene = EngineFacade::GetInstance().GetMainScene();
		if (!scene)
			return false;
		return scene->DetachObjectFB(mTrail);
	}

	MaterialPtr GetMaterial(){
		return mTrail->GetMaterial();
	}

};

//---------------------------------------------------------------------------
std::vector<FacadeTrailWeakPtr> sTrails;
//---------------------------------------------------------------------------
FacadeTrailPtr FacadeTrail::Create(){
	FacadeTrailPtr p(new FacadeTrail, [](FacadeTrail* obj){ delete obj; });
	sTrails.push_back(p);
	return p;
}

FacadeTrail::FacadeTrail()
	:mImpl(new Impl)
{

}

FacadeTrail::~FacadeTrail(){

}

void FacadeTrail::SetOffset(const Transformation& offset){
	mImpl->SetOffset(offset);
}

void FacadeTrail::AddPoint(const Transformation& parentLocation){
	mImpl->AddPoint(parentLocation);
}

void FacadeTrail::SetWidth(float width){
	mImpl->SetWidth(width);
}

bool FacadeTrail::AttachToScene(){
	return mImpl->AttachToScene();
}

bool FacadeTrail::DetachFromScene(){
	return mImpl->DetachFromScene();
}

MaterialPtr FacadeTrail::GetMaterial(){
	return mImpl->GetMaterial();
}

void FacadeTrail::SetDiffuseColor(const Color& color){
	auto mat = GetMaterial();
	if (mat){
		mat->SetDiffuseColor(color.GetVec4());
	}
}

void FacadeTrail::Update(TIME_PRECISION dt){
	mImpl->mTrail->Update(dt);
}