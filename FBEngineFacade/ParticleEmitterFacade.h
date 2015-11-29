#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	FB_DECLARE_SMART_PTR(ParticleEmitterFacade);
	class FB_DLL_ENGINEFACADE ParticleEmitterFacade{
		FB_DECLARE_PIMPL_NON_COPYABLE(ParticleEmitterFacade);
		ParticleEmitterFacade();
		~ParticleEmitterFacade();

	public:
		static ParticleEmitterFacadePtr Create();
		bool Load(unsigned id);
		bool IsValid() const;

		void SetTransformation(const Transformation& t);
		const Transformation& GetTransformation() const;
		void SetPosition(const Vec3& pos);
		void Active(bool active, bool pending = false);
		bool IsActive() const;
		void StopImmediate();
		bool GetVisible();
		void SetVisible(bool visible);
		float GetAlpha() const;
		void SetAlpha(float alpha);
		void SetScale(const Vec3& scale);
		void SetLength(float length);
		bool IsAlive() const;
		void Stop();

		void RemoveShaderDefine(const char* def);
		void AddShaderDefine(const char* def, const char* val);
		void ApplyShaderDefine();
		void SetRelativeVelocity(const Vec3& dir, float speed);

	};
}