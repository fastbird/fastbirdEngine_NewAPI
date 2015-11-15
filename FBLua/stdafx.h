#pragma warning (disable : 4251)
// If you are linking against Lua compiled in C++, define LUAW_NO_EXTERN_C
#ifndef LUAW_NO_EXTERN_C
extern "C"
{
#endif // LUAW_NO_EXTERN_C

#include "lua/lua.h"
#include "lua/lauxlib.h"

#ifndef LUAW_NO_EXTERN_C
}
#endif // LUAW_NO_EXTERN_C

#include "FBCommonHeaders/platform.h"
#if defined(_PLATFORM_WINDOWS_)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#endif
#include <regex>

#include "FBMemoryManagerLib/MemoryManager.h"