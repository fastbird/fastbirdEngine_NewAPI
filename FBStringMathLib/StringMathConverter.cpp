#include "StringMathConverter.h"
#include "FBMathLib/Vec2.h"
#include "FBMathLib/Vec2I.h"
#include "FBMathLib/Vec3.h"
#include "FBMathLib/Vec3I.h"
#include "FBMathLib/Vec4.h"
#include "FBMathLib/Mat33.h"
#include "FBMathLib/Mat44.h"
#include "FBMathLib/Quat.h"
#include "FBMathLib/Rect.h"
#include "FBStringLib/StringLib.h"
#include "FBStringLib/StringConverter.h"
#include <sstream>
#include <iomanip>
#include <assert.h>

namespace fastbird{

TString StringMathConverter::ToString(const Vec2& val){
	std::_tstringstream stream;
	stream << val.x << " " << val.y;
	return stream.str();
}
TString StringMathConverter::ToString(const Vec2I& val){
	std::_tstringstream stream;
	stream << val.x << " " << val.y;
	return stream.str();
}
TString StringMathConverter::ToString(const Vec3& val){
	std::_tstringstream stream;
	stream << val.x << " " << val.y << " " << val.z;
	return stream.str();
}
TString StringMathConverter::ToString(const Vec3I& val){
	std::_tstringstream stream;
	stream << val.x << " " << val.y << " " << val.z;
	return stream.str();
}
TString StringMathConverter::ToString(const Vec4& val){
	std::_tstringstream stream;
	stream << val.x << " " << val.y << " " << val.z << " " << val.w;
	return stream.str();
}
TString StringMathConverter::ToString(const Vec4& val, int w, int precision){
	std::_tstringstream stream;
	stream << std::setw(w);
	stream.precision(precision);
	stream << std::fixed;
	stream << val.x << " " << val.y << " " << val.z << " " << val.w;
	return stream.str();
}
TString StringMathConverter::ToString(const Mat33& val){
	std::_tstringstream stream;
	stream << val[0][0] << " "
		<< val[0][1] << " "
		<< val[0][2] << " "
		<< val[1][0] << " "
		<< val[1][1] << " "
		<< val[1][2] << " "
		<< val[2][0] << " "
		<< val[2][1] << " "
		<< val[2][2];
	return stream.str();
}
TString StringMathConverter::ToString(const Mat44& val){
	std::_tstringstream stream;
	stream << val[0][0] << " "
		<< val[0][1] << " "
		<< val[0][2] << " "
		<< val[0][3] << " "
		<< val[1][0] << " "
		<< val[1][1] << " "
		<< val[1][2] << " "
		<< val[1][3] << " "
		<< val[2][0] << " "
		<< val[2][1] << " "
		<< val[2][2] << " "
		<< val[2][3] << " "
		<< val[3][0] << " "
		<< val[3][1] << " "
		<< val[3][2] << " "
		<< val[3][3];
	return stream.str();
}
TString StringMathConverter::ToString(const Quat& val){
	std::_tstringstream stream;
	stream << val.w << " " << val.x << " " << val.y << " " << val.z;
	return stream.str();
}
TString StringMathConverter::ToString(const Rect& val){
	std::_tstringstream stream;
	stream << val.left << " " << val.top << " " << val.right << " " << val.bottom;
	return stream.str();
}

Vec2 StringMathConverter::ParseVec2(const TString& str, const Vec2& def){
	// Split on space
	TStringVector vec = Split(str);

	if (vec.size() < 2)
	{
		return def;
	}
	else
	{
		return Vec2(StringConverter::ParseReal(vec[0]), StringConverter::ParseReal(vec[1]));
	}
}
Vec2I StringMathConverter::ParseVec2I(const TString& str, const Vec2I& def){
	// Split on space
	TStringVector vec = Split(str);

	if (vec.size() < 2)
	{
		return def;
	}
	else
	{
		return Vec2I(StringConverter::ParseInt(vec[0]), StringConverter::ParseInt(vec[1]));
	}
}
Vec3 StringMathConverter::ParseVec3(const TString& str, const Vec3& def){
	// Split on space
	TStringVector vec = Split(str);

	if (vec.size() < 3)
	{
		return def;
	}
	else
	{
		return Vec3(StringConverter::ParseReal(vec[0]), StringConverter::ParseReal(vec[1]), 
			StringConverter::ParseReal(vec[2]));
	}
}
Vec3I StringMathConverter::ParseVec3I(const TString& str, const Vec3I& def){
	TStringVector vec = Split(str);

	if (vec.size() < 3)
	{
		return def;
	}
	else
	{
		return Vec3I(StringConverter::ParseInt(vec[0]), StringConverter::ParseInt(vec[1]),
			StringConverter::ParseInt(vec[2]));
	}
}
Vec4 StringMathConverter::ParseVec4(const TString& str, const Vec4& def){
	// Split on space
	TStringVector vec = Split(str);

	if (vec.size() < 4)
	{
		return def;
	}
	else
	{
		return Vec4(StringConverter::ParseReal(vec[0]), StringConverter::ParseReal(vec[1]), 
			StringConverter::ParseReal(vec[2]), StringConverter::ParseReal(vec[3]));
	}
}
Mat33 StringMathConverter::ParseMat33(const TString& str, const Mat33& def){
	// Split on space
	TStringVector vec = Split(str);

	if (vec.size() < 9)
	{
		return def;
	}
	else
	{
		return Mat33(StringConverter::ParseReal(vec[0]), StringConverter::ParseReal(vec[1]), StringConverter::ParseReal(vec[2]),
			StringConverter::ParseReal(vec[3]), StringConverter::ParseReal(vec[4]), StringConverter::ParseReal(vec[5]),
			StringConverter::ParseReal(vec[6]), StringConverter::ParseReal(vec[7]), StringConverter::ParseReal(vec[8]));
	}
}
Mat44 StringMathConverter::ParseMat44(const TString& str, const Mat44& def){
	// Split on space
	TStringVector vec = Split(str);

	if (vec.size() < 16)
	{
		return def;
	}
	else
	{
		return Mat44(StringConverter::ParseReal(vec[0]), StringConverter::ParseReal(vec[1]), StringConverter::ParseReal(vec[2]), StringConverter::ParseReal(vec[3]),
			StringConverter::ParseReal(vec[4]), StringConverter::ParseReal(vec[5]), StringConverter::ParseReal(vec[6]), StringConverter::ParseReal(vec[7]),
			StringConverter::ParseReal(vec[8]), StringConverter::ParseReal(vec[9]), StringConverter::ParseReal(vec[10]), StringConverter::ParseReal(vec[11]),
			StringConverter::ParseReal(vec[12]), StringConverter::ParseReal(vec[13]), StringConverter::ParseReal(vec[14]), StringConverter::ParseReal(vec[15]));
	}
}
Quat StringMathConverter::ParseQuat(const TString& str, const Quat& def){
	// Split on space
	TStringVector vec = Split(str);

	if (vec.size() < 4)
	{
		return def;
	}
	else
	{
		return Quat(StringConverter::ParseReal(vec[0]), StringConverter::ParseReal(vec[1]), StringConverter::ParseReal(vec[2]), StringConverter::ParseReal(vec[3]));
	}
}
Rect StringMathConverter::ParseRect(const TString& str, const Rect& def){
	TStringVector vec = Split(str);
	Rect value = { def.left, def.top, def.right, def.bottom };
	if (vec.size() == 4)
	{
		value.left = StringConverter::ParseLong(vec[0]);
		value.top = StringConverter::ParseLong(vec[1]);
		value.right = StringConverter::ParseLong(vec[2]);
		value.bottom = StringConverter::ParseLong(vec[3]);
	}
	else
	{
		assert(0);
	}

	return value;
}

}