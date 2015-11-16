#pragma once
#include "FBCommonHeaders/Types.h"
#include "Color.h"
#include "FBMathLib/Vec2I.h"
#include "FBMathLib/Vec3.h"
namespace fastbird{
	struct RenderParamOut;
	struct RenderParam;
	DECLARE_SMART_PTR(GeometryRenderer);
	class GeometryRenderer{
		DECLARE_PIMPL_NON_COPYABLE(GeometryRenderer);
		GeometryRenderer();

	public:
		static GeometryRendererPtr Create();
		~GeometryRenderer();

		void SetRenderTargetSize(const Vec2I& size);
		void Render(const RenderParam& renderParam, RenderParamOut* renderParamOut);
		// if wolrdspace is false, it's in the screenspace 0~width, 0~height
		void DrawLine(const Vec3& start, const Vec3& end, const Color& color0, const Color& color1);
		void DrawLineBeforeAlphaPass(const Vec3& start, const Vec3& end, const Color& color0, const Color& color1);
		void DrawTexturedThickLine(const Vec3& start, const Vec3& end, const Color& color0, const Color& color1, float thickness,
			const char* texture, bool textureFlow);
		void DrawSphere(const Vec3& pos, float radius, const Color& color);
		void DrawBox(const Vec3& boxMin, const Vec3& boxMax, const Color& color, float alpha);
		void DrawTriangle(const Vec3& a, const Vec3& b, const Vec3& c, const Color& color, float alpha);
	};
}