#pragma once
#include "AABB.h"
#include "Transformation.h"
namespace fastbird
{

class OBB
{
public:
	Transformation mTransform;
	AABB mAABB;
};

}