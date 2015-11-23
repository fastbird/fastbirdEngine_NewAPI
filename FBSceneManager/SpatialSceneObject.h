#pragma once
#include "FBCommonHeaders/Types.h"
#include "SceneObject.h"
#include "SpatialObject.h"
namespace fastbird{		
	DECLARE_SMART_PTR(SpatialSceneObject);
	class FB_DLL_SCENEMANAGER SpatialSceneObject : public SceneObject, public SpatialObject{
	protected:
		SpatialSceneObject();		
		~SpatialSceneObject();
	};
}