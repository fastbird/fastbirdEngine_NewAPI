#pragma once
#ifndef __Rendererd3d11_header_included__
#define __Rendererd3d11_header_included__

#include "FBRenderer/IPlatformRenderer.h"
#include "FBRenderer/RendererStructs.h"
#include "EssentialEngineData/shaders/Constants.h"
#include "RenderStatesD3D11.h"

namespace fastbird
{
	class IObject;
	class ICamera;
	class VertexBufferD3D11;
	class TextureD3D11;
	class Color;
	class RendererD3D11;
	class IRenderTarget;
	
	class RendererD3D11 : public IPlatformRenderer
	{	
		DECLARE_PIMPL_NON_COPYABLE(RendererD3D11);
		RendererD3D11();
		virtual ~RendererD3D11();

	public:
		static IPlatformRenderer* Create();
		static void Delete(IPlatformRenderer* renderer);
		static RendererD3D11& GetInstance();	
		
		virtual bool Init(int threadPool);
	private:
		void GetOutputInformationFor(IDXGIAdapter1* adapter);
	public:
		virtual bool InitSwapChain(HWindowId id, int width, int height);
		virtual void ReleaseSwapChain(HWindowId id);
		// For full screen.
		// See Renderer::ChangeWindowSize for windowed.
		virtual void ChangeResolution(HWindowId id, const Vec2I& resol);
		virtual void OnSizeChanged(HWindowId id, const Vec2I& resol);
		virtual void Deinit();				
		virtual unsigned GetMultiSampleCount() const;		

		//-------------------------------------------------------------------
		// Resource Managerment 
		//-------------------------------------------------------------------
		virtual IRenderTarget* CreateRenderTarget(const RenderTargetParam& param);
		virtual void DeleteRenderTarget(IRenderTarget*);
		virtual IVertexBuffer* CreateVertexBuffer(void* data, unsigned stride, unsigned numVertices, BUFFER_USAGE usage, BUFFER_CPU_ACCESS_FLAG accessFlag);
		virtual void DeleteVertexBuffer(IVertexBuffer* buffer);
		virtual IIndexBuffer* CreateIndexBuffer(void* data, unsigned int numIndices, INDEXBUFFER_FORMAT format);
		virtual IShader* CreateShader(const char* filepath, int shaders,
			const IMaterial::SHADER_DEFINES& defines = IMaterial::SHADER_DEFINES(), IShader* pReloadingShader = 0);
		virtual ITexture* CreateTexture(const Vec2I& size, int mipLevels, int arraySize);
		virtual ITexture* CreateTexture(const char* file, ITexture* pReloadingTexture = 0, bool async = true);
		virtual ITexture* CreateTexture(void* data, int width, int height, PIXEL_FORMAT format,
			BUFFER_USAGE usage, int  buffer_cpu_access, int  type);
		virtual void SetViewports(Viewport viewports[], int num);
		virtual void SetScissorRects(RECT rects[], int num);
		virtual void RestoreScissorRects();
		virtual IInputLayout* CreateInputLayout(const INPUT_ELEMENT_DESCS& descs, void* shaderByteSignature, int size);
		virtual IRasterizerState* CreateRasterizerState(const RASTERIZER_DESC& desc);
		virtual IBlendState* CreateBlendState(const BLEND_DESC& desc);
		virtual IDepthStencilState* CreateDepthStencilState(const DEPTH_STENCIL_DESC& desc);
		virtual ISamplerState* CreateSamplerState(const SAMPLER_DESC& desc);

		virtual void GenerateMips(ITexture* pTexture);
		
		//-------------------------------------------------------------------
		// Resource Binding
		//-------------------------------------------------------------------
		virtual void ClearState();
		virtual void SetVertexBuffer(unsigned int startSlot, unsigned int numBuffers,
			VertexBufferD3D11* pVertexBuffers[], unsigned int strides[], unsigned int offsets[]);
		virtual void SetIndexBuffer(IIndexBuffer* pIndexBuffer);
		virtual void SetTexture(TextureD3D11* pTexture, BINDING_SHADER shaderType, unsigned int slot);
		virtual void SetTextures(TextureD3D11* pTextures[], int num, BINDING_SHADER shaderType, int startSlot);
		virtual void SetRenderTarget(TextureD3D11* pRenderTarget[], size_t rtIndex[], int num,
			TextureD3D11* pDepthStencil, size_t dsViewIndex);
		virtual void SetShaders(IShader* pShader);
		virtual void SetVSShader(IShader* pShader);
		virtual void SetPSShader(IShader* pShader);
		virtual void SetGSShader(IShader* pShader);
		virtual void SetDSShader(IShader* pShader);
		virtual void SetHSShader(IShader* pShader);
		virtual void SetInputLayout(IInputLayout* pInputLayout);
		virtual void SetPrimitiveTopology(PRIMITIVE_TOPOLOGY pt);
		virtual void SetRasterizerState(IRasterizerState* pRasterizerState);
		virtual void SetBlendState(IBlendState* pBlendState);
		virtual void SetDepthStencilState(IDepthStencilState* pDepthStencilState, unsigned stencilRef);
		virtual void SetSamplerState(ISamplerState* pSamplerState, BINDING_SHADER shader, int slot);

