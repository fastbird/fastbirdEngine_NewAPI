#pragma once
namespace fastbird{
	struct RenderParam;
	struct RenderParamOut;
	class RenderableObject{
	public:
		//-------------------------------------------------------------------
		// Rendering
		//-------------------------------------------------------------------
		virtual void PreRender(const RenderParam& param, RenderParamOut* paramOut) = 0;
		virtual void Render(const RenderParam& param, RenderParamOut* paramOut) = 0;
		virtual void PostRender(const RenderParam& param, RenderParamOut* paramOut) = 0;
	};
}