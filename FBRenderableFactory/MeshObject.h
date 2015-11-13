#pragma once
#include "FBCommonHeaders/platform.h"
namespace fastbird{
	DECLARE_SMART_PTR(MeshObject);
	class FB_DLL_PUBLIC MeshObject{
		DECLARE_PIMPL_NON_COPYABLE(MeshObject);
		MeshObject();

	public:
		virtual ~MeshObject();
	};
}