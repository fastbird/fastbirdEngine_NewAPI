#pragma once
#include "FBMathLib/Vec3.h"
namespace fastbird
{
	class RigidBody;
	struct RayResultClosest
	{
		RayResultClosest();
		RayResultClosest(RigidBody* body, const Vec3& hitPoint, const Vec3& normal, int index);

		RigidBody* mRigidBody;
		Vec3 mHitPointWorld;
		Vec3 mHitNormalWorld;
		int mIndex; // child index for compound shapes
	};

	struct RayResultWithObj
	{
		RayResultWithObj(RigidBody* body);

		RigidBody* mTargetBody;
		RigidBody* mRigidBody;
		Vec3 mHitPointWorld;
		Vec3 mHitNormalWorld;
		int mIndex; // child index for compound shapes
	};

	struct RayResultAll
	{
		static const int SIZE = 200;
		RayResultAll();
		~RayResultAll();
		void AddResult(RigidBody* rigidBody, const Vec3& hitPoint, const Vec3& hitNormal, int index);

		RayResultClosest* mRayResults[SIZE];
		unsigned mCurSize;
	};
}