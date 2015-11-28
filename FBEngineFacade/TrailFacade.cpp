#include "stdafx.h"
#include "TrailFacade.h"
#include "EngineFacade.h"
#include "FBRenderer/Material.h"
#include "FBSceneManager/Scene.h"
#include "FBSceneObjectFactory/TrailObject.h"
using namespace fastbird;
class TrailFacade::Impl{
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
		return mTrail->DetachFromScene();
	}

	MaterialPtr GetMaterial(){
		return mTrail->GetMaterial();
	}

};

//---------------------------------------------------------------------------
std::vector<TrailFacadeWeakPtr> sTrails;
//---------------------------------------------------------------------------
TrailFacadePtr TrailFacade::Create(){
	TrailFacadePtr p(new TrailFacade, [](TrailFacade* obj){ delete obj; });
	sTrails.push_back(p);
	return p;
}

TrailFacade::TrailFacade()
	:mImpl(new Impl)
{

}

TrailFacade::~TrailFacade(){

}

void TrailFacade::SetOffset(const Transformation& offset){
	mImpl->SetOffset(offset);
}

void TrailFacade::AddPoint(const Transformation& parentLocation){
	mImpl->AddPoint(parentLocation);
}

void TrailFacade::SetWidth(float width){
	mImpl->SetWidth(width);
}

bool TrailFacade::AttachToScene(){
	return mImpl->AttachToScene();
}

bool TrailFacade::DetachFromScene(){
	return mImpl->DetachFromScene();
}

MaterialPtr TrailFacade::GetMaterial(){
	return mImpl->GetMaterial();
}

void TrailFacade::SetDiffuseColor(const Color& color){
	auto mat = GetMaterial();
	if (mat){
		mat->SetDiffuseColor(color.GetVec4());
	}
}

void TrailFacade::Update(TIME_PRECISION dt){
	mImpl->mTrail->Update(dt);
}