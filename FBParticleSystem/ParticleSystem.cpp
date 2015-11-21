#include "stdafx.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "ParticleRenderObject.h"
#include "FBRenderer/Renderer.h"
#include "FBRenderer/RenderTarget.h"
#include "FBSceneManager/Camera.h"
using namespace fastbird;

Timer* fastbird::gpTimer = Timer::GetMainTimer().get();
class ParticleSystem::Impl
{
public:
	ParticleSystem* mSelf;
	typedef std::vector< ParticleEmitterPtr > Emitters;
	Emitters mActiveParticles;
	Emitters mPendingAdds;
	Emitters mPendingDeletes;

	// cache
	typedef std::map<unsigned, ParticleEmitterPtr > PARTICLE_EMITTERS_BY_ID;
	PARTICLE_EMITTERS_BY_ID mParticleEmitters;
	typedef std::map<std::string, ParticleEmitterPtr > PARTICLE_EMITTERS_BY_NAME;
	PARTICLE_EMITTERS_BY_NAME mParticleEmittersByName;

	ParticleEmitterPtr mEditingParticle;

	//---------------------------------------------------------------------------
	Impl(ParticleSystem* self)
		: mSelf(self)
	{
	}

	void Update(float elapsedTime){
		ParticleRenderObject::ClearParticles();
		auto& renderer = Renderer::GetInstance();
		for (auto p : mPendingDeletes)
		{
			mActiveParticles.erase(
				std::remove(mActiveParticles.begin(), mActiveParticles.end(), p),
				mActiveParticles.end());
		}
		mPendingDeletes.clear();

		for (auto p : mPendingAdds)
		{
			AddActiveParticle(p);
		}
		mPendingAdds.clear();

		if (mEditingParticle)
		{
			if (!mEditingParticle->IsAlive())
			{
				mEditingParticle->Active(true);
			}
			if (0) // Move Edit Particle
			{
				float time = gpTimer->GetTime();
				Vec3 movedir = Vec3(cos(time), 0.f, 0.f);
				Vec3 oldPos = mEditingParticle->GetPosition();
				Vec3 pos = oldPos + 1.f * movedir;
				mEditingParticle->SetPosition(pos);
				Vec3 dir = pos - oldPos;				
				auto pcam = renderer.GetMainCamera();
				Vec3 campos = pcam->GetPosition();
				pcam->SetPosition(campos + dir);
				float len = dir.Normalize();
				if (len > 0.f)
				{
					mEditingParticle->SetDirection(dir);
				}
			}


		}
		auto cam = renderer.GetMainCamera();
		Vec3 camDir = cam->GetDirection();
		Vec3 camPos = cam->GetPosition();
		std::stable_sort(mActiveParticles.begin(), mActiveParticles.end(), [&camDir, &camPos](ParticleEmitterPtr a, ParticleEmitterPtr b)->bool
		{
			float da = (a->GetPosition() - camPos).Dot(camDir);
			float db = (b->GetPosition() - camPos).Dot(camDir);
			return da > db;
		});

		Emitters::iterator it = mActiveParticles.begin();
		for (; it != mActiveParticles.end();)
		{
			bool updated = (*it)->Update(elapsedTime);
			if (!updated)
				it = mActiveParticles.erase(it);
			else
				it++;
		}

		if (0)//r_numParticleEmitters
		{
			wchar_t buf[256];
			swprintf_s(buf, L"num of active particles : %u", mActiveParticles.size());
			renderer.QueueDrawText(Vec2I(100, 226), buf, Color::White);
		}

		ParticleRenderObject::EndUpdateParticles();
	}

