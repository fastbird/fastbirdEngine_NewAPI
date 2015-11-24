#pragma once
#ifndef _UIManager_header_include__
#define _UIManager_header_include__

#include "IUIManager.h"
#include "DragBox.h"
#include "RegionTestParam.h"
#include "FBFileMonitor/IFileChangeObserver.h"
#include "FBRenderer/IRendererObserver.h"
#include "FBInputManager/IInputConsumer.h"

namespace fastbird
{
	void RegisterLuaFuncs(lua_State* mL);
	void RegisterLuaEnums(lua_State* mL);

	class IWinBase;
	class IUIObject;
	class ListBox;
	class UICommands;
	class Container;

	DECLARE_SMART_PTR(UIManager);
	class FB_DLL_UI UIManager : public IFileChangeObserver,
		public IRendererObserver, public IInputConsumer
	{
		DECLARE_PIMPL_NON_COPYABLE(UIManager);
		UIManager();
		~UIManager();

	public:
		static UIManagerPtr Create();
		static bool HasInstance();
		static UIManager& GetInstance();

		typedef std::vector<IWinBase*> WinBases;
		enum POPUP_TYPE
		{
			POPUP_TYPE_YES_NO
		};

		void Shutdown();
		// IFileChangeListeners
		bool OnFileChanged(const char* file);

		// IRenderListener
		void BeforeUIRendering(HWindowId hwndId);
		void BeforeDebugHudRendered(HWindowId hwndId);
		void AfterDebugHudRendered(HWindowId hwndId);
		void OnResolutionChanged(HWindowId hwndId);

		// IUIManager Interfaces
		void Update(float elapsedTime);
		void GatherRenderList();
		bool ParseUI(const char* filepath, WinBases& windows, std::string& uiname, HWindowId hwndId = INVALID_HWND_ID, bool luaUI = false);
		bool SaveUI(const char* uiname, tinyxml2::XMLDocument& doc);
		bool AddLuaUI(const char* uiName, LuaObject& data, HWindowId hwndId = INVALID_HWND_ID);
		void DeleteLuaUI(const char* uiName, bool pending);
		bool IsLoadedUI(const char* uiName);

		IWinBase* AddWindow(int posX, int posY, int width, int height, ComponentType::Enum type, HWindowId hwndId = INVALID_HWND_ID);
		IWinBase* AddWindow(const Vec2I& pos, const Vec2I& size, ComponentType::Enum type, HWindowId hwndId = INVALID_HWND_ID);
		IWinBase* AddWindow(float posX, float posY, float width, float height, ComponentType::Enum type, HWindowId hwndId = INVALID_HWND_ID);
		IWinBase* AddWindow(ComponentType::Enum type, HWindowId hwndId = INVALID_HWND_ID);

		void DeleteWindow(IWinBase* pWnd);
		void DeleteWindowsFor(HWindowId hwndId);
		void SetFocusUI(IWinBase* pWnd);
		IWinBase* GetFocusUI() const;
		IWinBase* GetKeyboardFocusUI() const;
		IWinBase* GetNewFocusUI() const;
		void SetFocusUI(const char* uiName);
		bool IsFocused(const IWinBase* pWnd) const;
		void DirtyRenderList(HWindowId hwndId);

		void SetUIProperty(const char* uiname, const char* compname, const char* prop, const char* val, bool updatePosSize = false);
		void SetUIProperty(const char* uiname, const char* compname, UIProperty::Enum prop, const char* val, bool updatePosSize = false);
		void SetEnableComponent(const char* uiname, const char* compname, bool enable);

		// IInputListener Interfaces
		void ConsumeInput(IInputInjectorPtr injector);		
		void ProcessMouseInput(IInputInjectorPtr injector);
		void EnableInputListener(bool enable);
		bool IsEnabledInputLIstener() const;
		HCURSOR GetMouseCursorOver() const;
		void SetMouseCursorOver();
		void DisplayMsg(const std::string& msg, ...);
		bool IsMouseInUI() const;

		void SetTooltipString(const std::wstring& ts);
		void SetTooltipPos(const Vec2& npos, bool checkNewPos = true);
		void CleanTooltip();

		void PopupDialog(WCHAR* msg, POPUP_TYPE type, std::function< void(void*) > func);
		int GetPopUpResult() const;

		lua_State* GetLuaState() const;
		IWinBase* FindComp(const char* uiname, const char* compName) const;
		void FindUIWnds(const char* uiname, WinBases& outV) const;
		bool CacheListBox(const char* uiname, const char* compName);
		ListBox* GetCachedListBox() const;
		void SetEnablePosSizeEvent(bool enable);
		bool GetEnablePosSizeEvent() const;
		void SetVisible(const char* uiname, bool visible);
		void LockFocus(bool lock);
		bool GetVisible(const char* uiname) const;
		void CloseAllLuaUI();

		void CloneUI(const char* uiname, const char* newUIname);
		void IgnoreInput(bool ignore, IWinBase* modalWindow);
		void ToggleVisibleLuaUI(const char* uisname);

		void RegisterAlwaysOnTopWnd(IWinBase* win);
		void UnRegisterAlwaysOnTopWnd(IWinBase* win);

		void MoveToBottom(const char* moveToBottom);
		void MoveToBottom(IWinBase* moveToBottom);
		void MoveToTop(IWinBase* moveToTop);
		void HideUIsExcept(const std::vector<std::string>& excepts);

		void HighlightUI(const char* uiname);
		void StopHighlightUI(const char* uiname);

		IUIAnimation* GetGlobalAnimation(const char* animName);
		IUIAnimation* GetGlobalAnimationOrCreate(const char* animName);
		void PrepareTooltipUI();

		UICommands* GetUICommands() const;
		void SetUIEditorModuleHandle(HMODULE moduleHandle);
		HMODULE GetUIEditorModuleHandle() const;
		
		IWinBase* WinBaseWithPoint(const Vec2I& pt, const RegionTestParam& param);
		IWinBase* WinBaseWithPointCheckAlways(const Vec2I& pt, const RegionTestParam& param);
		TextManipulator* GetTextManipulator() const;
		
		const char* GetUIPath(const char* uiname) const;
		const char* GetUIScriptPath(const char* uiname) const;

		void SuppressPropertyWarning(bool suppress);

		void SetStyle(const char* style);

		const char* GetBorderRegion(const char* key) const;
		const char* GetWndBorderRegion(const char* key) const;
		const char* GetStyleString(Styles::Enum s) const;

		TexturePtr GetBorderAlphaInfoTexture(const Vec2I& size, bool& callmeLater);

		void AddAlwaysMouseOverCheck(IWinBase* comp);
		void RemoveAlwaysMouseOverCheck(IWinBase* comp);

		//-------------------------------------------------------------------
		// For UI Editing
		//-------------------------------------------------------------------	
		void SetUIEditor(IUIEditor* editor);
		IUIEditor* GetUIEditor() const;
		void StartLocatingComponent(ComponentType::Enum c);
		void CancelLocatingComponent();
		void ChangeFilepath(IWinBase* root, const char* newfile);
		void CopyCompsAtMousePos(const std::vector<IWinBase*>& src);
	};
}

#endif //_UIManager_header_include__