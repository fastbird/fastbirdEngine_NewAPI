#pragma once
#define FB_RENDERER_VERSION "1"
#include "IPlatformRendererStructs.h"
#include "FBCommonHeaders/Types.h"
#include <memory>

namespace fastbird{
	class IPlatformShader;	
	DECLARE_SMART_PTR(ISamplerState);
	DECLARE_SMART_PTR(IRasterizerState);
	DECLARE_SMART_PTR(IDepthStencilState);
	DECLARE_SMART_PTR(IBlendState);
	DECLARE_SMART_PTR(IPlatformTexture);
	DECLARE_SMART_PTR(IPlatformShader);	
	DECLARE_SMART_PTR(IInputLayout);
	DECLARE_SMART_PTR(IIndexBuffer);
	DECLARE_SMART_PTR(IVertexBuffer);		
	DECLARE_SMART_PTR(IPlatformRenderer);
	
	/** Plug-in interface for render engine like D3D11, OpenGL.	
	*/
	class IPlatformRenderer{
	public:
		virtual bool InitCanvas(HWindowId id, HWindow window, int width, int height,
			IPlatformTexturePtr& outColorTexture, IPlatformTexturePtr& outDepthTexture) = 0;

		//-------------------------------------------------------------------
		// Resource creation
		//-------------------------------------------------------------------		
		virtual IPlatformTexturePtr CreateTexture(const char* path, bool async) = 0;
		virtual IPlatformTexturePtr CreateTexture(void* data, int width, int height,
			PIXEL_FORMAT format, BUFFER_USAGE usage, int  buffer_cpu_access,
			int texture_type) = 0;		
		virtual IVertexBufferPtr CreateVertexBuffer(void* data, unsigned stride,
			unsigned numVertices, BUFFER_USAGE usage, BUFFER_CPU_ACCESS_FLAG accessFlag) = 0;
		virtual IIndexBufferPtr CreateIndexBuffer(void* data, unsigned int numIndices,
			INDEXBUFFER_FORMAT format) = 0;
		virtual IPlatformShaderPtr CreateShader(const char* path, int shaders,
			const SHADER_DEFINES& defines);
		virtual IInputLayoutPtr CreateInputLayout(const INPUT_ELEMENT_DESCS& desc,
			void* shaderByteCode, unsigned size) = 0;
		virtual IBlendStatePtr CreateBlendState(const BLEND_DESC& desc) = 0;
		virtual IDepthStencilStatePtr CreateDepthStencilState(const DEPTH_STENCIL_DESC& desc) = 0;
		virtual IRasterizerStatePtr CreateRasterizerState(const RASTERIZER_DESC& desc) = 0;
		virtual ISamplerStatePtr CreateSamplerState(const SAMPLER_DESC& desc) = 0;

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
		virtual void DrawFullscreenQuad(IPlatformShader* pixelShader, bool farside) = 0;		
		virtual void DrawTriangle(const DrawTriangleParam& param) = 0;
		virtual void SetWireframe(bool enable) = 0;
		virtual bool GetWireframe() const = 0;
		virtual void Clear(Real r, Real g, Real b, Real a, Real z, unsigned char stencil) = 0;
		virtual void Clear(Real r, Real g, Real b, Real a) = 0;
		virtual void ClearState() = 0;
		virtual void Present() = 0;

		//-------------------------------------------------------------------
		// Debugging & Profiling
		//-------------------------------------------------------------------
		virtual void BeginEvent(const char* name) = 0;
		virtual void EndEvent() = 0;
		virtual void TakeScreenshot(const char* filename) = 0;

	protected:
		~IPlatformRenderer();
	};
}