#include "stdafx.h"
#include "LuaUtils.h"
#include "LuaObject.h"

namespace fastbird
{
	const char* GetCWD() {
		static char buf[MAX_PATH];
#if defined(_PLATFORM_WINDOWS_)
		GetCurrentDirectory(MAX_PATH, buf);
#else
		assert(0 && "Not implemented");
#endif
		return buf;
	}

	void LuaUtils::CallLuaFunction(lua_State* L, const char* func, const char* sig, ...)
	{
		va_list vl;
		int narg, nres;

		va_start(vl, sig);
		lua_getglobal(L, func);

		// push args
		for (narg = 0; *sig; narg++)
		{
			luaL_checkstack(L, 1, "too many arguments");
			switch (*sig++)
			{
			case 'd':
				lua_pushnumber(L, va_arg(vl, double));
				break;
			case 'f':
				lua_pushnumber(L, (lua_Number)va_arg(vl, float));
				break;
			case 'i':
				lua_pushinteger(L, va_arg(vl, int));
				break;
			case 'u':
				lua_pushunsigned(L, va_arg(vl, unsigned));
				break;
			case 's':
				lua_pushstring(L, va_arg(vl, char*));
				break;

			case '>':
				goto endargs;

			default:
				Logger::Log(FB_ERROR_LOG_ARG, FormatString("Invalid arg(%c)", *(sig - 1)).c_str());
			}
		}
	endargs:

		nres = strlen(sig);

		if (lua_pcall(L, narg, nres, 0) != 0)
		{
			Logger::Log(FB_ERROR_LOG_ARG, FormatString("Error calling '%s':'%s'", func, lua_tostring(L, -1)).c_str());			
			return;
		}

		// retrieve results
		nres = -nres;
		while (*sig)
		{
			switch (*sig++)
			{
			case 'd':
			{
						int isnum;
						double n = lua_tonumberx(L, nres, &isnum);
						if (!isnum)
							Logger::Log(FB_ERROR_LOG_ARG, "Wrong result type.");
						*va_arg(vl, double*) = n;
						break;
			}
			case 'f':
			{
				int isnum;
				float n = (float)lua_tonumberx(L, nres, &isnum);
				if (!isnum)
					Logger::Log(FB_ERROR_LOG_ARG, "Wrong result type.");
				*va_arg(vl, float*) = n;				
				break;
			}
			case 'i':
			{
						int isnum;
						int n = lua_tointegerx(L, nres, &isnum);
						if (!isnum)
							Logger::Log(FB_ERROR_LOG_ARG, "Wrong result type.");
						*va_arg(vl, int*) = n;
						break;
			}

			case 's':
			{
						const char* s = lua_tostring(L, nres);
						if (s == NULL)
							Logger::Log(FB_ERROR_LOG_ARG, "Wrong result type.");
						*va_arg(vl, const char**) = s;
						break;
			}

			case'b':
			{
					   bool b = lua_toboolean(L, nres)!=0;
					   *va_arg(vl, bool*) = b;
					   break;
			}

			case 'u':
			{
						unsigned u = lua_tounsigned(L, nres);
						*va_arg(vl, unsigned*) = u;
						break;
			}

			default:
				Logger::Log(FB_ERROR_LOG_ARG, FormatString("Invalid option(%c)", *(sig - 1)).c_str());				
			}
			nres++;
		}

		va_end(vl);
	}

	bool LuaUtils::CheckLuaGlobalExist(lua_State* L, const char* name)
	{
		LUA_STACK_WATCHER watcher(L, "bool CheckLuaGlobalExist(lua_State* L, const char* name)");
		lua_getglobal(L, name);
		bool exist = !lua_isnil(L, -1);
		lua_pop(L, 1);
		return exist;
	}

	void LuaUtils::PrintLuaErrorString(lua_State* L, const char* luaString)
	{
		std::regex e(":(\\d+):");
		char buf[1024];
		sprintf_s(buf, "\n%s/%s %s", GetCWD(), std::regex_replace(luaString, e, "($1):").c_str(), "Error");
		Logger::Log(FB_ERROR_LOG_ARG, buf);		
		PrintLuaDebugInfo(L, 0);
	}

