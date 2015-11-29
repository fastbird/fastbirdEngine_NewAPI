#include "FBCommonHeaders/platform.h"
#define FB_DLL_LUA __declspec(dllimport)
#include "FBLuaMath.h"
#include "FBMathLib/Math.h"

namespace fastbird{
	template<>
	Vec2 luaU_check<Vec2>(lua_State* L, int      index)
	{
		return luaU_Impl<Vec2Tuple>::luaU_check(L, index);
	}
	template<>
	Vec2 luaU_to<Vec2>(lua_State* L, int      index) {
		return luaU_Impl<Vec2Tuple>::luaU_to(L, index);
	}
	template<>
	void luaU_push<Vec2>(lua_State* L, const Vec2& value) {
		luaU_Impl<Vec2Tuple>::luaU_push(L, value);
	}
	template<> void luaU_push<Vec2>(lua_State* L, Vec2& value) {
		luaU_Impl<Vec2Tuple>::luaU_push(L, value);
	}

	template<>
	Vec2I luaU_check<Vec2I>(lua_State* L, int      index)
	{
		return luaU_Impl<Vec2ITuple>::luaU_check(L, index);
	}
	template<>
	Vec2I luaU_to(lua_State* L, int      index) {
		return luaU_Impl<Vec2ITuple>::luaU_to(L, index);
	}
	template<>
	void luaU_push(lua_State* L, const Vec2I& value) {
		luaU_Impl<Vec2ITuple>::luaU_push(L, value);
	}
	template<> void luaU_push(lua_State* L, Vec2I& value) {
		luaU_Impl<Vec2ITuple>::luaU_push(L, value);
	}


	template<>
	Vec3 luaU_check<Vec3>(lua_State* L, int      index) 
	{
		return luaU_Impl<Vec3Tuple>::luaU_check(L, index); 
	}
	template<>
	Vec3 luaU_to<Vec3>(lua_State* L, int      index) {
		return luaU_Impl<Vec3Tuple>::luaU_to(L, index);
	}
	template<>
	void luaU_push<Vec3>(lua_State* L, const Vec3& value) {
		luaU_Impl<Vec3Tuple>::luaU_push(L, value);
	}
	template<> void luaU_push<Vec3>(lua_State* L, Vec3& value) {
		luaU_Impl<Vec3Tuple>::luaU_push(L, value);
	}

	template<>
	Vec3I luaU_check<Vec3I>(lua_State* L, int      index)
	{
		return luaU_Impl<Vec3ITuple>::luaU_check(L, index);
	}
	template<>
	Vec3I luaU_to(lua_State* L, int      index) {
		return luaU_Impl<Vec3ITuple>::luaU_to(L, index);
	}
	template<>
	void luaU_push(lua_State* L, const Vec3I& value) {
		luaU_Impl<Vec3ITuple>::luaU_push(L, value);
	}
	template<> void luaU_push(lua_State* L, Vec3I& value) {
		luaU_Impl<Vec3ITuple>::luaU_push(L, value);
	}

	template<>
	Vec4 luaU_check<Vec4>(lua_State* L, int      index)
	{
		return luaU_Impl<Vec4Tuple>::luaU_check(L, index);
	}
	template<>
	Vec4 luaU_to(lua_State* L, int      index) {
		return luaU_Impl<Vec4Tuple>::luaU_to(L, index);
	}
	template<>
	void luaU_push(lua_State* L, const Vec4& value) {
		luaU_Impl<Vec4Tuple>::luaU_push(L, value);
	}
	template<> void luaU_push(lua_State* L, Vec4& value) {
		luaU_Impl<Vec4Tuple>::luaU_push(L, value);
	}

	template<>
	Quat luaU_check<Quat>(lua_State* L, int      index)
	{
		return luaU_Impl<QuatTuple>::luaU_check(L, index);
	}
	template<>
	Quat luaU_to(lua_State* L, int      index) {
		return luaU_Impl<QuatTuple>::luaU_to(L, index);
	}

	template<>
	void luaU_push(lua_State* L, const Quat& value) {
		luaU_Impl<QuatTuple>::luaU_push(L, value);
	}
	template<> void luaU_push(lua_State* L, Quat& value) {
		luaU_Impl<QuatTuple>::luaU_push(L, value);
	}

	template<>
	Transformation luaU_check<Transformation>(lua_State* L, int      index)
	{
		return luaU_Impl<TransformationTuple>::luaU_check(L, index);
	}
	template<>
	Transformation luaU_to(lua_State* L, int      index) {
		return luaU_Impl<TransformationTuple>::luaU_to(L, index);
	}
	template<>
	void luaU_push(lua_State* L, const Transformation& value) {
		luaU_Impl<TransformationTuple>::luaU_push(L, value);
	}
	template<> void luaU_push(lua_State* L, Transformation& value) {
		luaU_Impl<TransformationTuple>::luaU_push(L, value);
	}
}