#pragma once
#include "FBCommonHeaders/Types.h"
#include "SceneObjectFlag.h"
namespace fastbird{
	DECLARE_SMART_PTR(Scene);
	DECLARE_SMART_PTR(SceneObject);
	class FB_DLL_PUBLIC SceneObject{
		DECLARE_PIMPL_CLONEABLE(SceneObject);		

	protected:
		SceneObject();
		~SceneObject();

	public:

		void SetName(const char* name);
		const char* GetName() const;
		void OnAttachedToScene(ScenePtr pScene);
		void OnDetachedFromScene(ScenePtr pScene);
		/**
		\param pScene Pass null to check whether the object is attached to any scene. Otherwise checks for the specified scene.
		*/
		bool IsAttached(ScenePtr pScene) const;

		//-------------------------------------------------------------------
		// Object Flags
		//-------------------------------------------------------------------
		void SetObjFlag(unsigned flag);
		unsigned GetObjFlag() const;
		void ModifyObjFlag(unsigned flag, bool enable);
		bool HasObjFlag(unsigned flag);
		void SetVisible(bool visible);
		bool GetVisible() const;

		//-------------------------------------------------------------------
		// Debugging features
		//-------------------------------------------------------------------
		void SetGameType(int type);
		int GetGameType() const;
		void SetGameId(unsigned id);
		unsigned GetGameId() const;
		void SetGamePtr(void* ptr);
		void* GetGamePtr() const;
	};
}