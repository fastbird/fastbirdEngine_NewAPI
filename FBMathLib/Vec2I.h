#pragma once
#include "FBCommonHeaders/Types.h"

namespace fastbird
{
	class Vec2;
	class Vec2I
	{
	public:
		int x, y;

		static const Vec2I ZERO;

		//-------------------------------------------------------------------
		Vec2I();
		Vec2I(int _x, int _y);
		Vec2I(const Vec2& v);
		Vec2I(const std::pair<int, int>& coordinates);

		//-------------------------------------------------------------------
		Vec2I operator+ (int s) const;
		Vec2I operator+ (const Vec2I& v) const;
		Vec2I& operator+=(const Vec2I& v);
		Vec2I operator- (int s) const;
		Vec2I operator- (const Vec2I& v) const;
		Vec2I& operator-=(const Vec2I& v);
		Vec2I operator* (int s) const;
		Vec2I operator* (const Vec2I& v) const;
		Vec2I operator/ (int s) const;
		Vec2I operator/ (const Vec2I& v) const;	
		bool operator== (const Vec2I& v) const;
		bool operator!=(const Vec2I& v) const;		
		bool operator<(const Vec2I& v) const;

		int Cross(const Vec2I& right);
		int Dot(const Vec2I& right);
		Real DistanceTo(const Vec2I& d) const;
		Real Length() const;
		Vec2I Scale(Real f) const;
	};
}

//// luawapper util
//template<>
//struct luaU_Impl<fastbird::Vec2I>
//{
//	static fastbird::Vec2I luaU_check(lua_State* L, int index)
//	{
//		luaL_checktype(L, index, LUA_TTABLE);
//		fastbird::Vec2I ret;
//		lua_rawgeti(L, index, 1);
//		ret.x = luaL_checkint(L, -1);
//		lua_pop(L, 1);
//		lua_rawgeti(L, index, 2);
//		ret.y = luaL_checkint(L, -1);
//		lua_pop(L, 1);
//		return ret;
//	}
//
//	static fastbird::Vec2I luaU_to(lua_State* L, int index)
//	{
//		fastbird::Vec2I ret;
//		lua_rawgeti(L, index, 1);
//		ret.x = lua_tointeger(L, -1);
//		lua_pop(L, 1);
//		lua_rawgeti(L, index, 2);
//		ret.y = lua_tointeger(L, -1);
//		lua_pop(L, 1);
//		return ret;
//	}
//
//	static void luaU_push(lua_State* L, const fastbird::Vec2I& val)
//	{
//		lua_createtable(L, 2, 0);
//		lua_pushinteger(L, val.x);
//		lua_rawseti(L, -2, 1);
//		lua_pushinteger(L, val.y);
//		lua_rawseti(L, -2, 2);
//	}
//};