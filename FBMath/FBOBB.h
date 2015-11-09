#pragma once
#include "FBAABB.h"
#include "FBTransformation.h"
namespace fastbird
{

class OBB
{
public:
	Transformation mTransform;
	AABB mAABB;
};

}