	void LuaUtils::PrintLuaDebugInfo(lua_State* L, int level)
	{
		luaL_traceback(L, L, 0, 0);
		Logger::Log(FB_ERROR_LOG_ARG, lua_tostring(L, -1));
		lua_pop(L, 1);

	/*	lua_Debug ar;
		if (lua_getstack(L, level, &ar))
		{
			int i = 1;
			while (true)
			{
				const char* name = lua_getlocal(L, &ar, i++);
				if (!name)
					break;
				Error("[local variable] name = %s, value = %s", name, GetLuaValueAsString(L, -1).c_str());
				lua_pop(L, 1);
			}
		}*/
	}

	std::string LuaUtils::GetLuaValueAsString(lua_State* L, int idx)
	{
		switch (lua_type(L, idx))
		{
		case LUA_TBOOLEAN:
		{
			bool val = lua_toboolean(L, idx) != 0;
			if (val)
				return "true";
			else
				return "false";
					
			break;
		}
		case LUA_TFUNCTION:
		case LUA_TLIGHTUSERDATA:
		case LUA_TNIL:
		case LUA_TNONE:
		case LUA_TTHREAD:
		case LUA_TUSERDATA:
		{
						  return lua_typename(L, idx);
							  break;
		}

		case LUA_TNUMBER:
		case LUA_TSTRING:
		{
							return lua_tostring(L, idx);
							break;
		}
		case LUA_TTABLE:
		{
						   LuaObject t(L, idx);
						   auto it  = t.GetTableIterator();
						   LuaTableIterator::KeyValue kv;
						   std::string ret;
						   bool first = true;
						   while (it.GetNext(kv))
						   {
							   if (!first)
							   {
								   ret += "\n";
							   }
							   kv.first.PushToStack();
							   ret += GetLuaValueAsString(L, -1);
							   lua_pop(L, 1);
							   ret += " : ";
							   kv.second.PushToStack();
							   ret += GetLuaValueAsString(L, -1);
							   lua_pop(L, 1);
							   first = false;
						   }
						   return ret;
						   break;
		}
		default:
		{
				   assert(0);
		}
		
		}
		return std::string();
	}

	std::string LuaUtils::GetLuaVarAsString(lua_State* L, const char* varName, const char* luaFile)
	{
		auto luaObj = GetLuaVar(L, varName, luaFile);
		if (luaObj.IsValid())
		{
			return luaObj.GetString();
		}
		return std::string();
	}

	bool LuaUtils::GetLuaVarAsBoolean(lua_State* L, const char* varName)
	{
		LUA_STACK_CLIPPER w(L);
		lua_getglobal(L, varName);
		return lua_toboolean(L, -1)!=0;
	}

	Vec2ITuple LuaUtils::GetLuaVarAsVec2I(lua_State* L, const char* varname)
	{
		LUA_STACK_CLIPPER w(L);
		lua_getglobal(L, varname);
		assert(lua_istable(L, -1));
		Vec2ITuple ret = luaU_check<Vec2ITuple>(L, -1);

		lua_pop(L, 1);
		return ret;
	}

	float LuaUtils::GetLuaVarAsFloat(lua_State* L, const char* varName){
		LUA_STACK_CLIPPER w(L);
		lua_getglobal(L, varName);
		return (float)luaL_checknumber(L, -1);
	}

	unsigned LuaUtils::GetLuaVarAsUnsigned(lua_State* L, const char* varName){
		LUA_STACK_CLIPPER w(L);
		lua_getglobal(L, varName);
		return (unsigned)luaL_checknumber(L, -1);
	}

	void LuaUtils::SetLuaVar(lua_State* L, const char* varName, bool value)
	{
		LUA_STACK_WATCHER w(L, "void SetLuaVar(lua_State* L, const char* varName, bool value)");
		lua_pushboolean(L, value);
		lua_setglobal(L, varName);
	}
}