	void RenderProfile(){
		wchar_t msg[255];
		int x = 700;
		int y = 20;
		int yStep = 20;
		auto& r = Renderer::GetInstance();
		swprintf_s(msg, 255, L"Emitter Types= %u", mParticleEmitters.size());
		r.QueueDrawText(Vec2I(x, y), msg, Vec3(1, 1, 1));
		y += yStep;

		swprintf_s(msg, 255, L"Total Emitter Instances= %u", mActiveParticles.size());
		r.QueueDrawText(Vec2I(x, y), msg, Vec3(1, 1, 1));
		y += yStep;

		swprintf_s(msg, 255, L"Total Emitter Renderers = %u", ParticleRenderObject::GetNumRenderObject());
		r.QueueDrawText(Vec2I(x, y), msg, Vec3(1, 1, 1));
		y += yStep;

		swprintf_s(msg, 255, L"Number of Draw Calls = %u", ParticleRenderObject::GetNumDrawCalls());
		r.QueueDrawText(Vec2I(x, y), msg, Vec3(1, 1, 1));
		y += yStep;

		swprintf_s(msg, 255, L"Number of Draw Vertices = %u", ParticleRenderObject::GetNumPrimitives());
		r.QueueDrawText(Vec2I(x, y), msg, Vec3(1, 1, 1));
		y += yStep;
	}

	ParticleEmitterPtr GetParticleEmitter(const char* file){
		PARTICLE_EMITTERS_BY_NAME::iterator found = mParticleEmittersByName.find(file);
		if (found != mParticleEmittersByName.end())
		{
			return found->second->Clone();
		}

		ParticleEmitterPtr p = ParticleEmitter::Create();
		bool succ = p->Load(file);
		if (succ)
		{
			mParticleEmitters[p->GetEmitterID()] = p;
			mParticleEmittersByName[file] = p;
			return (IParticleEmitter*)p->Clone();
		}

		return 0;
	}

	ParticleEmitterPtr GetParticleEmitter(unsigned id){		
		PARTICLE_EMITTERS_BY_ID::iterator found = mParticleEmitters.find(id);
		if (found != mParticleEmitters.end())
			return found->second->Clone();

		std::string filepath = FindParticleNameWithID("data/particles", id);
		return GetParticleEmitter(filepath.c_str());		
	}

	void ReloadParticle(const char* file){
		for (auto& p : mParticleEmittersByName)
		{
			if (_stricmp(p.first.c_str(), file) == 0)
			{
				std::string lower = file;
				ToLowerCase(lower);
				bool reload = true;
				p.second->Load(lower.c_str(), reload);				
			}
		}
	}

	void EditThisParticle(const char* file){
		if (mEditingParticle)
			mEditingParticle->StopImmediate();

		bool numeric = IsNumeric(file);
		std::string fullpath;
		if (numeric)
		{
			fullpath = FindParticleNameWithID("data/particles", StringConverter::ParseInt(file));
		}
		else
		{
			fullpath = file;
			fullpath += ".particle";
		}


		ParticleEmitterPtr pnew = 0;
		if (strcmp(file, "0"))
			pnew = GetParticleEmitter(fullpath.c_str());

		auto& renderer = Renderer::GetInstance();
		if (pnew)
		{

			mEditingParticle = pnew;
			Logger::Log(FB_DEFAULT_LOG_ARG, FormatString("Editing : %s", fullpath.c_str()).c_str());			
			auto rt = renderer.GetMainRenderTarget();
			assert(rt);
			auto oldCam = rt->GetCamera();
			mEditingParticle->SetPosition(oldCam->GetPosition() + oldCam->GetDirection() * 4.f);
			mEditingParticle->Active(true);
			oldCam->SetTarget(mEditingParticle);
			oldCam->SetEnalbeInput(true);
		}
		else if (strcmp(file, "0") == 0)
		{
			mEditingParticle->StopImmediate();
			mEditingParticle = 0;
			auto rt = renderer->GetMainRenderTarget();
			assert(rt);
			rt->RemoveOverridingCamera();
			Log("Exit particle editor");
		}
		else
		{
			Log("Cannot find the particle %s", fullpath.c_str());
		}
	}

	void ScaleEditingParticle(float scale){
		
	}

	unsigned GetNumActiveParticles() const{
		
	}