		//-------------------------------------------------------------------
		// Resource Manipulation
		//-------------------------------------------------------------------
		virtual MapData MapVertexBuffer(IVertexBuffer* pBuffer, UINT subResource,
			MAP_TYPE type, MAP_FLAG flag);
		virtual void UnmapVertexBuffer(IVertexBuffer* pBuffer, UINT subResource);

		virtual MapData MapTexture(ITexture* pTexture, UINT subResource,
			MAP_TYPE type, MAP_FLAG flag);
		virtual void UnmapTexture(ITexture* pTexture, UINT subResource);

		virtual void CopyToStaging(ITexture* dst, UINT dstSubresource, UINT dstx, UINT dsty, UINT dstz,
			ITexture* src, UINT srcSubresource, Box3D* pBox);

		virtual void SaveTextureToFile(ITexture* texture, const char* filename);

		//-------------------------------------------------------------------
		// Data
		//-------------------------------------------------------------------		
		virtual void FrameData(void* data, int size);
		virtual void ObjectData(void* data, int size);
		virtual void PointLightData(void* data, int size);
		virtual void CameraData(void* data, int size);
		virtual void RenderTargetData(void* data, int size);
		virtual void SceneData(void* data, int size);
		virtual void RareChangeData(void* data, int size);
		virtual void RadianceData(void* data, int size);
		virtual void MaterialData(void* data, int size);
		virtual void MaterialParamData(void* data, int size);
		virtual void BigData(void* data, int size);

		

		//-------------------------------------------------------------------
		// Drawing
		//-------------------------------------------------------------------
		virtual void Draw(unsigned int vertexCount, unsigned int startVertexLocation);
		virtual void DrawIndexed(unsigned indexCount, unsigned startIndexLocation, unsigned startVertexLocation);
		virtual void DrawQuad(const DrawQuadParam& param);
		virtual void DrawQuadLine(const DrawQuadLineParam& param);
		virtual void DrawQuadWithTexture(const DrawQuadWithTextureParam& param);
		virtual void DrawQuadWithTextureUV(const DrawQuadWithTextureUVParam& param);
		virtual void DrawBillboardWorldQuad(const DrawBillboardWorldQuadParam& param);
		virtual void DrawFullscreenQuad(IShader* pixelShader, bool farside);
		virtual void DrawTriangle(const DrawTriangleParam& param);
		virtual void SetWireframe(bool enable);
		virtual void Clear(float r, float g, float b, float a, float z, unsigned char stencil);
		virtual void Clear(float r, float g, float b, float a);
		virtual void Present();

		//-------------------------------------------------------------------
		// Accessing Information -- probably should remove. Handle in FBRenderer
		//-------------------------------------------------------------------
		virtual unsigned GetNumLoadingTexture() const;
		virtual Vec2I FindClosestSize(HWindowId id, const Vec2I& input);
		virtual void ChangeFullscreenMode(int mode);
		virtual bool GetResolutionList(unsigned& outNum, Vec2I* list);

		//-------------------------------------------------------------------
		// Platform Specific
		//-------------------------------------------------------------------
	private:
		
		MapData MapBuffer(ID3D11Resource* pResource, UINT subResource, MAP_TYPE type, MAP_FLAG flag);
		bool ResizeSwapChain(HWindowId hwndId, const Vec2I& resol);
		RenderTarget* CreateRenderTargetFor(IDXGISwapChain* swapChain, const Vec2I& size);
		bool FindClosestMatchingMode(const DXGI_MODE_DESC* finding, DXGI_MODE_DESC* best, HMONITOR monitor);

	};
}



#endif //__Rendererd3d11_header_included__