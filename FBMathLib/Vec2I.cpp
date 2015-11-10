#include "stdafx.h"
#include "Vec2I.h"
#include "Math.h"

namespace fastbird
{
	const Vec2I Vec2I::ZERO(0, 0);

	//-------------------------------------------------------------------
	Vec2I::Vec2I()
	{
	}

	Vec2I::Vec2I(int _x, int _y)
		: x(_x), y(_y)
	{
	}

	Vec2I::Vec2I(const Vec2& v){
		x = Round(v.x);
		y = Round(v.y);
	}	

	//-------------------------------------------------------------------
	Vec2I Vec2I::operator+ (int s) const
	{
		return Vec2I(x + s, y + s);
	}

	Vec2I Vec2I::operator+ (const Vec2I& v) const
	{
		return Vec2I(x + v.x, y + v.y);
	}

	Vec2I& Vec2I::operator+=(const Vec2I& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vec2I Vec2I::operator- (int s) const
	{
		return Vec2I(x - s, y - s);
	}

	Vec2I Vec2I::operator- (const Vec2I& v) const
	{
		return Vec2I(x - v.x, y - v.y);
	}

	Vec2I& Vec2I::operator-=(const Vec2I& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vec2I Vec2I::operator* (int s) const
	{
		return Vec2I(x*s, y*s);
	}

	Vec2I Vec2I::operator* (const Vec2I& v) const
	{
		return Vec2I(x*v.x, y*v.y);
	}

	Vec2I Vec2I::operator/ (int s) const
	{
		return Vec2I(x / s, y / s);
	}	

	Vec2I Vec2I::operator/ (const Vec2I& v) const
	{
		return Vec2I(x / v.x, y / v.y);
	}

	bool Vec2I::operator== (const Vec2I& v) const
	{
		return x == v.x && y == v.y;
	}

	bool Vec2I::operator!=(const Vec2I& v) const
	{
		return !operator==(v);
	}

	bool Vec2I::operator<(const Vec2I& v) const
	{
		if (x < v.x)
		{
			return true;
		}
		else if (x == v.x)
		{
			if (y<v.y)
			{
				return true;
			}
		}

		return false;
	}

	int Vec2I::Cross(const Vec2I& right){
		return x * right.y - y * right.x;
	}
	int Vec2I::Dot(const Vec2I& right){
		return x * right.x + y * right.y;
	}
	
	Real Vec2I::DistanceTo(const Vec2I& d) const{
		return (*this - d).Length();
	}

	Real Vec2I::Length() const{
		return (Real)std::sqrt(x*x + y*y);
	}

	Vec2I Vec2I::Scale(Real f) const {
		return Vec2I(Round(x*f), Round(y*f));
	}
}