	void StopParticles(){
		
	}

	
	std::string FindParticleNameWithID(const char* particleFolder, unsigned id){

	}
	void AddActiveParticle(ParticleEmitterPtr pEmitter){
		
	}

	void RemoveDeactiveParticle(ParticleEmitterPtr pEmitter){
		
	}

	void AddActiveParticlePending(ParticleEmitterPtr pEmitter){
		
	}

	
};

static ParticleSystem* sParticleSystem;
namespace fastbird
{

ParticleSystem* ParticleSystem::sParticleSystem=0;

//---------------------------------------------------------------------------
void ParticleSystem::InitializeParticleSystem()
{
	sParticleSystem = FB_NEW(ParticleSystem);
}

//---------------------------------------------------------------------------
ParticleSystem& ParticleSystem::GetParticleSystem()
{
	assert(sParticleSystem);
	return *sParticleSystem;
}

//---------------------------------------------------------------------------
void ParticleSystem::FinalizeParticleSystem()
{
	FB_SAFE_DEL(sParticleSystem);
}

//---------------------------------------------------------------------------
std::string ParticleSystem::FindParticleNameWithID(const char* particlefolder, unsigned id)
{
	tinydir_dir dir;
	if (tinydir_open(&dir, particlefolder) == -1)
	{
		Log("Failed to open %s", particlefolder);
		return std::string();
	}

	while (dir.has_next)
	{
		tinydir_file file;
		if (tinydir_readfile(&dir, &file) == -1)
			break;
		StringVector sv = Split(file.name, "_");
		if (sv.size() >= 2)
		{
			unsigned fileid = StringConverter::parseUnsignedInt(sv[0]);
			if (fileid == id)
				return std::string(particlefolder) + "/" + file.name;
		}
		tinydir_next(&dir);
	}
	Log("Cannot find particles which has id %d", id);
	return std::string();
}

//---------------------------------------------------------------------------
ParticleSystem::ParticleSystem()
{
}

//---------------------------------------------------------------------------
ParticleSystem::~ParticleSystem()
{
	FB_FOREACH(it, mActiveParticles)
	{
		(*it)->Active(false);
	}

	mActiveParticles.clear();
	mPendingDeletes.clear();
	mPendingAdds.clear();

	mParticleEmitters.clear();
	mParticleEmittersByName.clear();
	ParticleRenderObject::FinalizeRenderObjects();
}

//---------------------------------------------------------------------------
void ParticleSystem::Update(float elapsedTime)
{
	ParticleRenderObject::ClearParticles();

	for (auto p : mPendingDeletes)
	{
		mActiveParticles.erase(
			std::remove(mActiveParticles.begin(), mActiveParticles.end(), p), 
			mActiveParticles.end());
	}
	mPendingDeletes.clear();

	for (auto p : mPendingAdds)
	{
		AddActiveParticle(p);
	}
	mPendingAdds.clear();

	if (mEditingParticle)
	{
		if (!mEditingParticle->IsAlive())
		{
			mEditingParticle->Active(true);
		}
		if (gFBEnv->pConsole->GetEngineCommand()->MoveEditParticle)
		{
			float time = gFBEnv->pTimer->GetTime();
			Vec3 movedir = Vec3(cos(time), 0.f, 0.f);
			Vec3 oldPos = mEditingParticle->GetPos();
			Vec3 pos = oldPos + 1.f * movedir;
			mEditingParticle->SetPos(pos);
			Vec3 dir = pos - oldPos;
			auto const renderer = gFBEnv->pRenderer;
			auto mainRT = renderer->GetMainRenderTarget();
			assert(mainRT);
			ICamera* pcam = mainRT->GetCamera();
			Vec3 campos = pcam->GetPos();
			pcam->SetPos(campos + dir);
			float len = dir.Normalize();
			if (len > 0.f)
			{
				mEditingParticle->SetDir(dir);
			}
		}
		

	}

	Vec3 camDir = gFBEnv->pRenderer->GetCamera()->GetForward();
	Vec3 camPos = gFBEnv->pRenderer->GetCamera()->GetPos();
	std::stable_sort(mActiveParticles.begin(), mActiveParticles.end(), [&camDir, &camPos](IParticleEmitter* a, IParticleEmitter* b)->bool
	{
		float da = (a->GetPos() - camPos).Dot(camDir);
		float db = (b->GetPos() - camPos).Dot(camDir);
		return da > db;
	});

	Emitters::iterator it = mActiveParticles.begin();
	for (; it!=mActiveParticles.end(); )
	{
		bool updated = (*it)->Update(elapsedTime);
		if (!updated)
			it = mActiveParticles.erase(it);
		else
			it++;
	}

	if (gFBEnv->pConsole->GetEngineCommand()->r_numParticleEmitters)
	{
		wchar_t buf[256];
		swprintf_s(buf, L"num of active particles : %u", mActiveParticles.size());
		gFBEnv->pRenderer->DrawText(Vec2I(100, 226), buf, Color::White);
	}

	ParticleRenderObject::EndUpdateParticles();
}

void ParticleSystem::AddActiveParticle(IParticleEmitter* pEmitter)
{
	LOCK_CRITICAL_SECTION lock(mActivePCS);
	assert(ValueNotExistInVector(mActiveParticles, pEmitter));
	mActiveParticles.push_back(pEmitter);

	DeleteValuesInVector(mPendingDeletes, pEmitter);
}

void ParticleSystem::AddActiveParticlePending(IParticleEmitter* pEmitter)
{
	LOCK_CRITICAL_SECTION lock(mActivePCS);
	assert(ValueNotExistInVector(mPendingAdds, pEmitter));
	mPendingAdds.push_back(pEmitter);

	DeleteValuesInVector(mPendingDeletes, pEmitter);
}

void ParticleSystem::RemoveDeactiveParticle(IParticleEmitter* pEmitter)
{
	LOCK_CRITICAL_SECTION lock(mPendingDCS);
	mPendingDeletes.push_back(pEmitter);
}

//---------------------------------------------------------------------------
IParticleEmitter* ParticleSystem::GetParticleEmitter(const char* file)
{
	LOCK_CRITICAL_SECTION lock(mGetPNameCS);
	PARTICLE_EMITTERS_BY_NAME::iterator found =mParticleEmittersByName.find(file);
	if (found != mParticleEmittersByName.end())
	{
		return (IParticleEmitter*)found->second->Clone();
	}

	SmartPtr<IParticleEmitter> p = IParticleEmitter::CreateParticleEmitter();
	bool succ = p->Load(file);
	if (succ)
	{
		mParticleEmitters[p->GetEmitterID()] = p;
		mParticleEmittersByName[file] = p;
		return (IParticleEmitter*)p->Clone();
	}
		
	return 0;
}

IParticleEmitter* ParticleSystem::GetParticleEmitter(unsigned id)
{
	LOCK_CRITICAL_SECTION lock(mGetPCS);
	PARTICLE_EMITTERS_BY_ID::iterator found = mParticleEmitters.find(id);
	if (found != mParticleEmitters.end())
		return (IParticleEmitter*)found->second->Clone();

	std::string filepath = ParticleSystem::FindParticleNameWithID("data/particles", id);
	IParticleEmitter* pemitter = GetParticleEmitter(filepath.c_str());
	return pemitter;
}

void ParticleSystem::RenderProfile()
{
	wchar_t msg[255];
	int x = 700;
	int y = 20;
	int yStep = 20;	
	IRenderer* r = gFBEnv->pRenderer;
	swprintf_s(msg, 255, L"Emitter Types= %u", mParticleEmitters.size());
	r->DrawText(Vec2I(x, y), msg, Vec3(1, 1, 1));
	y += yStep;

	swprintf_s(msg, 255, L"Total Emitter Instances= %u", mActiveParticles.size());
	r->DrawText(Vec2I(x, y), msg, Vec3(1, 1, 1));
	y += yStep;

	swprintf_s(msg, 255, L"Total Emitter Renderers = %u", ParticleRenderObject::GetNumRenderObject());
	r->DrawText(Vec2I(x, y), msg, Vec3(1, 1, 1));
	y += yStep;

	swprintf_s(msg, 255, L"Number of Draw Calls = %u", ParticleRenderObject::GetNumDrawCalls());
	r->DrawText(Vec2I(x, y), msg, Vec3(1, 1, 1));
	y += yStep;

	swprintf_s(msg, 255, L"Number of Draw Vertices = %u", ParticleRenderObject::GetNumPrimitives());
	r->DrawText(Vec2I(x, y), msg, Vec3(1, 1, 1));
	y += yStep;


}

//---------------------------------------------------------------------------
void ParticleSystem::ReloadParticle(const char* file)
{
	for (auto& p : mParticleEmittersByName)
	{
		if (_stricmp(p.first.c_str(), file)==0)
		{
			std::string lower = file;
			ToLowerCase(lower);
			p.second->Load(lower.c_str());
			ParticleEmitter* editing = (ParticleEmitter*)mEditingParticle.get();
			if (editing && editing->mAdam == p.second)
			{
				auto pos = mEditingParticle->GetPos();
				mEditingParticle->StopImmediate();
				mEditingParticle = (IParticleEmitter*)p.second->Clone();
				mEditingParticle->SetPos(pos);
				auto const renderer = gFBEnv->pRenderer;
				auto mainRT = renderer->GetMainRenderTarget();
				ICamera* pCam = mainRT->GetOrCreateOverridingCamera();
				if (pCam)
					pCam->SetTarget(mEditingParticle);
				mEditingParticle->Active(true);
			}
		}
	}
}

void ParticleSystem::EditThisParticle(const char* file)
{
	if (mEditingParticle)
		mEditingParticle->StopImmediate();

	bool numeric = IsNumeric(file);
	std::string fullpath;
	if (numeric)
	{
		fullpath = FindParticleNameWithID("data/particles", StringConverter::parseInt(file));
	}
	else
	{
		fullpath = file;
		fullpath += ".particle";
	}
	
	
	IParticleEmitter* pnew = 0;
	if (strcmp(file, "0"))
		pnew = GetParticleEmitter(fullpath.c_str());
	 
	auto const renderer = gFBEnv->pRenderer;
	if (pnew)
	{
		
		mEditingParticle = pnew;
		Log("Editing : %s", fullpath.c_str());
		auto rt = renderer->GetMainRenderTarget();
		assert(rt);
		auto oldCam = rt->GetCamera();
		mEditingParticle->SetPos(oldCam->GetPos() + oldCam->GetDir() * 4.f);
		mEditingParticle->Active(true);
		auto newCam = rt->GetOrCreateOverridingCamera();
		newCam->SetPos(oldCam->GetPos());
		newCam->SetDir(oldCam->GetDir());
		newCam->SetTarget(mEditingParticle);
		newCam->SetEnalbeInput(true);
	}
	else if (strcmp(file,"0")==0)
	{
		mEditingParticle->StopImmediate();
		mEditingParticle = 0;
		auto rt = renderer->GetMainRenderTarget();
		assert(rt);
		rt->RemoveOverridingCamera();
		Log("Exit particle editor");
	}
	else
	{
		Log("Cannot find the particle %s", fullpath.c_str());
	}
}

void ParticleSystem::ScaleEditingParticle(float scale)
{
	if (mEditingParticle)
	{
		mEditingParticle->SetScale(Vec3(scale));
	}
}


unsigned ParticleSystem::GetNumActiveParticles() const
{
	return mActiveParticles.size();
}

void ParticleSystem::StopParticles(){
	Emitters::iterator it = mActiveParticles.begin();
	for (; it != mActiveParticles.end(); it++)
	{
		(*it)->StopImmediate();			
	}
}

}
