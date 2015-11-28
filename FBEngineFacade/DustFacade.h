#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	FB_DECLARE_SMART_PTR(DustFacade);
	class FB_DLL_ENGINEFACADE DustFacade{
		FB_DECLARE_PIMPL_NON_COPYABLE(DustFacade);
		DustFacade();
		~DustFacade();

	public:
		static DustFacadePtr Create();
		void InitDustRenderer(const Vec3& min, const Vec3& max, size_t count,
			const Color& cmin, const Color& cmax, float normalizeDist);
		void SetMaterial(const char* path);
		void AttachToScene();
		void DetachFromScene();
		const Vec3& GetMin() const;
	};
}