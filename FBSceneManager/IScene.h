#pragma once
#include "FBCommonHeaders/Types.h"
#include "FBMathLib/Vec4.h"
namespace fastbird{
	struct DirectionalLightInfo{
		Vec4 mDirection_Intensiy;
		Vec4 mDiffuse;
		Vec4 mSpecular;
	};
	struct RenderParam;
	struct RenderParamOut;
	class Color;
	DECLARE_SMART_PTR(SkySphere);
	DECLARE_SMART_PTR(IScene);
	DECLARE_SMART_PTR(ISceneObserver);
	class IScene{
	public:
		virtual void AddSceneObserver(int ISceneObserverEnum, ISceneObserverPtr observer) = 0;
		virtual void GetDirectionalLightInfo(unsigned idx, DirectionalLightInfo& data) = 0;
		virtual const Vec3& GetMainLightDirection() = 0;
		virtual void PreRender(const RenderParam& prarm, RenderParamOut* paramOut) = 0;
		virtual void Render(const RenderParam& prarm, RenderParamOut* paramOut) = 0;
		virtual void PreRenderCloudVolumes(const RenderParam& prarm, RenderParamOut* paramOut) = 0;
		virtual void RenderCloudVolumes(const RenderParam& prarm, RenderParamOut* paramOut) = 0;			
		virtual const Color& GetFogColor() const = 0;
		virtual void AttachSkySphere(SkySpherePtr p) = 0;
		virtual void DetachSkySphere() = 0;
	};
}