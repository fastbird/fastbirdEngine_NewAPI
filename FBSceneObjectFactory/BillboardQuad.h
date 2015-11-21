#pragma once
#include "FBCommonHeaders/platform.h"
#include "FBSceneManager/SpatialObject.h"
#include "FBRenderer/IRenderable.h"
namespace fastbird
{
	DECLARE_SMART_PTR(Color);
	DECLARE_SMART_PTR(Material);
	DECLARE_SMART_PTR(BillboardQuad);
	class FB_DLL_SCENEOBJECTFACTORY BillboardQuad : public SpatialObject, public IRenderable
	{
		DECLARE_PIMPL_NON_COPYABLE(BillboardQuad);
		BillboardQuad();
		~BillboardQuad();

	public:

		static BillboardQuadPtr Create();
		//---------------------------------------------------------------------------
		// IRenderable Interfaces
		//---------------------------------------------------------------------------
		void PreRender(const RenderParam& param, RenderParamOut* paramOut);
		void Render(const RenderParam& param, RenderParamOut* paramOut);
		void PostRender(const RenderParam& param, RenderParamOut* paramOut);

		void SetMaterial(MaterialPtr mat);
		MaterialPtr GetMaterial() const;		
		void SetBillobardData(const Vec3& pos, const Vec2& size, const Vec2& offset, const Color& color);
		void SetAlphaBlend(bool blend);
	};
}