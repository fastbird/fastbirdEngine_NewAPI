#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	FB_DECLARE_SMART_PTR(Material);
	FB_DECLARE_SMART_PTR(TrailFacade);
	class FB_DLL_ENGINEFACADE TrailFacade{
		FB_DECLARE_PIMPL_NON_COPYABLE(TrailFacade);
		TrailFacade();
		~TrailFacade();

	public:
		static TrailFacadePtr Create();

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