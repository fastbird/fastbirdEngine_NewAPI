#pragma once
#include "FBCommonHeaders/platform.h"
#include "FBRenderer/IRenderable.h"
namespace fastbird{
	DECLARE_SMART_PTR(MeshObject);
	class FB_DLL_PUBLIC RenderableFactory{
		DECLARE_PIMPL(RenderableFactory)
		RenderableFactory();
		
	public:
		virtual ~RenderableFactory();
		virtual MeshObjectPtr CreateMesh(const char* path);
	};
}