#include "stdafx.h"
#include "ParticleEmitterFacade.h"
#include "FBParticleSystem/ParticleEmitter.h"
#include "FBParticleSystem/ParticleSystem.h"
using namespace fastbird;
class ParticleEmitterFacade::Impl{
public:
	ParticleEmitterPtr mParticleEmitter;

};

//---------------------------------------------------------------------------
std::vector<ParticleEmitterFacadeWeakPtr> sEmitters;

ParticleEmitterFacadePtr ParticleEmitterFacade::Create(){
	ParticleEmitterFacadePtr p(new ParticleEmitterFacade, [](ParticleEmitterFacade* obj){ delete obj; });
	sEmitters.push_back(p);
	return p;
}

ParticleEmitterFacade::ParticleEmitterFacade()
	: mImpl(new Impl)
{

}

ParticleEmitterFacade::~ParticleEmitterFacade(){
	for (auto it = sEmitters.begin(); it != sEmitters.end(); /**/)
	{
		IteratingWeakContainer(sEmitters, it, emitter);
	}
}

bool ParticleEmitterFacade::Load(unsigned id){
	mImpl->mParticleEmitter =  ParticleSystem::GetInstance().GetParticleEmitter(id);
	return mImpl->mParticleEmitter != 0;
}

bool ParticleEmitterFacade::IsValid() const{
	return mImpl->mParticleEmitter != 0;
}

void ParticleEmitterFacade::SetTransformation(const Transformation& t){
	if (mImpl->mParticleEmitter){
		mImpl->mParticleEmitter->SetLocation(t);
	}
}

const Transformation& ParticleEmitterFacade::GetTransformation() const{
	if (mImpl->mParticleEmitter){
		return mImpl->mParticleEmitter->GetLocation();
	}
	return Transformation::IDENTITY;
}

void ParticleEmitterFacade::SetPosition(const Vec3& pos){
	if (mImpl->mParticleEmitter){
		return mImpl->mParticleEmitter->SetPosition(pos);
	}
}

void ParticleEmitterFacade::Active(bool active, bool pending){
	if (mImpl->mParticleEmitter){
		mImpl->mParticleEmitter->Active(active, pending);
	}
}

bool ParticleEmitterFacade::IsActive() const{
	if (mImpl->mParticleEmitter){
		return mImpl->mParticleEmitter->IsActive();
	}
	return false;
}

void ParticleEmitterFacade::StopImmediate(){
	if (mImpl->mParticleEmitter){
		mImpl->mParticleEmitter->StopImmediate();
	}
}

bool ParticleEmitterFacade::GetVisible(){
	if (mImpl->mParticleEmitter){
		return mImpl->mParticleEmitter->GetVisible();
	}
	return false;
}

void ParticleEmitterFacade::SetVisible(bool visible){
	if (mImpl->mParticleEmitter){
		return mImpl->mParticleEmitter->SetVisible(visible);
	}
}

float ParticleEmitterFacade::GetAlpha() const{
	if (mImpl->mParticleEmitter){
		return mImpl->mParticleEmitter->GetAlpha();
	}
	return 0.f;
}

void ParticleEmitterFacade::SetAlpha(float alpha){
	if (mImpl->mParticleEmitter){
		return mImpl->mParticleEmitter->SetAlpha(alpha);
	}
}

void ParticleEmitterFacade::SetScale(const Vec3& scale){
	if (mImpl->mParticleEmitter){
		return mImpl->mParticleEmitter->SetScale(scale);
	}
}

void ParticleEmitterFacade::SetLength(float length){
	if (mImpl->mParticleEmitter){
		return mImpl->mParticleEmitter->SetLength(length);
	}
}

bool ParticleEmitterFacade::IsAlive() const{
	if (mImpl->mParticleEmitter){
		return mImpl->mParticleEmitter->IsAlive();
	}
	return false;
}

void ParticleEmitterFacade::Stop(){
	if (mImpl->mParticleEmitter){
		return mImpl->mParticleEmitter->Stop();
	}
}

void ParticleEmitterFacade::RemoveShaderDefine(const char* def){
	if (mImpl->mParticleEmitter){
		return mImpl->mParticleEmitter->RemoveShaderDefine(def);
	}
}

void ParticleEmitterFacade::AddShaderDefine(const char* def, const char* val){
	if (mImpl->mParticleEmitter){
		return mImpl->mParticleEmitter->AddShaderDefine(def, val);
	}
}

//void ParticleEmitterFacade::ApplyShaderDefine(){
//	if (mImpl->mParticleEmitter){
//		return mImpl->mParticleEmitter->ApplyShaderDefine();
//	}
//}

void ParticleEmitterFacade::SetRelativeVelocity(const Vec3& dir, float speed){
	if (mImpl->mParticleEmitter){
		return mImpl->mParticleEmitter->SetRelativeVelocity(dir, speed);
	}
}