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