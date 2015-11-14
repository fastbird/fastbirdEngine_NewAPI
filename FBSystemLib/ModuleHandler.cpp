#include "stdafx.h"
#include "ModuleHandler.h"
#include "FBDebugLib/Logger.h"
namespace fastbird{
	// intended redundancy
	static std::string FormatString(const char* str, ...){
		static char buf[2048];
		va_list args;

		va_start(args, str);
		vsprintf_s(buf, str, args);
		va_end(args);

		return buf;
	}

	ModuleHandle ModuleHandler::LoadModule(const char* path){
#if defined(_PLATFORM_WINDOWS_)
		if (!path)
			return 0;

		auto dotPos = strchr(path, _T('.'));
		char buf[MAX_PATH] = { 0 };
		if (dotPos){
			strncpy_s(buf, MAX_PATH, path, dotPos - path);
		}
		else{
			strcpy_s(buf, MAX_PATH, path);
		}

#ifdef _DEBUG
		strcat_s(buf, MAX_PATH, "_Debug.dll");
#else
		strcat_s(buf, MAX_PATH, "_Release.dll");
#endif
		Logger::Log(FB_DEFAULT_LOG_ARG_NO_LINE, FormatString("Trying to load a module(%s)... ", buf).c_str());
		HMODULE module = LoadLibraryA(buf);
		if (!module) {
			Logger::Log(FB_ERROR_LOG_ARG, "\tFailed.");
			strcat_s(buf, MAX_PATH, ".dll");
			Logger::Log(FB_DEFAULT_LOG_ARG_NO_LINE, FormatString("Trying to load a module(%s)... ", buf).c_str());
			module = LoadLibraryA(buf);
		}		
		
		if (!module) {
			Logger::Log(FB_ERROR_LOG_ARG, "\tFailed.");
		}
		else{
			Logger::Log(FB_DEFAULT_LOG_ARG, "\tSucceeded.");
		}
		return (intptr_t)module;
#else
		assert(0 && "Not implemented");
#endif
	}

	void ModuleHandler::UnloadModule(ModuleHandle handle){
#if defined(_PLATFORM_WINDOWS_)
		if (handle)
			FreeLibrary((HMODULE)handle);
#else
		assert(0 && "Not implemented");
#endif
	}

	FunctionHandle ModuleHandler::GetFunction(ModuleHandle module, const char* functionName){
		if (!module){
			Logger::Log(FB_DEFAULT_LOG_ARG, "ModuleHandler::GetFunction : invalid param");
			return 0;
		}
#if defined(_PLATFORM_WINDOWS_)
		return (FunctionHandle)GetProcAddress((HMODULE)module, functionName);
#else
		assert(0 && "Not implemented");
#endif
	}
}