#pragma once
#include "FBCommonHeaders/Types.h"
#include "FBMathLib/Math.h"
namespace fastbird
{
	DECLARE_SMART_PTR(PointLight);
	class PointLight
	{
		DECLARE_PIMPL_NON_COPYABLE(PointLight);
		friend class PointLightMan;
		PointLight(const Vec3& pos, Real range, const Vec3& color, Real intensity, Real lifeTime, bool manualDeletion);

	public:
		static PointLightPtr Create(const Vec3& pos, Real range, const Vec3& color, Real intensity, Real lifeTime, bool manualDeletion);		
		
		void SetPosition(const Vec3& pos);
		const Vec3& GetPosition() const;
		void SetRange(Real range);
		void SetColorAndIntensity(const Vec3& color, Real intensity);
		Real GetRange() const;
		const Vec3& GetColor() const;
		const Vec3& GetColorPowered() const;
		Real GetIntensity() const;
		void SetLifeTime(Real lifeTime);
		Real GetLifeTime() const;
		void SetManualDeletion(bool manual);
		bool GetManualDeletion() const;
		void SetAlpha(Real alpha);
		bool GetEnabled() const;
		void SetEnabled(bool enable);
		Real GetOlder(Real time);
	};
}