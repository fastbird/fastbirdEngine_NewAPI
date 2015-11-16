#include "stdafx.h"
#include "BoundingVolume.h"
#include "BVaabb.h"
#include "BVSphere.h"

namespace fastbird{
	BoundingVolumePtr BoundingVolume::Create(BVType type /*= BV_SPHERE*/)
	{
		if (type == BV_SPHERE)
			return BoundingVolumePtr(new BVSphere);
		else if (type == BV_AABB)
			return BoundingVolumePtr(new BVaabb);

		else
		{
			assert(0);
			return 0;
		}
	}

	BoundingVolume& BoundingVolume::operator = (const BoundingVolume& other){
		mAlwaysPass = other.mAlwaysPass;
	}
}