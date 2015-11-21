#pragma once
#include "FBCommonHeaders/Types.h"
#include "FBCommonHeaders/CircularBuffer.h"
namespace fastbird
{
	DECLARE_SMART_PTR(ParticleEmitter);
	class FB_DLL_PARTICLESYSTEM ParticleEmitter
	{
		DECLARE_PIMPL_CLONEABLE(ParticleEmitter);
		ParticleEmitter();
		~ParticleEmitter();
		static ParticleEmitterPtr Create(const ParticleEmitter& other);

	public:
		static ParticleEmitterPtr Create();

		ParticleEmitterPtr Clone();
		/** Loads a particle emitter.
		if reload is true, Particle Template will be kept the sharing state.
		*/
		bool Load(const char* filepath, bool reload);
		bool Update(float elapsedTime);
		unsigned GetEmitterID() const;		
		void Active(bool a, bool pending = false);
		void Stop();
		void ProcessDeleteOnStop(bool stopping, bool force = false);
		void StopImmediate();
		void SetVisibleParticle(bool visible);
		bool IsAlive();
		bool IsActive() const;
		void SetEmitterDirection(const fastbird::Vec3& dir);
		void SetEmitterColor(const Color& c);
		void SetAlpha(float alpha);
		float GetAlpha() const ;
		void UpdateEmit(float dt);
		void CopyDataToRenderer(float dt);
		void SetPosition(const Vec3& pos);
		void SetScale(const Vec3& scale);
		const Vec3& GetPosition() const;
		void SetDirection(const Vec3& dir);
		bool IsInfinite() const;
		void SetBufferSize(unsigned size);
		//void Sort();
		void SetLength(float length);
		void SetRelativeVelocity(const Vec3& dir, float speed);
		void RemoveShaderDefine(const char* def);
		void AddShaderDefine(const char* def, const char* val);
		void ApplyShaderDefine();	
	};
}