//---------------------------------------------------------------------------
// Tuple Helper
//---------------------------------------------------------------------------
// PushNumber
template <class T>
void PushNumbersElem(lua_State* L, int& n, const T& value){
	lua_pushnumber(L, value);
	lua_rawseti(L, -2, n++);
}

template <>
void PushNumbersElem(lua_State* L, int& n, const bool& value){
	lua_pushboolean(L, value);
	lua_rawseti(L, -2, n++);
}

template<class Tuple, std::size_t N>
struct TupleIteratorPush {
	static void iterate(lua_State* L, int& n, const Tuple& t)
	{
		TupleIteratorPush<Tuple, N - 1>::iterate(L, n, t);
		PushNumbersElem(L, n, std::get<N - 1>(t));
	}
};
template<class Tuple>
struct TupleIteratorPush<Tuple, 1>{
	static void iterate(lua_State* L, int& n, const Tuple& t)
	{
		PushNumbersElem(L, n, std::get<0>(t));
	}
};

template<class... Args>
void PushNumbers(lua_State* L, int& n, const std::tuple<Args...>& t)
{
	TupleIteratorPush<decltype(t), sizeof...(Args)>::iterate(L, n, t);
}

//---------------------------------------------------------------------------
// PullNumber
template <class T>
void PullNumbersElem(lua_State* L, int index, int& n, T& value){
	lua_rawgeti(L, index, n++);
	value = lua_tonumber(L, -1);
	lua_pop(L, 1);
}
template <>
void PullNumbersElem(lua_State* L, int index, int& n, bool& value){
	lua_rawgeti(L, index, n++);
	value = lua_toboolean(L, -1) != 0;
	lua_pop(L, 1);
}

template<class Tuple, std::size_t N>
struct TupleIteratorPull {
	static void iterate(lua_State* L, int index, int& n, Tuple& t)
	{
		TupleIteratorPull<Tuple, N - 1>::iterate(L, index, n, t);
		PullNumbersElem(L, index, n, std::get<N - 1>(t));
	}
};
template<class Tuple>
struct TupleIteratorPull<Tuple, 1>{
	static void iterate(lua_State* L, int index, int& n, Tuple& t)
	{
		PullNumbersElem(L, index, n, std::get<0>(t));
	}
};

template<class... Args>
void PullNumbers(lua_State* L, int index, int& n, std::tuple<Args...>& t)
{
	TupleIteratorPull<decltype(t), sizeof...(Args)>::iterate(L, index, n, t);
}


//---------------------------------------------------------------------------
// Vec2ITuple
//---------------------------------------------------------------------------
fastbird::Vec2ITuple luaU_Impl< fastbird::Vec2ITuple >::luaU_check(lua_State* L, int index){
	luaL_checktype(L, index, LUA_TTABLE);
	return luaU_to(L, index);
}

fastbird::Vec2ITuple luaU_Impl< fastbird::Vec2ITuple >::luaU_to(lua_State* L, int index)
{
	fastbird::Vec2ITuple ret;
	lua_rawgeti(L, index, 1);
	std::get<0>(ret) = lua_tointeger(L, -1);
	lua_pop(L, 1);
	lua_rawgeti(L, index, 2);
	std::get<1>(ret) = lua_tointeger(L, -1);
	lua_pop(L, 1);
	return ret;
}

void luaU_Impl< fastbird::Vec2ITuple >::luaU_push(lua_State* L, const fastbird::Vec2ITuple& val)
{
	lua_createtable(L, 2, 0);
	lua_pushinteger(L, std::get<0>(val));
	lua_rawseti(L, -2, 1);
	lua_pushinteger(L, std::get<1>(val));
	lua_rawseti(L, -2, 2);
}

//---------------------------------------------------------------------------
// Vec2Tuple
//---------------------------------------------------------------------------
fastbird::Vec2Tuple luaU_Impl< fastbird::Vec2Tuple >::luaU_check(lua_State* L, int index)
{
	luaL_checktype(L, index, LUA_TTABLE);
	return luaU_to(L, index);
}

