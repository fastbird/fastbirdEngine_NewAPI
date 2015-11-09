#pragma once
#include "FBCommonHeaders/String.h"
namespace fastbird{
	class Vec2; 
	class Vec2I;
	class Vec3;
	class Vec3I;
	class Vec4;
	class Mat33;
	class Mat44;
	class Quat;
	struct Rect;

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

		static Vec2 ParseVec2(const TString& str, const Vec2& def);
		static Vec2I ParseVec2I(const TString& str, const Vec2I& def);
		static Vec3 ParseVec3(const TString& str, const Vec3& def);
		static Vec3I ParseVec3I(const TString& str, const Vec3I& def);
		static Vec4 ParseVec4(const TString& str, const Vec4& def);
		static Mat33 ParseMat33(const TString& str, const Mat33& def);
		static Mat44 ParseMat44(const TString& str, const Mat44& def);
		static Quat ParseQuat(const TString& str, const Quat& def);
		static Rect ParseRect(const TString& str, const Rect& def);
	};
}