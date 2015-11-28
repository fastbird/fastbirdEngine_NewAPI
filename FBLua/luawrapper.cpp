#include "stdafx.h"
#include "luawrapper.hpp"
#include "LuaUtils.h"
using namespace fastbird;

void luaW_registerfuncs(lua_State* L, const luaL_Reg defaulttable[], const luaL_Reg table[])
{
	// ... T
#if LUA_VERSION_NUM == 502
	if (defaulttable)
		luaL_setfuncs(L, defaulttable, 0); // ... T
	if (table)
		luaL_setfuncs(L, table, 0); // ... T
#else
	if (defaulttable)
		luaL_register(L, NULL, defaulttable); // ... T
	if (table)
		luaL_register(L, NULL, table); // ... T
#endif
}

void luaW_initialize(lua_State* L)
{
	using namespace fastbird;
	// Ensure that the LuaWrapper table is set up
	LuaUtils::getfield(L, LUA_REGISTRYINDEX, LUAW_WRAPPER_KEY); // ... LuaWrapper
	if (LuaUtils::isnil(L, -1))
	{
		LuaUtils::newtable(L); // ... nil {}
		LuaUtils::pushvalue(L, -1); // ... nil {} {}
		LuaUtils::setfield(L, LUA_REGISTRYINDEX, LUAW_WRAPPER_KEY); // ... nil LuaWrapper

		// Create a storage table        
		LuaUtils::newtable(L); // ... LuaWrapper nil {}
		LuaUtils::setfield(L, -2, LUAW_STORAGE_KEY); // ... nil LuaWrapper

		// Create a holds table
		LuaUtils::newtable(L); // ... LuaWrapper {}
		LuaUtils::setfield(L, -2, LUAW_HOLDS_KEY); // ... nil LuaWrapper

		// Create a cache table, with weak values so that the userdata will not
		// be ref counted
		LuaUtils::newtable(L); // ... nil LuaWrapper {}
		LuaUtils::setfield(L, -2, LUAW_CACHE_KEY); // ... nil LuaWrapper

		LuaUtils::newtable(L); // ... nil LuaWrapper {}
		LuaUtils::pushstring(L, "v"); // ... nil LuaWrapper {} "v"
		LuaUtils::setfield(L, -2, "__mode"); // ... nil LuaWrapper {}
		LuaUtils::setfield(L, -2, LUAW_CACHE_METATABLE_KEY); // ... nil LuaWrapper

		LuaUtils::pop(L, 1); // ... nil
	}
	LuaUtils::pop(L, 1); // ...
}