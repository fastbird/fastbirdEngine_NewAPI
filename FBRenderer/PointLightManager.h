#pragma once
#include "FBCommonHeaders/Types.h"
#include "FBMathLib/Math.h"
namespace fastbird
{
	struct POINT_LIGHT_CONSTANTS;
	DECLARE_SMART_PTR(PointLight);
	DECLARE_SMART_PTR(PointLightManager);
	class PointLightManager
	{
		DECLARE_PIMPL_NON_COPYABLE(PointLightManager);
		PointLightManager();

	public:
		static PointLightManagerPtr Create();
		~PointLightManager();

		PointLightPtr CreatePointLight(const Vec3& pos, Real range, const Vec3& color, Real intensity, Real lifeTime, bool manualDeletion);
		void DeletePointLight(PointLightPtr pointLight);
		void Update(Real dt);
		void GatherPointLightData(const BoundingVolume* aabb, const Transformation& transform, POINT_LIGHT_CONSTANTS* plConst);
		unsigned GetNumPointLights() const;
	};
}