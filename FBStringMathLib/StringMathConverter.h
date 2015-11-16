#pragma once
#include "FBCommonHeaders/String.h"
#include "FBMathLib/Vec2.h"
#include "FBMathLib/Vec2I.h"
#include "FBMathLib/Vec3.h"
#include "FBMathLib/Vec3I.h"
#include "FBMathLib/Vec4.h"
#include "FBMathLib/Mat33.h"
#include "FBMathLib/Mat44.h"
#include "FBMathLib/Quat.h"
#include "FBMathLib/Rect.h"
namespace fastbird{
	class StringMathConverter{
	public:
		static TString ToString(const Vec2& val);
		static TString ToString(const Vec2I& val);
		static TString ToString(const Vec3& val);
		static TString ToString(const Vec3I& val);
		static TString ToString(const Vec4& val);
		static TString ToString(const Vec4& val, int w, int precision);
		static TString ToString(const Mat33& val);
		static TString ToString(const Mat44& val);
		static TString ToString(const Quat& val);
		static TString ToString(const Rect& val);

		static Vec2 ParseVec2(const TString& str, const Vec2& def = Vec2::ZERO);
		static Vec2I ParseVec2I(const TString& str, const Vec2I& def = Vec2I::ZERO);
		static Vec3 ParseVec3(const TString& str, const Vec3& def = Vec3::ZERO);
		static Vec3I ParseVec3I(const TString& str, const Vec3I& def = Vec3I::ZERO);
		static Vec4 ParseVec4(const TString& str, const Vec4& def = Vec4::ZERO);
		static Mat33 ParseMat33(const TString& str, const Mat33& def = Mat33::IDENTITY);
		static Mat44 ParseMat44(const TString& str, const Mat44& def = Mat44::IDENTITY);
		static Quat ParseQuat(const TString& str, const Quat& def = Quat::IDENTITY);
		static Rect ParseRect(const TString& str, const Rect& def = { 0, 0, 0, 0 });
	};
}
