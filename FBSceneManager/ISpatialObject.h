#pragma once
namespace fastbird{
	class Vec3;
	DECLARE_SMART_PTR(ISpatialObject);
	DECLARE_SMART_PTR(BoundingVolume);
	class ISpatialObject{
	public:
		virtual const Vec3& GetPosition() const = 0;
		virtual BoundingVolumePtr GetBoundingVolumeWorld() = 0;

	};
}