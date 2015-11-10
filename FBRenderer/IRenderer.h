#pragma once
#define IRenderer_Version "2015-11-10, 0"
#include "IRendererStructs.h"

namespace fastbird{
	class IShader;
	typedef intptr_t HWND_ID;
	DECLARE_SMART_PTR(IRenderer);
	/** Interface for specific renderers like D3D11, OpenGL.
	This is an internal interface and not intended to used as an API
	by clients. However they can crea	Except when day want to implement their own renderer.
	Clients should use \see Renderer instead of this one.
	\ internal
	*/
	class IRenderer{
	public:
		virtual ~IRenderer();

		//-------------------------------------------------------------------
		// Data
		//-------------------------------------------------------------------
		virtual void FrameData(void* data, int size) = 0;
		virtual void ObjectData(void* data, int size) = 0;
		virtual void PointLightData(void* data, int size) = 0;
		virtual void CameraData(void* data, int size) = 0;
		virtual void RenderTargetData(void* data, int size) = 0;
		virtual void SceneData(void* data, int size) = 0;		
		virtual void RareChangeData(void* data, int size) = 0;
		virtual void RadianceData(void* data, int size) = 0;
		virtual void MaterialData(void* data, int size) = 0;
		virtual void MaterialParamData(void* data, int size) = 0;
		virtual void BigData(void* data, int size) = 0;

		//-------------------------------------------------------------------
		// Drawing
		//-------------------------------------------------------------------
		virtual void Draw(unsigned int vertexCount, unsigned int startVertexLocation) = 0;
		virtual void DrawIndexed(unsigned indexCount, unsigned startIndexLocation, unsigned startVertexLocation) = 0;
		virtual void DrawQuad(const DrawQuadParam& param) = 0;
		virtual void DrawQuadLine(const DrawQuadLineParam& param) = 0;
		virtual void DrawQuadWithTexture(const DrawQuadWithTextureParam& param) = 0;
		virtual void DrawQuadWithTextureUV(const DrawQuadWithTextureUVParam& param) = 0;
		virtual void DrawBillboardWorldQuad(const DrawBillboardWorldQuadParam& param) = 0;
		virtual void DrawFullscreenQuad(IShader* pixelShader, bool farside) = 0;		
		virtual void DrawTriangle(const DrawTriangleParam& param) = 0;
		virtual void SetWireframe(bool enable) = 0;
		virtual void Clear(float r, float g, float b, float a, float z, unsigned char stencil) = 0;
		virtual void Clear(float r, float g, float b, float a) = 0;
		virtual void Present() = 0;

	};
}