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
		/** Returns the SceneManager as a reference.
		This function does not check the validity whether the SceneManager is created or not.
		It will cause a crash if you call this function without calling CreateSceneManager();
		*/
		static SceneManager& GetInstance();
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