#pragma once
#include "FBCommonHeaders/platform.h"
#include "luawrapperutil.hpp"
#include "FBStringLib/StringLib.h"
#include "FBDebugLib/Logger.h"
#include "FBCommonHeaders/Types.h"

struct lua_State;
namespace fastbird
{ 
	const char* GetCWD();
}

static int traceback(lua_State *L) {
	const char *msg = lua_tostring(L, 1);
	if (msg)
		luaL_traceback(L, L, msg, 1);
	//else if (!lua_isnoneornil(L, 1)) {  /* is there an error object? */
	//	if (!luaL_callmeta(L, 1, "__tostring"))  /* try its 'tostring' metamethod */
	//		lua_pushliteral(L, "(no error message)");
	//}
	return 1;
}

#define CHECK_NUM_LUA_ARGS_FB(x) \
	int numLuaArgs = lua_gettop(L); \
if (numLuaArgs != (x)) \
{\
	assert(0); \
	return luaL_error(L, "Got %d arguments, expected %d", numLuaArgs, (x)); \
}

#define LUA_SETCFUNCTION(lua, name) lua_pushcfunction((lua), (name));\
	lua_setglobal((lua), (#name));

#define LUA_PCALL(lua, arg, ret) if(int error = lua_pcall((lua), (arg), ret, 0)) \
{\
	const char* errorString = lua_tostring(lua, -1); \
	Logger::Log(FB_ERROR_LOG_ARG, FormatString("Failed to call lua function. Error(%d)", error).c_str());\
	fastbird::PrintLuaErrorString(lua, errorString);\
	lua_pop(lua, 1); \
	assert(0);\
	return;\
}

#define LUA_PCALL_RET_FALSE(lua, arg, ret) \
int cfuncbase = lua_gettop(lua) - (arg);  /* function index */\
lua_pushcfunction(lua, traceback);  /* push traceback function */\
lua_insert(lua, cfuncbase);  /* put it under chunk and args */\
if(int error = lua_pcall((lua), (arg), ret, cfuncbase)) \
{\
	lua_remove(lua, cfuncbase);\
	const char* errorString = lua_tostring(lua, -1);\
	Logger::Log(FB_ERROR_LOG_ARG, FormatString("Failed to call lua function. Error(%d)", error).c_str());\
	fastbird::LuaUtils::PrintLuaErrorString(lua, errorString);\
	lua_pop(lua, 1); \
	assert(0); \
	return false; \
}\
else{\
	lua_remove(lua, cfuncbase);\
}

#define LUA_PCALL_NO_RET(lua, arg, ret) if(int error = lua_pcall((lua), arg, ret, 0)) \
{\
	const char* errorString = lua_tostring(lua, -1); \
	Logger::Log(FB_ERROR_LOG_ARG, FormatString("Failed to call lua function. Error(%d)", error).c_str());\
	fastbird::PrintLuaErrorString(lua, errorString);\
	lua_pop(lua, 1); \
	assert(0); \
}

#define REGISTER_ENUM_TO_LUA(endIdx, enumName) \
	inline void RegisterToLua(lua_State* L)\
{\
	lua_createtable(L, 0, (endIdx)); \
for (int i = 0; i <= (endIdx); ++i)\
{\
	lua_pushinteger(L, i); \
	lua_setfield(L, -2, ConvertToString(Enum(i))); \
}\
	lua_getglobal(L, "NoNewMethod"); \
	lua_setfield(L, -2, "__newindex"); \
	lua_pushvalue(L, -1); \
	lua_setmetatable(L, -2); \
	lua_setglobal(L, #enumName); \
	\
	lua_createtable(L, 0, (endIdx)); \
for (int i = 0; i <= (endIdx); ++i)\
{\
	lua_pushstring(L, ConvertToString(Enum(i))); \
	lua_rawseti(L, -2, i); \
}\
	lua_getglobal(L, "NoNewMethod"); \
	lua_setfield(L, -2, "__newindex"); \
	lua_pushvalue(L, -1); \
	lua_setmetatable(L, -2); \
	lua_setglobal(L, #enumName "String");\
}

#define REGISTER_CLASS_ENUM_TO_LUA(classname, enumName, endIdx) \
	static void RegisterEnumToLua(lua_State* L)\
{\
	lua_getglobal(L, #classname); \
	if (lua_isnil(L, -1))\
		{\
		assert(0); \
		lua_pop(L, 1); \
		lua_createtable(L, 1, 0); \
		lua_setglobal(L, #classname); \
		lua_getglobal(L, #classname);\
		}\
\
	lua_createtable(L, 0, endIdx); \
	for (int i = 0; i <= endIdx; ++i)\
		{\
		lua_pushinteger(L, i);\
		lua_setfield(L, -2, ConvertToString(##enumName(i))); \
		}\
	lua_getglobal(L, "NoNewMethod");\
	lua_setfield(L, -2, "__newindex");\
	lua_pushvalue(L, -1);\
	lua_setmetatable(L, -2);\
	lua_setfield(L, -2, #enumName);\
	lua_pop(L, 1);\
}

namespace fastbird
{
	struct FB_DLL_PUBLIC LUA_STACK_WATCHER
	{
		LUA_STACK_WATCHER(lua_State* L, const char* name)
			: lua(L), mName(name)
		{
			assert(name != 0);
			top = lua_gettop(L);
		}

		~LUA_STACK_WATCHER()
		{
			int now = lua_gettop(lua);
			if (top != now)
			{
				Logger::Log(FB_DEFAULT_LOG_ARG, FormatString("Stack is polluted.", mName).c_str());
			}
			assert(top == now);
		}

		lua_State* lua;
		int top;
		const char* mName;
	};

	struct FB_DLL_PUBLIC LUA_STACK_CLIPPER
	{
		LUA_STACK_CLIPPER(lua_State* L)
		{
			lua = L;
			top = lua_gettop(L);
		}

		~LUA_STACK_CLIPPER()
		{
			lua_settop(lua, top);
		}

		lua_State* lua;
		int top;
	};

	// Generic lua call
	class FB_DLL_PUBLIC LuaUtils{
	public:
		static void CallLuaFunction(lua_State* L, const char* func, const char* sig, ...);
		static bool CheckLuaGlobalExist(lua_State* L, const char* name);
		static void PrintLuaErrorString(lua_State* L, const char* luaString);
		static void PrintLuaDebugInfo(lua_State* L, int level);
		static std::string GetLuaValueAsString(lua_State* L, int stackIndex);
		static std::string GetLuaVarAsString(lua_State* L, const char* varName, const char* luaFile = 0);
		static bool GetLuaVarAsBoolean(lua_State* L, const char* varName);
		static Vec2ITuple GetLuaVarAsVec2I(lua_State* L, const char* varname);
		static float GetLuaVarAsFloat(lua_State* L, const char* varName);
		static unsigned GetLuaVarAsUnsigned(lua_State* L, const char* varName);
		static void SetLuaVar(lua_State* L, const char* varName, bool value);
		static bool ExecuteLua(lua_State* L, const char* chunk);
	};	
}

// luawapper util
// std::string
template<>
struct luaU_Impl<std::string>
{
	static std::string luaU_check(lua_State* L, int index)
	{
		return std::string(luaL_checkstring(L, index));
	}

	static std::string luaU_to(lua_State* L, int index)
	{
		return std::string(lua_tostring(L, index));
	}

	static void luaU_push(lua_State* L, const std::string& val)
	{
		lua_pushstring(L, val.c_str());
	}
};

template<>
struct luaU_Impl < fastbird::Vec2ITuple >
{
	static fastbird::Vec2ITuple luaU_check(lua_State* L, int index);
	static fastbird::Vec2ITuple luaU_to(lua_State* L, int index);
	static void luaU_push(lua_State* L, const fastbird::Vec2ITuple& val);
};

template<>
struct luaU_Impl < fastbird::Vec2Tuple >
{
	static fastbird::Vec2Tuple luaU_check(lua_State* L, int index);
	static fastbird::Vec2Tuple luaU_to(lua_State* L, int index);
	static void luaU_push(lua_State* L, const fastbird::Vec2Tuple& val);
};

template<>
struct luaU_Impl < fastbird::Vec3ITuple >
{
	static fastbird::Vec3ITuple luaU_check(lua_State* L, int index);
	static fastbird::Vec3ITuple luaU_to(lua_State* L, int index);
	static void luaU_push(lua_State* L, const fastbird::Vec3ITuple& val);
};

template<>
struct luaU_Impl < fastbird::Vec3Tuple >
{
	static fastbird::Vec3Tuple luaU_check(lua_State* L, int index);
	static fastbird::Vec3Tuple luaU_to(lua_State* L, int index);
	static void luaU_push(lua_State* L, const fastbird::Vec3Tuple& val);
};

template<>
struct luaU_Impl < fastbird::Vec4Tuple >
{
	static fastbird::Vec4Tuple luaU_check(lua_State* L, int index);
	static fastbird::Vec4Tuple luaU_to(lua_State* L, int index);
	static void luaU_push(lua_State* L, const fastbird::Vec4Tuple& val);
};

template<>
struct luaU_Impl < fastbird::QuatTuple >
{
	static fastbird::QuatTuple luaU_check(lua_State* L, int index);
	static fastbird::QuatTuple luaU_to(lua_State* L, int index);
	static void luaU_push(lua_State* L, const fastbird::QuatTuple& val);
};

template<>
struct luaU_Impl < fastbird::TransformationTuple >
{
	static fastbird::TransformationTuple luaU_check(lua_State* L, int index);
	static fastbird::TransformationTuple luaU_to(lua_State* L, int index);
	static void luaU_push(lua_State* L, const fastbird::TransformationTuple& val);
};