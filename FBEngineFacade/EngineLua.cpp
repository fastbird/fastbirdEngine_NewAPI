#include "stdafx.h"
#include "FBLua/LuaObject.h"
#include "FBLuaMathLib/FBLuaMath.h"
#include "FBRenderer/Renderer.h"
#include "FBSceneManager/SpatialObject.h"
#include "FBEngineFacade/EngineFacade.h"
#include "FBEngineFacade/MeshFacade.h"
namespace fastbird{
	int GetResolution(lua_State* L);
	int GetFrameCounter(lua_State* L);
	int PrintSpatialObject(lua_State* L);
	int InvalidateMouseKeyboard(lua_State* L);

	int IsKeyPressed(lua_State* L);
	int IsKeyDown(lua_State* L);
	int IsLButtonClicked(lua_State* L);
	int IsRButtonClicked(lua_State* L);
	int IsLButtonDown(lua_State* L);

	int GenGGX(lua_State* L);
	int LoadMesh(lua_State* L);
	int FBConsole(lua_State* L);

	void InitEngineLua(){
		LuaUtils::LoadConfig("configEngine.lua");
		LuaUtils::DoFile("EssentialEngineData/scripts/ConstKeys.lua");
		auto L = LuaUtils::GetLuaState();
		LUA_SETCFUNCTION(L, FBConsole);
		LUA_SETCFUNCTION(L, LoadMesh);
		LUA_SETCFUNCTION(L, GenGGX);
		LUA_SETCFUNCTION(L, IsKeyPressed);
		LUA_SETCFUNCTION(L, IsKeyDown);
		LUA_SETCFUNCTION(L, IsLButtonClicked);
		LUA_SETCFUNCTION(L, IsRButtonClicked);
		LUA_SETCFUNCTION(L, IsLButtonDown);
		LUA_SETCFUNCTION(L, PrintSpatialObject);
		LUA_SETCFUNCTION(L, GetResolution);
		LUA_SETCFUNCTION(L, GetFrameCounter);
		LUA_SETCFUNCTION(L, InvalidateMouseKeyboard);
	}

	int GetResolution(lua_State* L)
	{
		const auto& size = Renderer::GetInstance().GetMainRenderTargetSize();
		luaU_push<Vec2I>(L, size);
		return 1;
	}

	int GetFrameCounter(lua_State* L)
	{
		LuaUtils::pushunsigned(L, gpTimer->GetFrame());
		return 1;
	}

	int PrintSpatialObject(lua_State* L)
	{
		auto scene = EngineFacade::GetInstance().GetMainScene();
		if (scene)
		{
			Logger::Log(FB_DEFAULT_LOG_ARG, "PrintSpatialObject() is under construction.");
			//scene->PrintSpatialObject();
		}
		return 0;
	}

	int InvalidateMouseKeyboard(lua_State* L)
	{
		auto injector = EngineFacade::GetInstance().GetInputInjector();
		if (injector)
		{
			injector->Invalidate(InputDevice::Keyboard);
			injector->ClearBuffer();
			injector->InvalidateClickTime();
			injector->ClearWheel();			
		}
		return 0;
	}

	int IsKeyPressed(lua_State* L)
	{
		unsigned short keyCode = LuaUtils::checkunsigned(L, 1);
		auto injector = EngineFacade::GetInstance().GetInputInjector();
		bool pressed = injector->IsKeyPressed(keyCode);
		LuaUtils::pushboolean(L, pressed);
		return 1;
	}
	int IsKeyDown(lua_State* L)
	{
		unsigned short keyCode = LuaUtils::checkunsigned(L, 1);
		auto injector = EngineFacade::GetInstance().GetInputInjector();
		bool down = injector->IsKeyDown(keyCode);
		LuaUtils::pushboolean(L, down);
		return 1;
	}
	int IsLButtonClicked(lua_State* L)
	{
		auto injector = EngineFacade::GetInstance().GetInputInjector();
		LuaUtils::pushboolean(L, injector->IsLButtonClicked());
		return 1;
	}
	int IsRButtonClicked(lua_State* L)
	{
		auto injector = EngineFacade::GetInstance().GetInputInjector();
		LuaUtils::pushboolean(L, injector->IsRButtonClicked());
		return 1;
	}
	int IsLButtonDown(lua_State* L)
	{
		auto injector = EngineFacade::GetInstance().GetInputInjector();
		LuaUtils::pushboolean(L, injector->IsLButtonDown());
		return 1;
	}

	int GenGGX(lua_State* L){
		Logger::Log(FB_ERROR_LOG_ARG, "GenGGX() lua function is Deprecated.");		
		return 0;
	}

	int LoadMesh(lua_State* L){
		auto meshPath = LuaUtils::checkstring(L, 1);
		if (strlen(meshPath) != 0){
			MeshFacadePtr mesh = MeshFacade::Create()->LoadMeshObject(meshPath);
			if (mesh){
				mesh->AttachToScene();
				EngineFacade::GetInstance().SetMainCameraTarget(mesh->GetSpatialObject());
				EngineFacade::GetInstance().EnableCameraInput(true);
			}
			else{
				EngineFacade::GetInstance().EnableCameraInput(false);
			}
		}
		else{
			EngineFacade::GetInstance().EnableCameraInput(false);
		}
		return 0;
	}

	int FBConsole(lua_State* L){
		auto str = LuaUtils::checkstring(L, 1);
		EngineFacade::GetInstance().QueueProcessConsoleCommand(str, false);
		return 0;
	}
}