fastbird::Vec2Tuple luaU_Impl< fastbird::Vec2Tuple >::luaU_to(lua_State* L, int index)
{
	fastbird::Vec2Tuple ret;
	lua_rawgeti(L, index, 1);
	std::get<0>(ret) = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_rawgeti(L, index, 2);
	std::get<1>(ret) = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	return ret;
}

void luaU_Impl< fastbird::Vec2Tuple >::luaU_push(lua_State* L, const fastbird::Vec2Tuple& val)
{
	lua_createtable(L, 2, 0);
	lua_pushnumber(L, std::get<0>(val));
	lua_rawseti(L, -2, 1);
	lua_pushnumber(L, std::get<1>(val));
	lua_rawseti(L, -2, 2);
}

//---------------------------------------------------------------------------
// Vec3ITuple
//---------------------------------------------------------------------------
fastbird::Vec3ITuple luaU_Impl< fastbird::Vec3ITuple >::luaU_check(lua_State* L, int index)
{
	fastbird::LUA_STACK_WATCHER watcher(L, "fastbird::Vec3I luaU_check(lua_State* L, int index)");
	luaL_checktype(L, index, LUA_TTABLE);
	return luaU_to(L, index);
}

fastbird::Vec3ITuple luaU_Impl< fastbird::Vec3ITuple >::luaU_to(lua_State* L, int index)
{
	fastbird::LUA_STACK_WATCHER watcher(L, "fastbird::Vec3I luaU_to(lua_State* L, int index)");
	fastbird::Vec3ITuple ret;
	lua_rawgeti(L, index, 1);
	std::get<0>(ret) = lua_tointeger(L, -1);
	lua_pop(L, 1);
	lua_rawgeti(L, index, 2);
	std::get<1>(ret) = lua_tointeger(L, -1);
	lua_pop(L, 1);
	lua_rawgeti(L, index, 3);
	std::get<2>(ret) = lua_tointeger(L, -1);
	lua_pop(L, 1);
	return ret;
}

void luaU_Impl< fastbird::Vec3ITuple >::luaU_push(lua_State* L, const fastbird::Vec3ITuple& val)
{
	lua_createtable(L, 3, 0);
	lua_pushinteger(L, std::get<0>(val));
	lua_rawseti(L, -2, 1);
	lua_pushinteger(L, std::get<1>(val));
	lua_rawseti(L, -2, 2);
	lua_pushinteger(L, std::get<2>(val));
	lua_rawseti(L, -2, 3);
}

//---------------------------------------------------------------------------
// Vec3Tuple
//---------------------------------------------------------------------------
fastbird::Vec3Tuple luaU_Impl< fastbird::Vec3Tuple >::luaU_check(lua_State* L, int index)
{
	fastbird::LUA_STACK_WATCHER watcher(L, "fastbird::Vec3Tuple luaU_check(lua_State* L, int index)");
	luaL_checktype(L, index, LUA_TTABLE);
	return luaU_to(L, index);
}

fastbird::Vec3Tuple luaU_Impl< fastbird::Vec3Tuple >::luaU_to(lua_State* L, int index)
{
	fastbird::LUA_STACK_WATCHER watcher(L, "fastbird::Vec3Tuple luaU_to(lua_State* L, int index)");
	fastbird::Vec3Tuple ret;
	lua_rawgeti(L, index, 1);
	std::get<0>(ret) = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_rawgeti(L, index, 2);
	std::get<1>(ret) = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_rawgeti(L, index, 3);
	std::get<2>(ret) = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	return ret;
}

void luaU_Impl< fastbird::Vec3Tuple >::luaU_push(lua_State* L, const fastbird::Vec3Tuple& val)
{
	lua_createtable(L, 3, 0);
	lua_pushnumber(L, std::get<0>(val));
	lua_rawseti(L, -2, 1);
	lua_pushnumber(L, std::get<1>(val));
	lua_rawseti(L, -2, 2);
	lua_pushnumber(L, std::get<2>(val));
	lua_rawseti(L, -2, 3);
}

