/*
 -----------------------------------------------------------------------------
 This source file is part of fastbird engine
 For the latest info, see http://www.jungwan.net/
 
 Copyright (c) 2013-2015 Jungwan Byun
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
*/

#pragma once
#include "FBCommonHeaders/platform.h"
#include "FBStringLib/StringLib.h"
#include "FBDebugLib/Logger.h"
#include "FBCommonHeaders/Types.h"
#include <assert.h>

struct lua_State;

namespace fastbird
{ 
	const char* GetCWD();
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
	LuaUtils::PrintLuaErrorString(lua, errorString);\
	lua_pop(lua, 1); \
	assert(0);\
	return;\
}

#define LUA_PCALL_RET_FALSE(lua, arg, ret) \
int cfuncbase = lua_gettop(lua) - (arg);  /* function index */\
lua_pushcfunction(lua, LuaUtils::Traceback);  /* push traceback function */\
lua_insert(lua, cfuncbase);  /* put it under chunk and args */\
if(int error = lua_pcall((lua), (arg), ret, cfuncbase)) \
{\
	lua_remove(lua, cfuncbase);\
	const char* errorString = lua_tostring(lua, -1);\
	Logger::Log(FB_ERROR_LOG_ARG, FormatString("Failed to call lua function. Error(%d)", error).c_str());\
	LuaUtils::PrintLuaErrorString(lua, errorString);\
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
	LuaUtils::PrintLuaErrorString(lua, errorString);\
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
	struct FB_DLL_LUA LUA_STACK_WATCHER
	{
		lua_State* lua;
		int top;
		const char* mName;

		LUA_STACK_WATCHER(lua_State* L, const char* name);
		~LUA_STACK_WATCHER();
		
	};

	struct FB_DLL_LUA LUA_STACK_CLIPPER
	{
		lua_State* lua;
		int top;

		LUA_STACK_CLIPPER(lua_State* L);
		~LUA_STACK_CLIPPER();		
	};

	//---------------------------------------------------------------------------
	// Generic lua call
	//---------------------------------------------------------------------------
	class FB_DLL_LUA LuaUtils{
	public:
		/** The first lua state created by this function will be the main state.
		*/
		static lua_State* OpenLuaState();
		/** Returns the main lua state.
		*/
		static lua_State* GetLuaState();
		/** Close lus state. If L is 0, then the main state will be closed.
		*/
		static void CloseLuaState(lua_State* L);
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
		static bool DoFile(lua_State* L, const char* filepath);
		static bool DoFile(const char* filepath);
		static int Traceback(lua_State *L);

		/** Pushes the zero-terminated string pointed to by s onto the stack. 
		Lua makes (or reuses) an internal copy of the given string, so the memory 
		at str can be freed or reused immediately after the function returns.
		Returns a pointer to the internal copy of the string. If s is NULL, 
		pushes nil and returns NULL. 
		*/
		static const char* Push(lua_State* L, const char* str);
		static const char* Push(const char* str);

		static void LockLua();
		static void UnlockLua();
	};	

	struct FB_DLL_LUA LuaLock{
		LuaLock();
		~LuaLock();

		operator lua_State*() const;
	};
}