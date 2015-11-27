#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	FB_DECLARE_SMART_PTR(Material);
	FB_DECLARE_SMART_PTR(FacadeTrail);
	class FB_DLL_ENGINEFACADE FacadeTrail{
		FB_DECLARE_PIMPL_NON_COPYABLE(FacadeTrail);
		FacadeTrail();
		~FacadeTrail();

	public:
		static FacadeTrailPtr Create();

		void SetOffset(const Transformation& offset);
		void AddPoint(const Transformation& parentLocation);
		void SetWidth(float width);
		bool AttachToScene();
		bool DetachFromScene();
		MaterialPtr GetMaterial();
		void SetDiffuseColor(const Color& color);
		void Update(TIME_PRECISION dt);
	};
}