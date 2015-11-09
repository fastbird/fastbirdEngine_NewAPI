#pragma once
#include "FBCommonHeaders/String.h"
namespace fastbird{
	/** Load a module in run-time.
	*/
	typedef intptr_t ModuleHandle;
	typedef intptr_t FunctionHandle;
	class ModuleHandler{
	public:		
		static ModuleHandle LoadModule(LPCTSTR path);
		static void UnloadModule(ModuleHandle handle);

		static FunctionHandle GetFunction(ModuleHandle module, const char* functionName);
	};
}
