#pragma once

#include "FBVec3.h"


namespace fastbird
{
	class Vec4
	{
	public:
		Real x, y, z, w;

		static const Vec4 UNIT_X;
		static const Vec4 UNIT_Y;
		static const Vec4 UNIT_Z;
		static const Vec4 ZERO;

		//-------------------------------------------------------------------------
		Vec4();
		Vec4(Real _x, Real _y, Real _z, Real _w);
		Vec4(const Vec3& xyz, Real _w);
		explicit Vec4(const Vec3& xyz);
		explicit Vec4(const char* s);

		//-------------------------------------------------------------------------
		Vec4 operator+(const Vec4& r) const;
		Vec4 operator+(Real v);
		Vec4 operator* (Real scalar) const;
		Vec4& operator*= (Real scalar);
		Vec4& operator*= (const Vec4& r);
		Vec4 operator* (const Vec4& r) const;
		Vec4& operator/=(Real scalar);
		bool operator== (const Vec4& other) const;
		
		//-------------------------------------------------------------------------
		Real Dot(const Vec4& other) const;			
		void SetXYZ(const Vec3& v);		
		Vec3 ToVec3() const;
	};
}

//// luawapper util
//template<>
//struct luaU_Impl<fastbird::Vec4>
//{
//	static fastbird::Vec4 luaU_check(lua_State* L, int index)
//	{
//		fastbird::LUA_STACK_WATCHER watcher(L, "static fastbird::Vec4 luaU_check(lua_State* L, int index)");
//		luaL_checktype(L, index, LUA_TTABLE);
//		fastbird::Vec4 ret;
//		lua_rawgeti(L, index, 1);
//		ret.x = (Real)luaL_checknumber(L, -1);
//		lua_pop(L, 1);
//		lua_rawgeti(L, index, 2);
//		ret.y = (Real)luaL_checknumber(L, -1);
//		lua_pop(L, 1);
//		lua_rawgeti(L, index, 3);
//		ret.z = (Real)luaL_checknumber(L, -1);
//		lua_pop(L, 1);
//		lua_rawgeti(L, index, 4);
//		ret.w = (Real)luaL_checknumber(L, -1);
//		lua_pop(L, 1);
//		return ret;
//	}
//
//	static fastbird::Vec4 luaU_to(lua_State* L, int index)
//	{
//		fastbird::LUA_STACK_WATCHER watcher(L, "static fastbird::Vec4 luaU_to(lua_State* L, int index)");
//		fastbird::Vec4 ret;
//		lua_rawgeti(L, index, 1);
//		ret.x = (Real)lua_tonumber(L, -1);
//		lua_pop(L, 1);
//		lua_rawgeti(L, index, 2);
//		ret.y = (Real)lua_tonumber(L, -1);
//		lua_pop(L, 1);
//		lua_rawgeti(L, index, 3);
//		ret.z = (Real)lua_tonumber(L, -1);
//		lua_pop(L, 1);
//		lua_rawgeti(L, index, 4);
//		ret.w = (Real)lua_tonumber(L, -1);
//		lua_pop(L, 1);
//		return ret;
//	}
//
//	static void luaU_push(lua_State* L, const fastbird::Vec4& val)
//	{
//		lua_createtable(L, 3, 0);
//		lua_pushnumber(L, val.x);
//		lua_rawseti(L, -2, 1);
//		lua_pushnumber(L, val.y);
//		lua_rawseti(L, -2, 2);
//		lua_pushnumber(L, val.z);
//		lua_rawseti(L, -2, 3);
//		lua_pushnumber(L, val.w);
//		lua_rawseti(L, -2, 4);
//	}
//
//	static void luaU_push(lua_State* L, fastbird::Vec4& val)
//	{
//		lua_createtable(L, 3, 0);
//		lua_pushnumber(L, val.x);
//		lua_rawseti(L, -2, 1);
//		lua_pushnumber(L, val.y);
//		lua_rawseti(L, -2, 2);
//		lua_pushnumber(L, val.z);
//		lua_rawseti(L, -2, 3);
//		lua_pushnumber(L, val.w);
//		lua_rawseti(L, -2, 4);
//	}
//};