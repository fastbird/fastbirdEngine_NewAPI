#pragma once
#include "FBCollisionShape.h"
namespace fastbird
{
	class IMeshObject;
	struct CollisionInfo
	{
		FBColShape::Enum mColShapeType;
		Transformation mTransform;
		MeshObjectPtr mCollisionMesh;
	};

	typedef std::vector< CollisionInfo > COLLISION_INFOS;
}