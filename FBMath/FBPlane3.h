#pragma once
#ifndef _fastbird_Plane3_header_included_
#define _fastbird_Plane3_header_included_

#include "FBVec3.h"

namespace fastbird
{
	class Plane3
	{
	public:
		Vec3 mNormal;
		Real mConstant;

		//Dot(N,X) = c
		Plane3();
		Plane3(const Plane3& other);
		Plane3(const Vec3& normal, Real c);
		Plane3(const Vec3& normal, const Vec3& p);
		Plane3(const Vec3& p0, const Vec3& p1, const Vec3& p2);

		//-------------------------------------------------------------------
		Plane3 operator=(const Plane3& other);

		//-------------------------------------------------------------------
		int WhichSide(const Vec3& p) const;
		Real DistanceTo(const Vec3& p) const;		
	};
}

#endif //_fastbird_Plane3_header_included_