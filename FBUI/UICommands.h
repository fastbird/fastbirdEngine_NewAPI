#pragma once
#include "FBConsole/ICVarObserver.h"
namespace fastbird
{
	DECLARE_SMART_PTR(UICommands);
	class UICommands : public ICVarObserver
	{
		DECLARE_PIMPL_NON_COPYABLE(UICommands);
		UICommands();
		~UICommands();

	public:

		static UICommandsPtr Create();

		virtual bool OnChangeCVar(CVar* pCVar);		
	};
}