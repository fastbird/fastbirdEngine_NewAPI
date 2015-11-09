#pragma once
#ifndef _Mat33_header_included_
#define _Mat33_header_included_

#include "FBVec3.h"

namespace fastbird
{
	class Mat33
	{
	public:
	
		Real m[3][3];

		static const Mat33 IDENTITY;
		static const Mat33 ZERO;

		//-------------------------------------------------------------------
		Mat33();
		Mat33(Real m11, Real m12, Real m13,
			Real m21, Real m22, Real m23,
			Real m31, Real m32, Real m33);
		Mat33(const Vec3& x, const Vec3& y, const Vec3& z);

		//-------------------------------------------------------------------
		Mat33 operator-() const;
		Mat33 operator* (const Mat33& rMat) const;
		Vec3 operator* (const Vec3& rVec) const;
		Mat33 operator/ (Real k) const;
		void operator/=(Real k);
		Real* operator[](size_t iRow) const;
		bool operator==(const Mat33& other) const;

		//-------------------------------------------------------------------
		void MakeIdentity();		
		void FromAxisAngle(const Vec3& axis, Real radian);
		Mat33 Inverse() const;
		Mat33 Transpose() const;
		Mat33 ScaleAxis(const Vec3& scale) const;
		Vec3 Column(int index) const;
		void SetColumn(int index, const Vec3& v);
	};
}
#endif //_Mat33_header_included_