#include "stdafx.h"
#include "RayResult.h"
using namespace fastbird;

RayResultClosest::RayResultClosest()
	: mRigidBody(0), mHitNormalWorld(0, 1, 0), mHitPointWorld(0, 0, 0), mIndex(-1)
{

}

RayResultClosest::RayResultClosest(RigidBody* body, const Vec3& hitPoint, const Vec3& normal, int index)
	:mRigidBody(body), mHitPointWorld(hitPoint), mHitNormalWorld(normal), mIndex(index)
{

}


RayResultWithObj::RayResultWithObj(RigidBody* body)
	: mTargetBody(body), mRigidBody(0), mHitNormalWorld(0, 1, 0), mHitPointWorld(0, 0, 0), mIndex(-1)
{

}

RayResultAll::RayResultAll()
	:mCurSize(0)
{

}

RayResultAll::~RayResultAll()
{
	for (unsigned i = 0; i < mCurSize; ++i)
	{
		mRayResults[i]->~RayResultClosest();
		free(mRayResults[i]);
	}
}

void RayResultAll::AddResult(RigidBody* rigidBody, const Vec3& hitPoint, const Vec3& hitNormal, int index)
{
	if (mCurSize >= SIZE)
		return;

	mRayResults[mCurSize++] = (RayResultClosest*)malloc(sizeof(RayResultClosest));

	new (mRayResults[mCurSize - 1]) RayResultClosest(rigidBody, hitPoint, hitNormal, index);
}