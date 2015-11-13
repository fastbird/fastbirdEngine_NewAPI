#pragma once
#include "RendererEnums.h"
#include "RendererStructs.h"
#include "ShaderDefines.h"
#include "InputElementDesc.h"
namespace tinyxml2{
	class XMLElement;
}
namespace fastbird{
	DECLARE_SMART_PTR(ColorRamp);
	DECLARE_SMART_PTR(Texture);
	DECLARE_SMART_PTR(Material);
	class FB_DLL_PUBLIC Material{
		DECLARE_PIMPL(Material);
		Material();
		static void* operator new (size_t);
		static void* operator new[](size_t);

		typedef std::vector< MaterialWeakPtr > Materials;
		static Materials mMaterials;

	public:
		static MaterialPtr Create();
		static void ReloadMaterial(const char* name);
		static void ReloadShader(const char* shaderPath);
		~Material();
		Material& Material::operator=(const Material& other);	

		virtual bool LoadFromFile(const char* filepath);
		bool LoadFromXml(tinyxml2::XMLElement* pRoot);
		virtual const char* GetName() const { return mName.c_str(); }
		virtual MaterialPtr GetAdam() const;
		virtual void SetAmbientColor(float r, float g, float b, float a);
		virtual void SetAmbientColor(const Vec4& ambient);
		virtual void SetDiffuseColor(float r, float g, float b, float a);
		virtual void SetDiffuseColor(const Vec4& diffuse);
		virtual void SetSpecularColor(float r, float g, float b, float shine);
		virtual void SetSpecularColor(const Vec4& specular);
		virtual void SetEmissiveColor(float r, float g, float b, float strength);
		virtual void SetEmissiveColor(const Vec4& emissive);
		virtual void SetTexture(const char* filepath, BINDING_SHADER shader, int slot,
			const SAMPLER_DESC& samplerDesc = SAMPLER_DESC());
		virtual void SetTexture(TexturePtr pTexture, BINDING_SHADER shader, int slot,
			const SAMPLER_DESC& samplerDesc = SAMPLER_DESC());
		virtual TexturePtr GetTexture(BINDING_SHADER shader, int slot);
		virtual void SetColorRampTexture(ColorRamp& cr, BINDING_SHADER shader, int slot,
			const SAMPLER_DESC& samplerDesc = SAMPLER_DESC());
		virtual void RemoveTexture(TexturePtr pTexture);
		virtual void RemoveTexture(BINDING_SHADER shader, int slot);
		virtual ColorRamp& GetColorRamp(int slot, BINDING_SHADER shader);
		virtual void RefreshColorRampTexture(int slot, BINDING_SHADER shader);
		virtual bool AddShaderDefine(const char* def, const char* val);
		virtual bool RemoveShaderDefine(const char* def);
		virtual void ApplyShaderDefines();
		virtual void SetMaterialParameters(unsigned index, const Vec4& value);
		virtual const SHADER_DEFINES& GetShaderDefines() const { return mShaderDefines; }
		virtual const Vec4& GetAmbientColor() const;
		virtual const Vec4& GetDiffuseColor() const;
		virtual const Vec4& GetSpecularColor() const;
		virtual const Vec4& GetEmissiveColor() const;
		virtual const char* GetShaderFile() const;
		virtual void* GetShaderByteCode(unsigned& size) const;
		virtual const Vec4& GetMaterialParameters(unsigned index);
		virtual bool IsUsingMaterialParameter(unsigned index);
		virtual bool IsRelatedShader(const char* shaderFile);
		virtual void Bind(bool inputLayout, unsigned stencilRef = 0);
		virtual void Unbind();
		MaterialPtr GetSubPassMaterial(RENDER_PASS p) const;
		virtual bool BindSubPass(RENDER_PASS p, bool includeInputLayout);
		virtual void BindMaterialParams();
		virtual void RegisterReloading();
		virtual void SetTransparent(bool trans);
		virtual void SetGlow(bool glow) { mGlow = glow; }
		virtual bool IsTransparent() const { return mTransparent; }
		virtual bool IsGlow() const { return mGlow; }
		virtual bool IsNoShadowCast() const { return mNoShadowCast; }
		virtual bool IsDoubleSided() const { return mDoubleSided; }
		virtual void ReloadShader();
		virtual int GetBindingShaders() const { return mShaders; }
		virtual void CopyMaterialParamFrom(const MaterialPtr src);
		virtual void CopyMaterialConstFrom(const MaterialPtr src);
		virtual void CopyTexturesFrom(const MaterialPtr src);
		virtual void CopyShaderDefinesFrom(const MaterialPtr src);
		virtual void SetRasterizerState(const RASTERIZER_DESC& desc);
		virtual void SetBlendState(const BLEND_DESC& desc);
		virtual void SetDepthStencilState(const DEPTH_STENCIL_DESC& desc);
		virtual RenderStatesPtr GetRenderStates() const { return mRenderStates; }
		virtual void ClearRasterizerState();
		virtual void ClearBlendState(const BLEND_DESC& desc);
		virtual void ClearDepthStencilState(const DEPTH_STENCIL_DESC& desc);
		virtual void CloneRenderStates();
		virtual void SetInputLayout(const INPUT_ELEMENT_DESCS& desc);
	};
}