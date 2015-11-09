#include "stdafx.h"
#include "ModuleHandler.h"
#include "FBDebugLib/Logger.h"
namespace fastbird{
	ModuleHandle ModuleHandler::LoadModule(LPCTSTR path){
#if defined(_PLATFORM_WINDOWS_)
		if (!path)
			return 0;

		auto dotPos = _tstrchr(path, _T('.'));
		if (!dotPos)
		{
			Logger::Log(_T("LoadModule(%s) failed. Invalid param."), path);
		}

		TCHAR buf[MAX_PATH] = { 0 };
		_tstrncpy(buf, MAX_PATH, path, dotPos - path);

#ifdef _DEBUG
		_tstrcat(buf, MAX_PATH, _T("_Debug.dll"));
#else
		_tstrcat(buf, MAX_PATH, _T("_Release.dll"));
#endif

		HMODULE module = LoadLibrary(buf);
		if (!module)
		{
			module = LoadLibrary(path);
		}
		
		if (!module) {
			Logger::Log(_T("LoadFBLibrary(%s) failed"), path);
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
			Logger::Log(_T("ModuleHandler::GetFunction : invalid param"));
			return 0;
		}
#if defined(_PLATFORM_WINDOWS_)
		return (FunctionHandle)GetProcAddress((HMODULE)module, functionName);
#else
		assert(0 && "Not implemented");
#endif
	}
}