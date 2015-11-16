#pragma once
#include "SceneObject.h"
#include "FBCommonHeaders/Types.h"
#include "FBMathLib/Math.h"
#include "FBMathLib/BoundingVolume.h"
namespace fastbird{
	DECLARE_SMART_PTR(Animation);
	DECLARE_SMART_PTR(SpatialObject);
	class FB_DLL_PUBLIC SpatialObject : public SceneObject{
		DECLARE_PIMPL_CLONEABLE(SpatialObject);		

	protected:
		SpatialObject();
		~SpatialObject();

	public:
		void SetRadius(Real r);
		Real GetRadius() const;
		void SetDistToCam(Real dist);
		Real GetDistToCam() const;
		const Vec3& GetPosition() const;
		const Vec3& GetScale() const;
		void SetPosition(const Vec3& pos);
		BoundingVolumePtr GetBoundingVolume();
		BoundingVolumePtr GetBoundingVolumeWorld();
		const Transformation& GetLocation() const;
		const Transformation& GetAnimatedLocation() const;
		void SetLocation(const Transformation& t);
		bool GetTransformChanged() const;
		void ClearTransformChanged();
		void SetAnimation(AnimationPtr anim);
		void UpdateAnimation(TIME_PRECISION dt);
		bool IsPlayingAction() const;
	};
}