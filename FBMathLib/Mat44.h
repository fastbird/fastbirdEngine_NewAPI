#pragma once

#include "Mat33.h"
#include "Vec3.h"
#include "Vec4.h"

namespace fastbird
{
	class Mat44
	{
	public:

		Real m[4][4];

		static const Mat44 IDENTITY;

		//-------------------------------------------------------------------------
		Mat44();
		Mat44(Real m11, Real m12, Real m13, Real m14,
			Real m21, Real m22, Real m23, Real m24,
			Real m31, Real m32, Real m33, Real m34,
			Real m41, Real m42, Real m43, Real m44);
		Mat44(const Mat33& mat33, const Vec3& translation);

		//-------------------------------------------------------------------------
		Vec4 operator*(const Vec4& columnVector) const;
		Vec3 operator*(const Vec3& columnVector) const;
		Mat44 operator* (const Mat44& other) const;
		Mat44& operator*=(const Mat44& other);
		bool operator==(const Mat44& other) const;
		Real* operator[](size_t iRow) const;

		//-------------------------------------------------------------------------
		 void MakeIdentity();
		 void MakeTranslation(const Vec3& pos);
		Mat44 Inverse() const;
		Mat44 InverseAffine() const;
		bool IsAffine(void) const;
		Mat44 Transpose(void) const;		
		const Vec4& Row(size_t iRow) const;		
		void SetTranslation(const Vec3& pos);
		Vec3 GetTranslation() const;
		Mat33 To33() const;
	};
}