#pragma once
#include "FBCommonHeaders/platform.h"
#include "FBSceneManager/SpatialObject.h"
#include "FBRenderer/IRenderable.h"
#include "FBMathLib/Color.h"
namespace fastbird
{
	DECLARE_SMART_PTR(DustRenderer);
	class FB_DLL_SCENEOBJECTFACTORY DustRenderer : public SpatialObject, public IRenderable
	{
		DECLARE_PIMPL_NON_COPYABLE(DustRenderer);
		DustRenderer();
		~DustRenderer();

	public:
		
		static DustRendererPtr Create();

		//---------------------------------------------------------------------------
		// IRenderable Interfaces
		//---------------------------------------------------------------------------		
		void PreRender(const RenderParam& param, RenderParamOut* paramOut);
		void Render(const RenderParam& param, RenderParamOut* paramOut);
		void PostRender(const RenderParam& param, RenderParamOut* paramOut);


		// IDustRenderer
		void Initialize(const Vec3& min, const Vec3& max, size_t count,
			const Color& cmin, const Color& cmax, float normalizeDist);
		void SetMaterial(const char* filepath, int pass);
		void SetMaterial(MaterialPtr pMat, int pass);
		MaterialPtr GetMaterial(int pass = 0) const;
		const Vec3& GetMin() const;
	};
}