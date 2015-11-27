#pragma once
#include "FBSceneObjectFactory/FBCollisionShape.h"
namespace fastbird{
	struct CollisionShapeInfo{
		FBColShape::Enum mType;
		Vec3 mOffset;
		Quat mRot;
		Vec3 mScale;
		Vec3s mPositions; // for meshes
	};
	typedef std::vector<CollisionShapeInfo> CollisionShapeInfos;
}