#pragma once
#include "Vec3.h"

namespace fastbird
{
	//-------------------------------------------------------------------------
	// max is included
	inline Real Random()
	{
		return (Real)std::rand() / (Real)RAND_MAX;
	}
	template<class T>
	inline  T Random(T min, T max)
	{
		int divider = (int)(max - min + 1);
		if (divider != 0)
			return min + (std::rand() % divider);
		return min;
	}

	inline Real Random(Real min, Real max)
	{
		auto k = max - min;
		auto offset = (Real)(((Real)std::rand() / (Real)RAND_MAX) * k);
		auto result = min + offset;
		return result;
	}

	inline Vec3 Random(const Vec3& min, const Vec3& max) 
	{
		return Vec3(Random(min.x, max.x),
			Random(min.y, max.y),
			Random(min.z, max.z));
	};
	inline Vec3 RandomDirection()
	{
		return Random(Vec3(-1, -1, -1), Vec3(1, 1, 1)).NormalizeCopy();
	}

	inline Vec2 Random(const Vec2& min, const Vec2& max) 
	{
		return Vec2(Random(min.x, max.x),
			Random(min.y, max.y));
	};

	//-------------------------------------------------------------------------
	template<class T>
	class Randomizer
	{
	public:
		Randomizer(T min, T max)
		{
			mMin = min;
			mMax = max;
		}
		T Get(T value)
		{
			T factor = Random(mMin, mMax);
			return value + factor;

		}

	private:
		T mMin;
		T mMax;
	};
}