#include "stdafx.h"
#include "Vec4.h"
#include "Math.h"
namespace fastbird
{

const Vec4 Vec4::UNIT_X(1.f, 0, 0, 0);
const Vec4 Vec4::UNIT_Y(0, 1.f, 0, 0);
const Vec4 Vec4::UNIT_Z(0, 0, 1.f, 0);
const Vec4 Vec4::ZERO(0, 0, 0, 0);

//-------------------------------------------------------------------------
Vec4::Vec4() {}
Vec4::Vec4(Real _x, Real _y, Real _z, Real _w)
	: x(_x), y(_y), z(_z), w(_w)
{
}
Vec4::Vec4(const Vec3& xyz, Real _w)
	: x(xyz.x), y(xyz.y), z(xyz.z), w(_w)
{
}
Vec4::Vec4(const Vec3& xyz)
	:x(xyz.x), y(xyz.y), z(xyz.z), w(1.f)
{}

Vec4::Vec4(const char* s)
{
	char* next;
	x = 0;
	y = 0;
	z = 0;
	w = 1.0f;
	if (s!=0)
	{
		x = (Real)strtod(s, &next);
		if (next!=0)
		{
			StepToDigit_(&next);
			y = (Real)strtod(next, &next);
			if (next!=0)
			{
				StepToDigit_(&next);
				z = (Real)strtod(next, &next);
				if (next!=0)
				{
					StepToDigit_(&next);
					if (next!=0)
						w = (Real)strtod(next, 0);
				}

			}
		}
	}			
}

//-------------------------------------------------------------------------
Vec4 Vec4::operator+ (const Vec4& r) const
{
	return Vec4(x + r.x, y + r.y, z + r.z, w + r.w);
}

Vec4 Vec4::operator+(Real v)
{
	return Vec4(v + x, v + y, v + z, v + w);
}

Vec4 Vec4::operator* (Real scalar) const
{
	return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vec4& Vec4::operator*= (Real scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}

Vec4& Vec4::operator*= (const Vec4& r)
{
	x *= r.x;
	y *= r.y;
	z *= r.z;
	w *= r.w;
	return *this;
}

Vec4 Vec4::operator* (const Vec4& r) const
{
	return Vec4(x*r.x, y*r.y, z*r.z, w*r.w);
}

Vec4& Vec4::operator/=(Real scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;
	return *this;
}

bool Vec4::operator== (const Vec4& other) const
{
	return (IsEqual(x, other.x) && IsEqual(y, other.y) && IsEqual(z, other.z) && IsEqual(w, other.w));
}

//-------------------------------------------------------------------------
Real Vec4::Dot(const Vec4& other) const
{
	return x * other.x + y * other.y + z * other.z + w * other.w;
}

void Vec4::SetXYZ(const Vec3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

Vec3 Vec4::ToVec3() const
{
	return Vec3(x, y, z);
}

}