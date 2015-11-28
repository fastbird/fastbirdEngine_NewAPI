#pragma once
#include "FBCommonHeaders/Types.h"
#include "FBRenderer/RenderPass.h"
namespace fastbird{
	FB_DECLARE_SMART_PTR(Scene);
	FB_DECLARE_SMART_PTR(Material);
	FB_DECLARE_SMART_PTR(SkyFacade);
	class FB_DLL_ENGINEFACADE SkyFacade{
		FB_DECLARE_PIMPL_NON_COPYABLE(SkyFacade);
		SkyFacade();
		~SkyFacade();

	public:
		static SkyFacadePtr Create();
		static SkyFacadePtr GetMain();

		void SetMaterial(const char* path, RENDER_PASS pass);
		void AttachToScene();
		void AttachToScene(ScenePtr scene);
		void DetachFromScene();
		MaterialPtr GetMaterial() const;
		void UpdateEnvironmentMap(const Vec3& pos);
		/// Attach as a blending sky of the main scene.
		void AttachToBlend();
		void SetAlpha(float alpha);
		void PrepareInterpolation(float time, SkyFacadePtr startFrom);
		void AttachBlendingSky(SkyFacadePtr blending);
		void SetInterpolationData(unsigned index, const Vec4& data);
	};
}