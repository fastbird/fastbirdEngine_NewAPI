#pragma once
#define IRenderer_Version "1"
#include "IRendererStructs.h"
#include <memory>

namespace fastbird{
	class IShader;
	typedef intptr_t HWindowId;
	class IRenderer;
	typedef std::shared_ptr<IRenderer> IRendererPtr;
	typedef std::weak_ptr<IRenderer> IRendererWeakPtr;
	/** Plug-in interface for render engine like D3D11, OpenGL.	
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