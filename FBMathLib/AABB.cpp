#include "stdafx.h"
#include "AABB.h"
using namespace fastbird;
#undef min
#undef max

AABB::AABB()
{
	Invalidate();
}

bool AABB::IsValid() const
{
	return mMax >= mMin;
}

void AABB::Invalidate()
{
	mMin = Vec3::MAX;
	mMax = Vec3::MIN;
}

void AABB::Merge(const Vec3& point)
{
	mMin.KeepLesser(point);
	mMax.KeepGreater(point);
}

void AABB::Merge(const AABB& aabb){
	mMin.x = std::min(mMin.x, aabb.mMin.x);
	mMin.y = std::min(mMin.y, aabb.mMin.y);
	mMin.z = std::min(mMin.z, aabb.mMin.z);
	mMax.x = std::max(mMax.x, aabb.mMax.x);
	mMax.y = std::max(mMax.y, aabb.mMax.y);
	mMax.z = std::max(mMax.z, aabb.mMax.z);
}

const Vec3& AABB::GetMin() const { return mMin; }
const Vec3& AABB::GetMax() const { return mMax; }

void AABB::SetMin(const Vec3& min) { mMin = min; }
void AABB::SetMax(const Vec3& max) { mMax = max; }

Vec3 AABB::GetCenter() const { return (mMin + mMax) * .5f; }

void AABB::Translate(const Vec3& pos)
{
	mMin += pos;
	mMax += pos;
}

bool AABB::Contain(const Vec3&pos) const
{
	if (pos.x < mMin.x || pos.y < mMin.y || pos.z < mMin.z ||
		pos.x > mMax.x || pos.y > mMax.y || pos.z > mMax.z)
		return false;

	return true;
}