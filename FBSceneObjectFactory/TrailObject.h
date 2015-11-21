#pragma once
#include "FBCommonHeaders/platform.h"
#include "FBSceneManager/SpatialObject.h"
#include "FBRenderer/IRenderable.h"
namespace fastbird{
	DECLARE_SMART_PTR(Material);
	DECLARE_SMART_PTR(TrailObject);
	class FB_DLL_SCENEOBJECTFACTORY TrailObject : public SpatialObject, public IRenderable
	{
		DECLARE_PIMPL_NON_COPYABLE(TrailObject);
		TrailObject();
		~TrailObject();

	public:
		static TrailObjectPtr Create();

		//---------------------------------------------------------------------------
		// IRenderable Interfaces
		//---------------------------------------------------------------------------		
		void PreRender(const RenderParam& param, RenderParamOut* paramOut);
		void Render(const RenderParam& param, RenderParamOut* paramOut);
		void PostRender(const RenderParam& param, RenderParamOut* paramOut);		

		//------------------------------------------------------------------------
		// Own
		//------------------------------------------------------------------------
		void SetMaterial(const char* filepath);
		void SetMaterial(MaterialPtr pMat);
		MaterialPtr GetMaterial() const;
		//for billboard trail - automatically face to the camera
		void AddPoint(const Vec3& worldPos);
		void SetWidth(float width);
		// for manual trail
		void AddPoint(const Vec3& worldPosA, const Vec3& worldPosB);		
		void SetMaxPoints(unsigned num);
		void Clear();
		void Update(float dt) ;
		float GetDistToCam() const;
	};
}