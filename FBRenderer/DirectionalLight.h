#pragma once
#include "FBCommonHeaders/platform.h"
namespace fastbird
{
	DECLARE_SMART_PTR(DirectionalLight);
	//------------------------------------------------------------------------
	class FB_DLL_PUBLIC DirectionalLight
	{
		DECLARE_PIMPL(DirectionalLight);

	public:
		DirectionalLight();
		~DirectionalLight();

		const Vec3& GetPosition();
		const Vec3& GetDiffuse();
		const Vec3& GetSpecular();
		Real GetIntensity() const;
		Real GetAttenuation() const;
		Real GetExponent() const;

		void AddTheta(Real radian);
		void AddPhi(Real radian);

		void SetPosition(const Vec3& pos);
		void SetDiffuse(const Vec3& diffuse);
		void SetSpecular(const Vec3& specular);
		void SetIntensity(Real intensity);
		void SetAttenuation(Real attenuation){}
		void SetExponent(Real exponent) {}

		void PrepareInterpolation(Real destTheta, Real destPhi, Real destIntensity, const Vec3& destDiffuse, Real time);
		void AddInterpolTime(Real time);
		const Vec3& GetInterpolDir(unsigned target) const;
		Real GetInterpolIntensity(unsigned target) const;
		const Vec3& GetInterpolDiffuse(unsigned target) const;
		Real GetTheta() const;
		Real GetPhi() const;

		void Update(Real dt);

		void PreRender();
		void Render();
		void PostRender();

		void CopyLight(DirectionalLight* other);
		
	};
}
