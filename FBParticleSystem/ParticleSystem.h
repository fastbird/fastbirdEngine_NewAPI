#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird
{
	DECLARE_SMART_PTR(ParticleEmitter);
	DECLARE_SMART_PTR(ParticleSystem);
	class FB_DLL_PARTICLESYSTEM ParticleSystem
	{
		DECLARE_PIMPL_NON_COPYABLE(ParticleSystem);
		ParticleSystem();
		~ParticleSystem();

	public:
		static ParticleSystemPtr Create();
		static ParticleSystem& GetInstance();

		void Update(float elapsedTime);
		void RenderProfile();
		ParticleEmitterPtr GetParticleEmitter(const char* file);
		ParticleEmitterPtr GetParticleEmitter(unsigned id);
		void ReloadParticle(const char* file);
		void EditThisParticle(const char* path);
		void ScaleEditingParticle(float scale);
		unsigned GetNumActiveParticles() const;
		void StopParticles();

	private:
		friend class ParticleEmitter;
		void AddActiveParticle(ParticleEmitterPtr pEmitter);
		void RemoveDeactiveParticle(ParticleEmitterPtr pEmitter);
		void AddActiveParticlePending(ParticleEmitterPtr pEmitter);
		
	};
}