//---------------------------------------------------------------------------
// Vec4Tuple
//---------------------------------------------------------------------------
fastbird::Vec4Tuple luaU_Impl< fastbird::Vec4Tuple >::luaU_check(lua_State* L, int index)
{
	fastbird::LUA_STACK_WATCHER watcher(L, "fastbird::Vec4Tuple luaU_check(lua_State* L, int index)");
	luaL_checktype(L, index, LUA_TTABLE);
	return luaU_to(L, index);
}

fastbird::Vec4Tuple luaU_Impl< fastbird::Vec4Tuple >::luaU_to(lua_State* L, int index)
{
	fastbird::LUA_STACK_WATCHER watcher(L, "fastbird::Vec4Tuple luaU_to(lua_State* L, int index)");
	fastbird::Vec4Tuple ret;
	lua_rawgeti(L, index, 1);
	std::get<0>(ret) = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_rawgeti(L, index, 2);
	std::get<1>(ret) = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_rawgeti(L, index, 3);
	std::get<2>(ret) = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_rawgeti(L, index, 4);
	std::get<3>(ret) = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	return ret;
}

void luaU_Impl< fastbird::Vec4Tuple >::luaU_push(lua_State* L, const fastbird::Vec4Tuple& val)
{
	lua_createtable(L, 3, 0);
	lua_pushnumber(L, std::get<0>(val));
	lua_rawseti(L, -2, 1);
	lua_pushnumber(L, std::get<1>(val));
	lua_rawseti(L, -2, 2);
	lua_pushnumber(L, std::get<2>(val));
	lua_rawseti(L, -2, 3);
	lua_pushnumber(L, std::get<3>(val));
	lua_rawseti(L, -2, 4);
}

//---------------------------------------------------------------------------
// QuatTuple
//---------------------------------------------------------------------------
fastbird::QuatTuple luaU_Impl< fastbird::QuatTuple>::luaU_check(lua_State* L, int index)
{
	fastbird::LUA_STACK_WATCHER watcher(L, " fastbird::QuatTuple luaU_check(lua_State* L, int index)");
	luaL_checktype(L, index, LUA_TTABLE);
	return luaU_to(L, index);
}

fastbird::QuatTuple luaU_Impl< fastbird::QuatTuple>::luaU_to(lua_State* L, int index)
{
	fastbird::LUA_STACK_WATCHER watcher(L, " fastbird::QuatTuple luaU_to(lua_State* L, int index)");
	fastbird::QuatTuple ret;
	int n = 1;
	PullNumbers(L, index, n, ret.value);	
	return ret;
}

void luaU_Impl< fastbird::QuatTuple>::luaU_push(lua_State* L, const fastbird::QuatTuple& val)
{
	lua_createtable(L, 0, 3);
	int n = 1;
	PushNumbers(L, n, val.value);
}

//---------------------------------------------------------------------------
// TransformationTuple
//---------------------------------------------------------------------------
fastbird::TransformationTuple luaU_Impl< fastbird::TransformationTuple>::luaU_check(lua_State* L, int index)
{
	fastbird::LUA_STACK_WATCHER watcher(L, "fastbird::TransformationTuple luaU_check(lua_State* L, int index)");
	luaL_checktype(L, index, LUA_TTABLE);
	return luaU_to(L, index);
}

fastbird::TransformationTuple luaU_Impl< fastbird::TransformationTuple>::luaU_to(lua_State* L, int index)
{
	fastbird::LUA_STACK_WATCHER watcher(L, "fastbird::TransformationTuple luaU_to(lua_State* L, int index)");
	int n = 1;
	fastbird::TransformationTuple ret;
	PullNumbers(L,index,  n, ret);
	return ret;
}

void luaU_Impl< fastbird::TransformationTuple>::luaU_push(lua_State* L, const fastbird::TransformationTuple& val)
{
	lua_createtable(L, 22, 0);

	int n = 1;
	PushNumbers(L, n, val);
}