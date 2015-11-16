#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	DECLARE_SMART_PTR(MeshObject);
	DECLARE_SMART_PTR(Scene);
	class FB_DLL_PUBLIC SceneManager{
		DECLARE_PIMPL_NON_COPYABLE(SceneManager);
		SceneManager();
		~SceneManager();
		static SceneManager* sSceneManager;
	public:
		static SceneManager* CreateSceneManager();
		static SceneManager* GetInstance();
		static void DeleteSceneManager();

		ScenePtr CreateScene(const char* name);
		/** Creates a MeshObject with .dae file.
		This function creates a MeshObject from the .dae file and returned the cloned version of it.
		The original create version(archetype) will be preserved. Whenever you requested the same
		.dae file for another mesh object, the new mesh will be cloned from the archetype.
		*/
		MeshObjectPtr CreateMeshObject(const char* daeFilePath);
		/** Get an archetype mesh already loaded.
		*/
		MeshObjectConstPtr GetMeshArcheType(const char* name);

	};
}