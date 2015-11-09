#include "stdafx.h"
#include "Vec3I.h"

namespace fastbird
{
	const Vec3I Vec3I::UNIT_X = Vec3I(1, 0, 0);
	const Vec3I Vec3I::UNIT_Y = Vec3I(0, 1, 0);
	const Vec3I Vec3I::UNIT_Z = Vec3I(0, 0, 1);
	const Vec3I Vec3I::ZERO = Vec3I(0, 0, 0);
	const Vec3I Vec3I::MAX = Vec3I(INT_MAX, INT_MAX, INT_MAX);

	//-------------------------------------------------------------------
	Vec3I::Vec3I()
	{
	}

	Vec3I::Vec3I(int _x, int _y, int _z)
		: x(_x), y(_y), z(_z)
	{
	}

	Vec3I::Vec3I(const Vec3& v)
	{
		x = (int)v.x;
		y = (int)v.y;
		z = (int)v.z;
	}

	//-------------------------------------------------------------------
	Vec3I Vec3I::operator+ (int s) const
	{
		return Vec3I(x + s, y + s, z + s);
	}

	Vec3I Vec3I::operator+ (const Vec3I& v) const
	{
		return Vec3I(x + v.x, y + v.y, z + v.z);
	}

	Vec3I Vec3I::operator- (int s) const
	{
		return Vec3I(x - s, y - s, y - s);
	}

	Vec3I Vec3I::operator- (const Vec3I& v) const
	{
		return Vec3I(x - v.x, y - v.y, z - v.z);
	}

	Vec3I Vec3I::operator* (int s) const
	{
		return Vec3I(x*s, y*s, z*s);
	}

	Vec3 Vec3I::operator*(Real s) const
	{
		return Vec3(x*s, y*s, z*s);
	}

	Vec3I Vec3I::operator* (const Vec3I& v) const
	{
		return Vec3I(x*v.x, y*v.y, z*v.z);
	}

	Vec3I Vec3I::operator/ (int s) const
	{
		return Vec3I(x / s, y / s, z / s);
	}	

	Vec3I Vec3I::operator/ (const Vec3I& v) const
	{
		return Vec3I(x / v.x, y / v.y, z / v.z);
	}

	bool Vec3I::operator== (const Vec3I& v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	bool Vec3I::operator!=(const Vec3I& v) const
	{
		return !operator==(v);
	}

	bool Vec3I::operator<(const Vec3I& other) const
	{
		return memcmp(this, &other, sizeof(Vec3I)) < 0;
	}

	//-------------------------------------------------------------------
	Real Vec3I::length() const
	{
		return (Real)sqrt(x*x + y*y + z*z);
	}

	Real Vec3I::lengthSQ() const
	{
		return (Real)(x*x + y*y + z*z);
	}

	Real Vec3I::distance(const Vec3I& to) const{
		Vec3I dif = to - *this;
		return dif.length();
	}

	Real Vec3I::distanceSQ(const Vec3I& to) const{
		Vec3I dif = to - *this;
		return dif.lengthSQ();
	}
}