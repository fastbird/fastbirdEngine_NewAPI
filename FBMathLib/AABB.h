/*
 -----------------------------------------------------------------------------
 This source file is part of fastbird engine
 For the latest info, see http://www.jungwan.net/
 
 Copyright (c) 2013-2015 Jungwan Byun
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
*/

#pragma once
#include "Vec3.h"
#include <algorithm>
namespace fastbird
{
	class AABB
	{
	public:
		AABB()
		{
			Invalidate();
		}

		bool IsValid() const
		{
			return mMax >= mMin;
		}

		void Invalidate()
		{
			mMin = Vec3::MAX;
			mMax = Vec3::MIN;
		}

		void Merge(const Vec3& point)
		{
			mMin.KeepLesser(point);
			mMax.KeepGreater(point);
		}

		void Merge(const AABB& aabb){
			mMin.x = std::min(mMin.x, aabb.mMin.x);
			mMin.y = std::min(mMin.y, aabb.mMin.y);
			mMin.z = std::min(mMin.z, aabb.mMin.z);
			mMax.x = std::max(mMax.x, aabb.mMax.x);
			mMax.y = std::max(mMax.y, aabb.mMax.y);
			mMax.z = std::max(mMax.z, aabb.mMax.z);
		}

		const Vec3& GetMin() const { return mMin; }
		const Vec3& GetMax() const { return mMax; }

		void SetMin(const Vec3& min) { mMin = min;}
		void SetMax(const Vec3& max) { mMax = max;}

		Vec3 GetCenter() const { return (mMin + mMax) * .5f; }

		void Translate(const Vec3& pos)
		{
			mMin += pos;
			mMax += pos;
		}

		bool Contain(const Vec3&pos) const
		{
			if (pos.x < mMin.x || pos.y < mMin.y || pos.z < mMin.z ||
				pos.x > mMax.x || pos.y > mMax.y || pos.z > mMax.z)
				return false;

			return true;
		}


	private:
		Vec3 mMin;
		Vec3 mMax;
	};
}