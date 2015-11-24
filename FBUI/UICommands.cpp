#include "StdAfx.h"
#include "UICommands.h"
#include "UIManager.h"
#include "FBConsole/ConsoleDataType.h"
#include "FBConsole/Console.h"
using namespace fastbird;

static void StartUIEditor(StringVector& arg);
static void KillUIEditor(StringVector& arg);

static ConsoleCommand ccStartUIEditor("StartUIEditor", StartUIEditor, "Start ui editor");
static ConsoleCommand ccKillUIEditor("KillUIEditor", KillUIEditor, "Kill ui editor");

class UICommands::Impl{
public:
	UICommandsWeakPtr mSelfPtr;
	std::vector<CVar*> mCVars;
	std::vector<ConsoleCommand*> mCommands;

	//---------------------------------------------------------------------------
	Impl(){
		FB_REGISTER_CC(&ccStartUIEditor);
		FB_REGISTER_CC(&ccKillUIEditor);
	}
	~Impl(){
		if (Console::HasInstance()){
			Console::GetInstance().RemoveObserver(ICVarObserver::Default, mSelfPtr.lock());
			for (auto p : mCVars)
			{
				Console::GetInstance().UnregisterVariable(p);
				delete p;
			}
			for (const auto& p : mCommands)
			{
				Console::GetInstance().UnregisterCommand(p);
			}
		}
	}
};

UICommandsPtr UICommands::Create(){
	UICommandsPtr p(new UICommands, [](UICommands* obj){ delete obj; });
	p->mImpl->mSelfPtr = p;
	return p;
}

UICommands::UICommands()
	: mImpl(new Impl)
{
	
}

UICommands::~UICommands()
{
	
}

bool UICommands::OnChangeCVar(CVar* pCVar)
{
	return false;
}

//typedef int(__cdecl *StartProc)(GlobalEnv* pEnv);
static bool uiEditorInitialized = false;
void StartUIEditor(StringVector& arg)
{
	/*if (uiEditorInitialized)
	{
		Log("Alreay started!");
		return;
	}
	auto moduleHandle = gFBEnv->pUIManager->GetUIEditorModuleHandle();
	if (!moduleHandle)
	{
		moduleHandle = fastbird::LoadFBLibrary("FBUIEditor.dll");
		gFBEnv->pUIManager->SetUIEditorModuleHandle(moduleHandle);
	}
	if (moduleHandle)
	{		
		StartProc startFunc;
		startFunc = (StartProc)GetProcAddress(moduleHandle, "StartUIEditor");
		if (startFunc)
		{
			gFBEnv->pUIManager->SetUIEditorModuleHandle(moduleHandle);
			startFunc(gFBEnv);
			uiEditorInitialized = true;
			LuaLock L;
			lua_pushboolean(L, 1);
			lua_setglobal(L, "gThreatHold");
		}
	}*/
}

typedef void(__cdecl *FinalizeProc)();
void KillUIEditor(StringVector& arg)
{
	//if (!uiEditorInitialized)
	//	return;

	//auto moduleHandle = gFBEnv->pUIManager->GetUIEditorModuleHandle();
	//if (moduleHandle)
	//{
	//	FinalizeProc finalizeFunc;
	//	finalizeFunc = (FinalizeProc)GetProcAddress(moduleHandle, "KillUIEditor");
	//	if (finalizeFunc)
	//		finalizeFunc();
	//	LuaLock L;
	//	lua_pushboolean(L, 0);
	//	lua_setglobal(L, "gThreatHold");

	//	//FreeLibrary(gFBEnv->pUIManager->GetUIEditorModuleHandle());
	//	//gFBEnv->pUIManager->SetUIEditorModuleHandle(0);
	//}
	//uiEditorInitialized = false;
}