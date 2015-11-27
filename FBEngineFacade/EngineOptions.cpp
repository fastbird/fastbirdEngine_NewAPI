#include "stdafx.h"
#include "EngineOptions.h"
#include "FBConsole/Console.h"
using namespace fastbird;

FB_IMPLEMENT_STATIC_CREATE(EngineOptions);
EngineOptions::EngineOptions(){
	if (!Console::HasInstance()){
		Logger::Log(FB_ERROR_LOG_ARG, "Console is not initialized! Engine Options won't work.");
		return;
	}

	WheelSens = Console::GetInstance().GetRealVariable("WheelSens", 0.005f);
	FB_REGISTER_CVAR(WheelSens, WheelSens, CVAR_CATEGORY_CLIENT, "WheelSensitivity");

	MouseSens = Console::GetInstance().GetRealVariable("MouseSens", 0.03f);
	FB_REGISTER_CVAR(MouseSens, MouseSens, CVAR_CATEGORY_CLIENT, "MouseSensitivity");

	FB_REGISTER_CVAR(MoveEditParticle, 0, CVAR_CATEGORY_CLIENT, "MoveEditParticle");
	
}

EngineOptions::~EngineOptions(){

}