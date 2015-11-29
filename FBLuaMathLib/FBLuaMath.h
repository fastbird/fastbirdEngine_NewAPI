#pragma once
#include "FBLua/LuaUtils.h"

namespace fastbird{
	class Vec2;
	template<>
	Vec2 luaU_check<Vec2>(lua_State* L, int      index);
	template<>
	Vec2 luaU_to<Vec2>(lua_State* L, int      index);
	template<>
	void luaU_push<Vec2>(lua_State* L, const Vec2& value);
	template<>
	void luaU_push<Vec2>(lua_State* L, Vec2& value);

	class Vec2I;
	template<>
	Vec2I luaU_check<Vec2I>(lua_State* L, int      index);
	template<>
	Vec2I luaU_to<Vec2I>(lua_State* L, int      index);
	template<>
	void luaU_push<Vec2I>(lua_State* L, const Vec2I& value);
	template<>
	void luaU_push<Vec2I>(lua_State* L, Vec2I& value);

	class Vec3;
	template<> 
	Vec3 luaU_check<Vec3>(lua_State* L, int      index);	
	template<>
	Vec3 luaU_to<Vec3>(lua_State* L, int      index);
	template<>
	void luaU_push<Vec3>(lua_State* L, const Vec3& value);
	template<> 
	void luaU_push<Vec3>(lua_State* L, Vec3& value);

	class Vec3I;
	template<>
	Vec3I luaU_check<Vec3I>(lua_State* L, int      index);
	template<>
	Vec3I luaU_to<Vec3I>(lua_State* L, int      index);
	template<>
	void luaU_push<Vec3I>(lua_State* L, const Vec3I& value);
	template<>
	void luaU_push<Vec3I>(lua_State* L, Vec3I& value);

	class Vec4;
	template<>
	Vec4 luaU_check<Vec4>(lua_State* L, int      index);
	template<>
	Vec4 luaU_to<Vec4>(lua_State* L, int      index);
	template<>
	void luaU_push<Vec4>(lua_State* L, const Vec4& value);
	template<>
	void luaU_push<Vec4>(lua_State* L, Vec4& value);

	class Quat;
	template<>
	Quat luaU_check<Quat>(lua_State* L, int      index);
	template<>
	Quat luaU_to<Quat>(lua_State* L, int      index);
	template<>
	void luaU_push<Quat>(lua_State* L, const Quat& value);
	template<>
	void luaU_push<Quat>(lua_State* L, Quat& value);

	class Transformation;
	template<>
	Transformation luaU_check<Transformation>(lua_State* L, int      index);
	template<>
	Transformation luaU_to<Transformation>(lua_State* L, int      index);
	template<>
	void luaU_push<Transformation>(lua_State* L, const Transformation& value);
	template<>
	void luaU_push<Transformation>(lua_State* L, Transformation& value);
}