#pragma once
#include "FBCommonHeaders/platform.h"
#include "FBRenderer/IRenderable.h"
namespace fastbird{
	DECLARE_SMART_PTR(MeshObject);
	DECLARE_SMART_PTR(RenderableFactory);
	class FB_DLL_PUBLIC RenderableFactory{
		DECLARE_PIMPL_NON_COPYABLE(RenderableFactory);
		RenderableFactory();
		
	public:
		static RenderableFactoryPtr Create();
		static RenderableFactoryPtr GetInstance();
		virtual ~RenderableFactory();
		virtual MeshObjectPtr CreateMesh(const char* path);
	};
}