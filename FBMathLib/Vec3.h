#pragma once
#include "MathDefines.h"
#include "Vec2.h"
namespace fastbird
{
	class Mat33;
	class Vec3I;
	class Vec3
	{
	public:

		Real x, y, z;

		static const Vec3 UNIT_X;
		static const Vec3 UNIT_Y;
		static const Vec3 UNIT_Z;
		static const Vec3 ZERO;
		static const Vec3 ONE;
		static const Vec3 MAX;
		static const Vec3 MIN;

		//-------------------------------------------------------------------
		Vec3();
		Vec3(Real _x, Real _y, Real _z);
		explicit Vec3(const Vec2& v2, Real _z);
		explicit Vec3(Real s);		
		explicit Vec3(const Vec3I& v);
		explicit Vec3(const char* s);

		//-------------------------------------------------------------------
		Vec3 operator+ (const Vec3& r) const;
		Vec3 operator+ (Real f) const;
		Vec3& operator+= (const Vec3& r);
		Vec3 operator- () const;
		Vec3 operator- (const Vec3& r) const;
		Vec3& operator-= (const Vec3& r);
		Vec3& operator-= (const Real f);
		Vec3 operator* (Real scalar) const;
		Vec3 operator* (const Vec3& v) const;
		Vec3 operator* (const Mat33& r) const;
		Vec3& operator*= (const Vec3& r);
		Vec3& operator*= (Real f);
		Vec3 operator/ (Real scalar) const;
		Vec3 operator/ (const Vec3& v) const;
		Vec3& operator/= (const Vec3& r);
		Vec3& operator/= (Real s);
		bool operator == (const Vec3& r) const;
		bool operator != (const Vec3& r) const;
		bool operator< (const Vec3& other) const;
		bool operator >= (const Vec3& other) const;
		bool operator <= (const Vec3& other) const;
		Real operator[] (const size_t i) const;
		Real& operator[] (const size_t i);
		Vec3 xyz();
		Vec3 yxy();
		Vec3 zzx();
		Vec2 xy();

		//-------------------------------------------------------------------
		Real Dot(const Vec3& vec) const;
		Real DistanceTo(const Vec3& other) const;
		Real DistanceToSQ(const Vec3&other) const;		
		void KeepGreater(const Vec3& other);
		void KeepLesser(const Vec3& other);		
		Vec3 Cross(const Vec3& rVector) const;
		int MaxAxis() const;
		void SafeNormalize();
		Real Normalize();
		Vec3 NormalizeCopy() const;
		Real Length() const;
		Real LengthSQ() const;
		Real AngleBetween(const Vec3& v) const;
		
	};

	Vec3 operator* (Real l, const Vec3& r);
	bool IsEqual(const Vec3& l, const Vec3& r, Real ep = EPSILON);
	bool IsEqual(const Vec2& l, const Vec2& r, Real ep = EPSILON);
	Vec3 Sign(const Vec3& v);
	Vec3 Floor(const Vec3& v);
	Vec3 Step(const Vec3& edge, const Vec3& v);
}

// serialization
std::istream& operator>>(std::istream& stream, fastbird::Vec3& v);
std::ostream& operator<<(std::ostream& stream, const fastbird::Vec3& v);

//
//// luawapper util
//template<>
//struct luaU_Impl<fastbird::Vec3>
//{
//	static fastbird::Vec3 luaU_check(lua_State* L, int index)
//	{
//		fastbird::LUA_STACK_WATCHER watcher(L, "static fastbird::Vec3 luaU_check(lua_State* L, int index)");
//		luaL_checktype(L, index, LUA_TTABLE);
//		fastbird::Vec3 ret;
//		lua_rawgeti(L, index, 1);
//		ret.x = (Real)luaL_checknumber(L, -1);		
//		lua_pop(L, 1);
//		lua_rawgeti(L, index, 2);
//		ret.y = (Real)luaL_checknumber(L, -1);
//		lua_pop(L, 1);
//		lua_rawgeti(L, index, 3);
//		ret.z = (Real)luaL_checknumber(L, -1);
//		lua_pop(L, 1);
//		return ret;
//	}
//
//	static fastbird::Vec3 luaU_to(lua_State* L, int index)
//	{
//		fastbird::LUA_STACK_WATCHER watcher(L, "static fastbird::Vec3 luaU_to(lua_State* L, int index)");
//		fastbird::Vec3 ret;
//		lua_rawgeti(L, index, 1);
//		ret.x = (Real)lua_tonumber(L, -1);
//		lua_pop(L, 1);
//		lua_rawgeti(L, index, 2);
//		ret.y = (Real)lua_tonumber(L, -1);
//		lua_pop(L, 1);
//		lua_rawgeti(L, index, 3);
//		ret.z = (Real)lua_tonumber(L, -1);
//		lua_pop(L, 1);
//		return ret;
//	}
//
//	static void luaU_push(lua_State* L, const fastbird::Vec3& val)
//	{
//		lua_createtable(L, 3, 0);
//		lua_pushnumber(L, val.x);
//		lua_rawseti(L, -2, 1);
//		lua_pushnumber(L, val.y);
//		lua_rawseti(L, -2, 2);
//		lua_pushnumber(L, val.z);
//		lua_rawseti(L, -2, 3);
//	}
//
//	static void luaU_push(lua_State* L, fastbird::Vec3& val)
//	{
//		lua_createtable(L, 3, 0);
//		lua_pushnumber(L, val.x);
//		lua_rawseti(L, -2, 1);
//		lua_pushnumber(L, val.y);
//		lua_rawseti(L, -2, 2);
//		lua_pushnumber(L, val.z);
//		lua_rawseti(L, -2, 3);
//	}
//};