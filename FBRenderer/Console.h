#pragma once
#include "FBCommonHeaders/Types.h"
#include "FBCommonHeaders/Observable.h"
#include "FBInputManager/IInputConsumer.h"
#include "ConsoleDataType.h"
namespace fastbird{
	class StdOutRedirect;
	DECLARE_SMART_PTR(Console);
	class Console : public IInputConsumer, public Observable<ICVarObserver>{
		DECLARE_PIMPL_NON_COPYABLE(Console);
		Console();

	public:
		static ConsolePtr Create();
		static ConsolePtr GetInstance();
		~Console();

		void SetRenderTargetSize(const Vec2I& size);
		void RegisterCommand(ConsoleCommand* pCom);
		void UnregisterCommand(ConsoleCommand* pCom);
		void RegisterVariable(CVar* cvar);
		void UnregisterVariable(CVar* cvar);
		void AddCandidatesTo(const char* parent, const StringVector& candidates);
		void Log(const char* szFmt, ...);
		void ProcessCommand(const char* command, bool history = true);
		void QueueProcessCommand(const char* command, bool history = true);
		void ToggleOpen();
		void Update();
		void Render();
		void RegisterStdout(StdOutRedirect* p);
		void Clear();

		//---------------------------------------------------------------------------
		// IInputConsumer
		//---------------------------------------------------------------------------
		void ConsumeInput(IInputInjectorPtr injector);
	};
}