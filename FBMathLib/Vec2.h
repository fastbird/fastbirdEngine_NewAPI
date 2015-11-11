#pragma once
#include "Vec2I.h"
#include "FBCommonHeaders\String.h"
namespace fastbird
{
	class Vec2
	{
	public:
		Real x, y;

		static const Vec2 UNIT_X;
		static const Vec2 UNIT_Y;
		static const Vec2 ZERO;
		static const Vec2 ONE;		

		//-------------------------------------------------------------------
		Vec2();
		Vec2(Real _x, Real _y);
		explicit Vec2(const Vec2I& v);
		Vec2(const std::pair<Real, Real>& coordinates);

		//-------------------------------------------------------------------
		Vec2 operator+ (Real s) const;
		Vec2& operator+= (Real s);
		Vec2 operator+ (const Vec2& v) const;
		Vec2& operator+= (const Vec2& s);

		Vec2 operator- (Real s) const;
		Vec2 operator- () const;
		Vec2 operator- (const Vec2& v) const;
		Vec2& operator-= (Real s);

		Vec2 operator* (Real s) const;
		Vec2 operator* (const Vec2& v) const;
		Vec2& operator*= (Real s);
		Vec2& operator*= (const Vec2& v);

		Vec2 operator/ (Real s) const;
		Vec2 operator/ (const Vec2& v) const;
		Vec2& operator/= (const Vec2& other);
		Vec2& operator/= (Real s);				
		
		bool operator== (const Vec2& other) const;
		bool operator!= (const Vec2& other) const;
		Real operator[] (const size_t i) const;
		Real& operator[] (const size_t i);
		bool operator<(const Vec2& other) const;

		//-------------------------------------------------------------------
		Real Normalize();
		Vec2 NormalizeCopy() const;
		void SafeNormalize();
		Real Length() const;
		Real LengthSQ() const;
		Real DistanceTo(const Vec2& other) const;
		Real DistanceToSQ(const Vec2& other) const;
		Real Cross(const Vec2& right);
		
		int MaxAxis() const;
		
		
	};

	Vec2 operator / (const Vec2I& left, const Vec2& right);
	Vec2 operator * (const Vec2I& left, const Vec2& right);
	Vec2 operator * (const Vec2& left, const Vec2I& right);
	Vec2 operator*(const Vec2I& left, Real right);
}

//// luawapper util
//template<>
//struct luaU_Impl<fastbird::Vec2>
//{
//	static fastbird::Vec2 luaU_check(lua_State* L, int index)
//	{
//		luaL_checktype(L, index, LUA_TTABLE);
//		fastbird::Vec2 ret;
//		lua_rawgeti(L, index, 1);
//		ret.x = (Real)luaL_checknumber(L, -1);
//		lua_pop(L, 1);
//		lua_rawgeti(L, index, 2);
//		ret.y = (Real)luaL_checknumber(L, -1);
//		lua_pop(L, 1);
//		return ret;
//	}
//
//	static fastbird::Vec2 luaU_to(lua_State* L, int index)
//	{
//		fastbird::Vec2 ret;
//		lua_rawgeti(L, index, 1);
//		ret.x = (Real)lua_tonumber(L, -1);
//		lua_pop(L, 1);
//		lua_rawgeti(L, index, 2);
//		ret.y = (Real)lua_tonumber(L, -1);
//		lua_pop(L, 1);
//		return ret;
//	}
//
//	static void luaU_push(lua_State* L, const fastbird::Vec2& val)
//	{
//		lua_createtable(L, 2, 0);
//		lua_pushnumber(L, val.x);
//		lua_rawseti(L, -2, 1);
//		lua_pushnumber(L, val.y);
//		lua_rawseti(L, -2, 2);
//	}
//};