#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	FB_DECLARE_SMART_PTR(Material);
	FB_DECLARE_SMART_PTR(BillboardQuadFacade);
	class FB_DLL_ENGINEFACADE BillboardQuadFacade{
		FB_DECLARE_PIMPL_NON_COPYABLE(BillboardQuadFacade);
		BillboardQuadFacade();
		~BillboardQuadFacade();

	public:
		static BillboardQuadFacadePtr Create();
		void AttachToCurrentScene();
		void DetachFromScenes();
		void SetAlwaysPassCullingTest(bool passAlways);
		void SetBillobardData(const Vec3& pos, const Vec2& size, const Vec2& offset, const Color& color);
		void SetMaterial(MaterialPtr mat);
		MaterialPtr GetMaterial() const;
	};
}