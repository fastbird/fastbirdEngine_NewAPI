#pragma once
#include "FBCommonHeaders/Types.h"

namespace fastbird{
	DECLARE_SMART_PTR(SamplerState);
	DECLARE_SMART_PTR(DepthStencilState);
	DECLARE_SMART_PTR(BlendState);
	DECLARE_SMART_PTR(RasterizerState);
	DECLARE_SMART_PTR(Material);
	DECLARE_SMART_PTR(Shader);
	DECLARE_SMART_PTR(Texture);
	DECLARE_SMART_PTR(ResourceProvider);
	class ResourceProvider{
		DECLARE_PIMPL_NON_COPYABLE(ResourceProvider);
		ResourceProvider();

	protected:
		~ResourceProvider();

	public:
		static ResourceProviderPtr Create();

		virtual TexturePtr GetTexture(int ResourceType_Textures);
		virtual TexturePtr GetTexture(int ResourceType_Textures, int index);
		virtual ShaderPtr GetShader(int ResourceType_Shaders);
		virtual MaterialPtr GetMaterial(int ResourceType_Materials);
		virtual RasterizerStatePtr GetRasterizerState(int ResourceType_RasterizerStates);
		virtual BlendStatePtr GetBlendState(int ResourceType_BlendStates);
		virtual DepthStencilStatePtr GetDepthStencilState(int ResourceType_DepthStencilStates);
		virtual SamplerStatePtr GetSamplerState(int ResourceType_SamplerStates);
		virtual int GetNumToneMaps() const;
		virtual int GetNumLuminanceMaps() const;
	};
}

