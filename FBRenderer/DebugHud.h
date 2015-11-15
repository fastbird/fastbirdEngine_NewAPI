#pragma once
#include "FBCommonHeaders/Types.h"
#include "Color.h"
#include "FBMathLib/Math.h"
#include "../EssentialEngineData/shaders/Constants.h"
#include <queue>
#include <list>
#include <map>
#undef DrawText
namespace fastbird
{
	DECLARE_SMART_PTR(MeshObject);
	DECLARE_SMART_PTR(Shader);
	DECLARE_SMART_PTR(Material);
	DECLARE_SMART_PTR(RenderStates);
	DECLARE_SMART_PTR(InputLayout);
	DECLARE_SMART_PTR(VertexBuffer);
	DECLARE_SMART_PTR(Scene);
	DECLARE_SMART_PTR(DebugHud);
	class DebugHud
	{
		DECLARE_PIMPL(DebugHud);
		DebugHud();

	public:
		static DebugHudPtr Create();
		~DebugHud();
		void SetRenderTargetSize(const Vec2I& size);		
		void Render(const RenderParam& renderParam, RenderParamOut* renderParamOut);		
		void OnBeforeRenderingTransparents(Scene* scene);

		//--------------------------------------------------------------------
		// Own
		//--------------------------------------------------------------------
		void DrawTextForDuration(float secs, const Vec2I& pos, WCHAR* text, 
			const Color& color, float size);
		void ClearDurationTexts();
		void DrawText(const Vec2I& pos, WCHAR* text, const Color& color, float size);
		void Draw3DText(const Vec3& pos, WCHAR* text, const Color& color, float size);
		// if wolrdspace is false, it's in the screenspace 0~width, 0~height
		void DrawLine(const Vec3& start, const Vec3& end, const Color& color0,
			const Color& color1);
		void DrawLineBeforeAlphaPass(const Vec3& start, const Vec3& end, const Color& color0,
			const Color& color1);
		void DrawLine(const Vec2I& start, const Vec2I& end, const Color& color0, 
			const Color& color1);
		void DrawQuad(const Vec2I& pos, const Vec2I& size, const Color& color);

		void DrawSphere(const Vec3& pos, float radius, const Color& color);
		void DrawBox(const Vec3& boxMin, const Vec3& boxMax, const Color& color, float alpha);
		void DrawTriangle(const Vec3& a, const Vec3& b, const Vec3& c, const Color& color, float alpha);
	};

}