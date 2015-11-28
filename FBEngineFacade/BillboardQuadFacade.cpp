#include "stdafx.h"
#include "BillboardQuadFacade.h"
#include "EngineFacade.h"
#include "FBSceneObjectFactory/BillboardQuad.h"
#include "FBSceneManager/Scene.h"
using namespace fastbird;
class BillboardQuadFacade::Impl{
public:
	BillboardQuadPtr mBillboardQuad;

};

//---------------------------------------------------------------------------
std::vector<BillboardQuadFacadeWeakPtr> sBillboards;
BillboardQuadFacadePtr BillboardQuadFacade::Create(){
	BillboardQuadFacadePtr p(new BillboardQuadFacade, [](BillboardQuadFacade* obj){delete obj; });	
	sBillboards.push_back(p);
	return p;
}

BillboardQuadFacade::BillboardQuadFacade()
	: mImpl(new Impl)
{

}

BillboardQuadFacade::~BillboardQuadFacade(){
	for (auto it = sBillboards.begin(); it != sBillboards.end(); ++it){
		IteratingWeakContainer(sBillboards, it, billboard);
	}
}

void BillboardQuadFacade::AttachToCurrentScene(){
	auto scene = EngineFacade::GetInstance().GetCurrentScene();
	if (scene)
		scene->AttachObjectFB(mImpl->mBillboardQuad);
}

void BillboardQuadFacade::DetachFromScenes(){
	mImpl->mBillboardQuad->DetachFromScene();
}

void BillboardQuadFacade::SetAlwaysPassCullingTest(bool passAlways){
	mImpl->mBillboardQuad->GetBoundingVolumeWorld()->SetAlwaysPass(passAlways);
}

void BillboardQuadFacade::SetBillobardData(const Vec3& pos, const Vec2& size, const Vec2& offset, const Color& color){
	mImpl->mBillboardQuad->SetBillobardData(pos, size, offset, color);
}

void BillboardQuadFacade::SetMaterial(MaterialPtr mat){
	mImpl->mBillboardQuad->SetMaterial(mat);
}

MaterialPtr BillboardQuadFacade::GetMaterial() const{
	return mImpl->mBillboardQuad->GetMaterial();
}