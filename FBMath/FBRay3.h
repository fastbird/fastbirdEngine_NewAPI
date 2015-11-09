#pragma once

#include "FBVec3.h"
#include "FBVec3I.h"
#include "FBPlane3.h"

namespace fastbird
{
	class BoundingVolume;
	class AABB;
	class Ray3
	{
		Vec3 mOrigin;
		Vec3 mDir;
		Vec3 mDirInv;
		Vec3I mSigns;

	public:
		Ray3();
		Ray3(const Vec3& origin, const Vec3& dir);

		// IntersectionResult
		typedef std::pair<bool, Real> IResult;
		IResult intersects(BoundingVolume* pBoundingVolume) const;
		IResult intersects(const AABB& aabb, Vec3& normal) const;
		IResult intersects(const Plane3& p) const;

		const Vec3& GetDir() const { return mDir; }
		const Vec3& GetOrigin() const { return mOrigin; }
		void SetOrigin(const Vec3& v) { mOrigin = v; }
		void SetDir(const Vec3& dir);
		Vec3 GetPoint(Real dist) const { return mOrigin + mDir * dist; }
		void AddOffset(const Vec3& v) { mOrigin  += v;}		
	};
}