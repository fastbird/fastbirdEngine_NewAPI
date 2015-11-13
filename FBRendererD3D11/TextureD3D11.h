#pragma once

#include "FBRenderer/ITexture.h"
namespace fastbird
{
	DECLARE_SMART_PTR(TextureD3D11);
	class TextureD3D11 : public ITexture
	{
		DECLARE_PIMPL(TextureD3D11);
		TextureD3D11();

	public:
		static TextureD3D11Ptr Create();
		~TextureD3D11();

		//--------------------------------------------------------------------
		// ITexture
		//--------------------------------------------------------------------
		virtual bool LoadFile(const char* filepath, bool async) = 0;
		virtual const char* GetName() const = 0;
		virtual void SetSlot(int slot) = 0;
		virtual int GetSlot() const = 0;
		virtual void SetShaderStage(BINDING_SHADER shader) = 0;
		virtual BINDING_SHADER GetShaderStage() const = 0;
		virtual bool IsReady() const = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual MapData Map(UINT subResource, MAP_TYPE type, MAP_FLAG flag) const = 0;
		virtual void Unmap(UINT subResource) const = 0;
		virtual void CopyToStaging(ITexture* dst, UINT dstSubresource,
			UINT dstX, UINT dstY, UINT dstZ, UINT srcSubresource, Box3D* srcBox) const = 0;
		virtual void SaveToFile(const char* filename) const = 0;
		virtual void GenerateMips() = 0;
		virtual void SetDebugName(const char*) = 0;


		virtual bool IsReady() const;
		virtual Vec2I GetSize() const;
		virtual unsigned GetWidth() const;
		virtual unsigned GetHeight() const;
		virtual PIXEL_FORMAT GetFormat() const;
		virtual void Bind();
		virtual void Unbind();

		virtual void SetSlot(int slot);
		virtual int GetSlot() const { return mSlot; }
		virtual void SetShaderStage(BINDING_SHADER shader);
		virtual BINDING_SHADER GetShaderStage() const { return mBindingShader; }
		virtual ITexture* Clone() const;
		virtual void CopyToStaging(ITexture* dst, UINT dstSubresource, UINT dstX, UINT dstY, UINT dstZ,
			UINT srcSubresource, Box3D* srcBox);
		virtual void SaveToFile(const char* filename);
		virtual void SetDebugName(const char*);

		void SetSize(const Vec2I& size);

	private:
		friend class RendererD3D11;
		ID3D11Texture2D* GetHardwareTexture() const;
		ID3D11ShaderResourceView* GetHardwareResourceView();
		void SetHardwareTexture(ID3D11Texture2D* pTexture);
		void SetHardwareResourceView(ID3D11ShaderResourceView* pResourceView);
		void AddRenderTargetView(ID3D11RenderTargetView* pRenderTargetView);
		void ClearRenderTargetViews();
		ID3D11RenderTargetView* GetRenderTargetView(int idx) const;
		size_t NumRTViews() const { return mRTViews.size(); }
		void AddDepthStencilView(ID3D11DepthStencilView* pDepthStencilView);
		void ClearDepthStencilViews();
		ID3D11DepthStencilView* GetDepthStencilView(int idx) const;
		size_t NumDSViews() const { return mDSViews.size(); }
		virtual void GenerateMips();
		void OnReloaded();

	private:
		ID3D11Texture2D* mTexture;

		HRESULT mHr;
		D3DX11_IMAGE_INFO mImageInfo;
		D3DX11_IMAGE_LOAD_INFO mLoadInfo;
		ID3D11ShaderResourceView* mSRView;
		ID3D11ShaderResourceView** mSRViewParent;
		std::vector<ID3D11RenderTargetView*> mRTViews;
		std::vector<ID3D11DepthStencilView*> mDSViews;

		/*ID3D11SamplerState* mSamplerState;*/
		PIXEL_FORMAT mPixelFormat;
		int mSlot;
		BINDING_SHADER mBindingShader;
	};
}