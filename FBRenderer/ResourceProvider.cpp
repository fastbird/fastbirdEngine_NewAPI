#include "stdafx.h"
#include "ResourceProvider.h"
#include "ResourceTypes.h"
#include "Renderer.h"
#include "Texture.h"
#include "FBCommonHeaders/VectorMap.h"
#include "FBStringLib/StringLib.h"
using namespace fastbird;

static const int NumToneMaps = 5;
static const int NumLuminanceMaps = 3;
class ResourceProvider::Impl
{
public:
	VectorMap<int, std::vector<TexturePtr> > mTextures;
	VectorMap<int, ShaderPtr> mShaders;
	VectorMap<int, MaterialPtr> mMaterials;
	VectorMap<int, RasterizerStatePtr> mRasterizerStates;
	VectorMap<int, BlendStatePtr> mBlendStates;
	VectorMap<int, DepthStencilStatePtr> mDepthStencilStates;
	VectorMap<int, SamplerStatePtr> mSamplerStates;

	//---------------------------------------------------------------------------
	std::vector<TexturePtr> CreateTexture(int ResourceType_Textures){
		std::vector<TexturePtr> ret;
		auto& renderer = Renderer::GetInstance();
		switch (ResourceType_Textures){		
		case ResourceTypes::Textures::Noise:
		{
			auto texture = renderer.CreateTexture("es/textures/pnoise.dds");
			if (!texture){
				Logger::Log(FB_ERROR_LOG_ARG, "Failed to create noise texture.");
			}
			else{
				ret.push_back(texture);
			}
			return ret;
			break;
		}
		case ResourceTypes::Textures::GGXGenTarget:
		{
			auto texture = renderer.CreateTexture(0, 512, 128, PIXEL_FORMAT_R16G16B16A16_FLOAT, BUFFER_USAGE_DEFAULT,
				BUFFER_CPU_ACCESS_NONE, TEXTURE_TYPE_RENDER_TARGET_SRV);
			if (!texture){
				Logger::Log(FB_ERROR_LOG_ARG, "Failed to create ggx gen target.");
			}
			else{
				ret.push_back(texture);
				return ret;
			}
			break;
		}	
		case ResourceTypes::Textures::ToneMap:
		{
			int nSampleLen = 1;
			// 1, 3, 9, 27, 81
			for (int i = 0; i < NumToneMaps; ++i){
				auto texture = renderer.CreateTexture(0, nSampleLen, nSampleLen,
					PIXEL_FORMAT_R16_FLOAT, BUFFER_USAGE_DEFAULT,
					BUFFER_CPU_ACCESS_NONE, TEXTURE_TYPE_RENDER_TARGET_SRV);
				if (!texture){
					Logger::Log(FB_ERROR_LOG_ARG, FormatString("Failed to create a tone map with size (%d)", nSampleLen).c_str());
				}
				else{
					char buff[255];
					sprintf_s(buff, "ToneMap(%d)", nSampleLen);
					texture->SetDebugName(buff);
					nSampleLen *= 3;
					ret.push_back(texture);
				}
			}
			return ret;
			break;
		}
		case ResourceTypes::Textures::LuminanceMap:
		{
			for (int i = 0; i < NumLuminanceMaps; ++i){
				auto texture = renderer.CreateTexture(0, 1, 1, PIXEL_FORMAT_R16_FLOAT, BUFFER_USAGE_DEFAULT,
					BUFFER_CPU_ACCESS_NONE, TEXTURE_TYPE_RENDER_TARGET_SRV);
				if (!texture){
					Logger::Log(FB_ERROR_LOG_ARG, FormatString("Failed to create luminance map(%d)", i).c_str());
				}
				else{
					ret.push_back(texture);
				}
			}
			return ret;
			break;
		}
		default:
		{
			Logger::Log(FB_ERROR_LOG_ARG, FormatString("Resource Provider cannot create the texture(%d)", ResourceType_Textures).c_str());
			break;
		}
		
		}
	}

	TexturePtr GetTexture(int ResourceType_Textures){
		GetTexture(ResourceType_Textures, 0);
	}

	TexturePtr GetTexture(int ResourceType_Textures, int index){
		auto it = mTextures.Find(ResourceType_Textures);
		if (it == mTextures.end() || index >= (int)it->second.size()){
			auto textures = CreateTexture(ResourceType_Textures);
			mTextures[ResourceType_Textures] = textures;
			if (index < textures.size()){
				return textures[index];
			}
		}
		else{
			if (index < it->second.size())
				return it->second[index];
		}
		return 0;
	}

	ShaderPtr CreateShader(int ResourceType_Shaders){
		auto& renderer = Renderer::GetInstance();
		SHADER_DEFINES shaderDefines;
		switch (ResourceType_Shaders){
		case ResourceTypes::Shaders::FullscreenQuadNearVS:
		{
			return renderer.CreateShader("es/shaders/fullscreenquadvs.hlsl", BINDING_SHADER_VS);
		}
		case ResourceTypes::Shaders::FullscreenQuadFarVS:
		{
			shaderDefines.push_back(ShaderDefine("_FAR_SIDE_QUAD", "1"));
			return renderer.CreateShader("es/shaders/fullscreenquadvs.hlsl", BINDING_SHADER_VS, shaderDefines);			
		}
		case ResourceTypes::Shaders::CopyPS:
		{
			return renderer.CreateShader("es/shaders/copyps.hlsl", BINDING_SHADER_PS);			
		}
		case ResourceTypes::Shaders::CopyPSMS:
		{
			shaderDefines.push_back(ShaderDefine("_MULTI_SAMPLE", "1"));
			return renderer.CreateShader("es/shaders/copyps.hlsl", BINDING_SHADER_PS, shaderDefines);
		}
		case ResourceTypes::Shaders::ShadowMapVSPS:
		{
			return renderer.CreateShader("es/shaders/shadowdepth.hlsl", BINDING_SHADER_VS | BINDING_SHADER_PS);
		}
		case ResourceTypes::Shaders::DepthWriteVSPS:
		{
			return renderer.CreateShader("es/shaders/depth.hlsl", BINDING_SHADER_VS | BINDING_SHADER_PS);
		}
		case ResourceTypes::Shaders::CloudDepthWriteVSPS:
		{
			return renderer.CreateShader("es/shaders/depth_cloud.hlsl", BINDING_SHADER_VS | BINDING_SHADER_PS);
		}
		case ResourceTypes::Shaders::SampleLumInitialPS:
		{
			if (renderer.GetMultiSampleCount() != 1) {
				shaderDefines.push_back(ShaderDefine("_MULTI_SAMPLE", "1"));				
			}
			return renderer.CreateShader("es/shaders/SampleLumInitialNew.hlsl", BINDING_SHADER_PS, shaderDefines);
		}
		case ResourceTypes::Shaders::SampleLumIterativePS:
		{
			return renderer.CreateShader("es/shaders/SampleLumIterativeNew.hlsl", BINDING_SHADER_PS);
		}
		case ResourceTypes::Shaders::SampleLumFinalPS:
		{
			return renderer.CreateShader("es/shaders/SampleLumFinalNew.hlsl", BINDING_SHADER_PS);
		}
		case ResourceTypes::Shaders::CalcAdaptedLumPS:
		{
			return renderer.CreateShader("es/shaders/CalculateAdaptedLum.hlsl", BINDING_SHADER_PS);
		}
		case ResourceTypes::Shaders::ToneMappingPS:
		{
			if (renderer.GetMultiSampleCount() != 1)
				shaderDefines.push_back(ShaderDefine("_MULTI_SAMPLE", "1"));

			if (renderer.GetLuminanaceOnCPU())
				shaderDefines.push_back(ShaderDefine("_CPU_LUMINANCE", "1"));

			if (renderer.GetFilmicToneMapping())
				shaderDefines.push_back(ShaderDefine("_FILMIC_TONEMAP", "1"));
			return renderer.CreateShader("es/shaders/tonemapping.hlsl", BINDING_SHADER_PS, shaderDefines);

		}
		case ResourceTypes::Shaders::BrightPassPS:
		{
			if (renderer.GetMultiSampleCount() != 1) {
				shaderDefines.push_back(ShaderDefine("_MULTI_SAMPLE", "1"));
			}
			return renderer.CreateShader("es/shaders/brightpassps.hlsl", BINDING_SHADER_PS, shaderDefines);
		}
		case ResourceTypes::Shaders::BloomPS:
		{
			return renderer.CreateShader("es/shaders/bloomps.hlsl", BINDING_SHADER_PS);
		}
		case ResourceTypes::Shaders::Blur5x5PS:
		{
			if (renderer.GetMultiSampleCount() != 1) {
				shaderDefines.push_back(ShaderDefine("_MULTI_SAMPLE", "1"));
			}
			return renderer.CreateShader("es/shaders/gaussblur5x5.hlsl", BINDING_SHADER_PS, shaderDefines);
		}
		case ResourceTypes::Shaders::StarGlarePS:
		{
			return renderer.CreateShader("es/shaders/starglare.hlsl", BINDING_SHADER_PS);
		}
		case ResourceTypes::Shaders::MergeTextures2PS:
		{
			return renderer.CreateShader("es/shaders/mergetextures2ps.hlsl", BINDING_SHADER_PS);
		}
		case ResourceTypes::Shaders::GodRayPS:
		{
			return renderer.CreateShader("es/shaders/GodRayPS.hlsl", BINDING_SHADER_PS);
		}
		case ResourceTypes::Shaders::OcclusionPrePassVSPS:
		{
			return renderer.CreateShader("es/shaders/OccPrePass.hlsl", BINDING_SHADER_VS | BINDING_SHADER_PS);
		}
		case ResourceTypes::Shaders::OcclusionPrePassVSGSPS:
		{
			return renderer.CreateShader("es/shaders/OccPrePassGS.hlsl", BINDING_SHADER_VS | BINDING_SHADER_GS | BINDING_SHADER_PS);
		}
		case ResourceTypes::Shaders::GlowPS:
		{
			if (renderer.GetMultiSampleCount() != 1) {
				shaderDefines.push_back(ShaderDefine("_MULTI_SAMPLE", "1"));
			}
			return renderer.CreateShader("es/shaders/BloomPS.hlsl", BINDING_SHADER_PS, shaderDefines);
		}
		case ResourceTypes::Shaders::SilouettePS:
		{
			return renderer.CreateShader("es/shaders/silouette.hlsl", BINDING_SHADER_PS);
		}
		case ResourceTypes::Shaders::GGXGenPS:
		{
			return renderer.CreateShader("es/shaders/GGXGen.hlsl", BINDING_SHADER_PS);
		}
		default:
			Logger::Log(FB_ERROR_LOG_ARG, "Resource Provider - Unknown shader type");
			return 0;
		}
	}

	ShaderPtr GetShader(int ResourceType_Shaders){
		auto it = mShaders.Find(ResourceType_Shaders);
		if (it == mShaders.end()){
			auto shader = CreateShader(ResourceType_Shaders);
			if (!shader){
				Logger::Log(FB_ERROR_LOG_ARG, FormatString("Resource Provider - Failed to create the shader (%d)", ResourceType_Shaders).c_str());
				return 0;
			}
			mShaders[ResourceType_Shaders] = shader;
			return shader;
		}
		return it->second;
	}

	MaterialPtr CreateMaterial(int ResourceType_Materials){
		auto& renderer = Renderer::GetInstance();
		switch (ResourceType_Materials){
		case ResourceTypes::Materials::Missing:{
			return renderer.CreateMaterial("es/materials/missing.material");
		}
		case ResourceTypes::Materials::Quad:{
			return renderer.CreateMaterial("es/materials/quad.material");
		}
		case ResourceTypes::Materials::QuadTextured:{
			return renderer.CreateMaterial("es/materials/QuadWithTexture.material");
		}
		case ResourceTypes::Materials::BillboardQuad:{
			return renderer.CreateMaterial("es/materials/billboardQuad.material");
		}
		default:
			Logger::Log(FB_ERROR_LOG_ARG, "Resource Provider - Unknown resource type.");
			return 0;
		}
	}

	MaterialPtr GetMaterial(int ResourceType_Materials){
		auto it = mMaterials.Find(ResourceType_Materials);
		if (it == mMaterials.end()){
			auto material = CreateMaterial(ResourceType_Materials);
			if (!material){
				Logger::Log(FB_ERROR_LOG_ARG, "Resource Provider - Failed to create a material(%d)", ResourceType_Materials);
				return 0;
			}
			mMaterials[ResourceType_Materials] = material;
			return material;
		}

		return it->second;
	}

	RasterizerStatePtr CreateRasterizerState(int ResourceType_RasterizerStates){
		auto& renderer = Renderer::GetInstance();
		RASTERIZER_DESC desc;
		switch (ResourceType_RasterizerStates){
		case ResourceTypes::RasterizerStates::Default:
		{
			return renderer.CreateRasterizerState(desc);
		}
		case ResourceTypes::RasterizerStates::CullFrontFace:
		{
			desc.CullMode = CULL_MODE_FRONT;
			return renderer.CreateRasterizerState(desc);
		}
		case ResourceTypes::RasterizerStates::OneBiased:
		{
			desc.DepthBias = 1;
			return renderer.CreateRasterizerState(desc);
		}
		case ResourceTypes::RasterizerStates::WireFrame:
		{
			desc.FillMode = FILL_MODE_WIREFRAME;
			desc.CullMode = CULL_MODE_NONE;
			desc.FrontCounterClockwise = false;
			desc.DepthBias = 0;
			desc.DepthBiasClamp = 0.0f;
			desc.SlopeScaledDepthBias = 0.0f;
			desc.DepthClipEnable = true;
			desc.ScissorEnable = false;
			desc.MultisampleEnable = false;
			desc.AntialiasedLineEnable = false;
			return renderer.CreateRasterizerState(desc);
		}
		default:
			Logger::Log(FB_ERROR_LOG_ARG, FormatString("Resource Provider - Unknown rasterizer states (%d)", ResourceType_RasterizerStates).c_str());
			return 0;
		}
	}

	RasterizerStatePtr GetRasterizerState(int ResourceType_RasterizerStates){
		auto it = mRasterizerStates.Find(ResourceType_RasterizerStates);
		if (it == mRasterizerStates.end()){
			auto rasterizerState = CreateRasterizerState(ResourceType_RasterizerStates);
			if (!rasterizerState){
				Logger::Log(FB_ERROR_LOG_ARG, "Resource Provider - Failed to create a rasterizer state(%d)", ResourceType_RasterizerStates);
				return 0;
			}
			mRasterizerStates[ResourceType_RasterizerStates] = rasterizerState;
			return rasterizerState;
		}

		return it->second;
	}

	BlendStatePtr CreateBlendState(int ResourceType_BlendStates){
		auto& renderer = Renderer::GetInstance();
		BLEND_DESC desc;
		switch (ResourceType_BlendStates){
		case ResourceTypes::BlendStates::Default:{
			return renderer.CreateBlendState(desc);
		}
		case ResourceTypes::BlendStates::Additive:{
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].BlendOp = BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlend = BLEND_ONE;
			desc.RenderTarget[0].DestBlend = BLEND_ONE;
			return renderer.CreateBlendState(desc);
		}
		case ResourceTypes::BlendStates::AlphaBlend:{
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].BlendOp = BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlend = BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = BLEND_INV_SRC_ALPHA;
			return renderer.CreateBlendState(desc);
		}
		case ResourceTypes::BlendStates::MaxBlend:{
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].BlendOp = BLEND_OP_MAX;
			desc.RenderTarget[0].SrcBlend = BLEND_ONE;
			desc.RenderTarget[0].DestBlend = BLEND_ONE;
			return renderer.CreateBlendState(desc);
		}
		case ResourceTypes::BlendStates::RedAlphaMask:{
			desc.RenderTarget[0].RenderTargetWriteMask = COLOR_WRITE_MASK_RED | COLOR_WRITE_MASK_ALPHA;
			return renderer.CreateBlendState(desc);
		}
		case ResourceTypes::BlendStates::GreenAlphaMask:{
			desc.RenderTarget[0].RenderTargetWriteMask = COLOR_WRITE_MASK_GREEN | COLOR_WRITE_MASK_ALPHA;
			return renderer.CreateBlendState(desc);
		}
		case ResourceTypes::BlendStates::GreenAlphaMaskMax:{
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].BlendOp = BLEND_OP_MAX;
			desc.RenderTarget[0].SrcBlend = BLEND_ONE;
			desc.RenderTarget[0].DestBlend = BLEND_ONE;
			desc.RenderTarget[0].RenderTargetWriteMask = COLOR_WRITE_MASK_GREEN | COLOR_WRITE_MASK_ALPHA;
			return renderer.CreateBlendState(desc);
		}
		case ResourceTypes::BlendStates::RedAlphaMaskAddMinus:{
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].BlendOp = BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlend = BLEND_ONE;
			desc.RenderTarget[0].DestBlend = BLEND_ONE;

			desc.RenderTarget[0].BlendOpAlpha = BLEND_OP_REV_SUBTRACT;
			desc.RenderTarget[0].SrcBlendAlpha = BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha = BLEND_ONE;
			desc.RenderTarget[0].RenderTargetWriteMask = COLOR_WRITE_MASK_RED | COLOR_WRITE_MASK_ALPHA;
			return renderer.CreateBlendState(desc);
		}
		case ResourceTypes::BlendStates::GreenAlphaMaskAddAdd:{
			BLEND_DESC bdesc;
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].BlendOp = BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlend = BLEND_ONE;
			desc.RenderTarget[0].DestBlend = BLEND_ONE;

			desc.RenderTarget[0].BlendOpAlpha = BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlendAlpha = BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha = BLEND_ONE;
			desc.RenderTarget[0].RenderTargetWriteMask = COLOR_WRITE_MASK_GREEN | COLOR_WRITE_MASK_ALPHA;
			return renderer.CreateBlendState(desc);
		}
		case ResourceTypes::BlendStates::RedAlphaMaskAddAdd:{
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].BlendOp = BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlend = BLEND_ONE;
			desc.RenderTarget[0].DestBlend = BLEND_ONE;

			desc.RenderTarget[0].BlendOpAlpha = BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlendAlpha = BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha = BLEND_ONE;
			desc.RenderTarget[0].RenderTargetWriteMask = COLOR_WRITE_MASK_RED | COLOR_WRITE_MASK_ALPHA;
			return renderer.CreateBlendState(desc);
		}

		case ResourceTypes::BlendStates::GreenAlphaMaskAddMinus:{
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].BlendOp = BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlend = BLEND_ONE;
			desc.RenderTarget[0].DestBlend = BLEND_ONE;

			desc.RenderTarget[0].BlendOpAlpha = BLEND_OP_REV_SUBTRACT;
			desc.RenderTarget[0].SrcBlendAlpha = BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha = BLEND_ONE;
			desc.RenderTarget[0].RenderTargetWriteMask = COLOR_WRITE_MASK_GREEN | COLOR_WRITE_MASK_ALPHA;
			return renderer.CreateBlendState(desc);
		}
		case ResourceTypes::BlendStates::GreenMask:{
			desc.RenderTarget[0].RenderTargetWriteMask = COLOR_WRITE_MASK_GREEN;
			return renderer.CreateBlendState(desc);
		}
		case ResourceTypes::BlendStates::BlueMask:{
			desc.RenderTarget[0].RenderTargetWriteMask = COLOR_WRITE_MASK_BLUE;
			return renderer.CreateBlendState(desc);
		}
		case ResourceTypes::BlendStates::NoColorWrite:{
			desc.RenderTarget[0].RenderTargetWriteMask = 0;
			return renderer.CreateBlendState(desc);
		}
		default:
			Logger::Log(FB_ERROR_LOG_ARG, FormatString("Resource Provider - Unknown blend states (%d)", ResourceType_BlendStates).c_str());
			return 0;
		}
	}

	BlendStatePtr GetBlendState(int ResourceType_BlendStates){
		auto it = mBlendStates.Find(ResourceType_BlendStates);
		if (it == mBlendStates.end()){
			auto state = CreateBlendState(ResourceType_BlendStates);
			if (!state){
				Logger::Log(FB_ERROR_LOG_ARG, "Resource Provider - Failed to create a rasterizer state(%d)", ResourceType_BlendStates);
				return 0;
			}
			mBlendStates[ResourceType_BlendStates] = state;
			return state;
		}

		return it->second;
	}

	DepthStencilStatePtr CreateDepthStencilState(int ResourceType_DepthStencilStates){
		auto& renderer = Renderer::GetInstance();
		DEPTH_STENCIL_DESC desc;
		switch (ResourceType_DepthStencilStates){
		case ResourceTypes::DepthStencilStates::Default:{
			return renderer.CreateDepthStencilState(desc);
		}
		case ResourceTypes::DepthStencilStates::NoDepthStencil:{
			desc.DepthEnable = false;
			desc.DepthWriteMask = DEPTH_WRITE_MASK_ZERO;
			return renderer.CreateDepthStencilState(desc);
		}
		case ResourceTypes::DepthStencilStates::NoDepthWrite_LessEqual:{
			desc.DepthWriteMask = DEPTH_WRITE_MASK_ZERO;
			desc.DepthFunc = COMPARISON_LESS_EQUAL;
			return renderer.CreateDepthStencilState(desc);
		}
		case ResourceTypes::DepthStencilStates::LessEqual:{
			desc.DepthFunc = COMPARISON_LESS_EQUAL;
			return renderer.CreateDepthStencilState(desc);
		}
		default:
			Logger::Log(FB_ERROR_LOG_ARG, FormatString("Resource Provider - Unknown depth stencil states (%d)", ResourceType_DepthStencilStates).c_str());
			return 0;
		}
	}

	DepthStencilStatePtr GetDepthStencilState(int ResourceType_DepthStencilStates){
		auto it = mDepthStencilStates.Find(ResourceType_DepthStencilStates);
		if (it == mDepthStencilStates.end()){
			auto state = CreateDepthStencilState(ResourceType_DepthStencilStates);
			if (!state){
				Logger::Log(FB_ERROR_LOG_ARG, "Resource Provider - Failed to create a depth stencil state(%d)", ResourceType_DepthStencilStates);
				return 0;
			}
			mDepthStencilStates[ResourceType_DepthStencilStates] = state;
			return state;
		}

		return it->second;
	}

	SamplerStatePtr CreateSamplerState(int ResourceType_SamplerStates){
		auto& renderer = Renderer::GetInstance();
		SAMPLER_DESC desc;
		switch (ResourceType_SamplerStates){
		case ResourceTypes::SamplerStates::Point:
		{
			desc.Filter = TEXTURE_FILTER_MIN_MAG_MIP_POINT;
			return renderer.CreateSamplerState(desc);
		}
		case ResourceTypes::SamplerStates::Linear:
		{
			desc.Filter = TEXTURE_FILTER_MIN_MAG_MIP_LINEAR;
			return renderer.CreateSamplerState(desc);
		}
		case ResourceTypes::SamplerStates::Anisotropic:
		{
			desc.Filter = TEXTURE_FILTER_ANISOTROPIC;
			return renderer.CreateSamplerState(desc);
		}
		case ResourceTypes::SamplerStates::Shadow:
		{
			desc.Filter = TEXTURE_FILTER_COMPARISON_ANISOTROPIC;
			desc.AddressU = TEXTURE_ADDRESS_BORDER;
			desc.AddressV = TEXTURE_ADDRESS_BORDER;
			desc.AddressW = TEXTURE_ADDRESS_BORDER;
			for (int i = 0; i < 4; i++)
				desc.BorderColor[i] = 1.0f;
			desc.ComparisonFunc = COMPARISON_LESS_EQUAL;
			return renderer.CreateSamplerState(desc);
		}
		case ResourceTypes::SamplerStates::PointWrap:
		{
			desc.Filter = TEXTURE_FILTER_MIN_MAG_MIP_POINT;
			desc.AddressU = TEXTURE_ADDRESS_WRAP;
			desc.AddressV = TEXTURE_ADDRESS_WRAP;
			desc.AddressW = TEXTURE_ADDRESS_WRAP;
			return renderer.CreateSamplerState(desc);
		}
		case ResourceTypes::SamplerStates::LinearWrap:
		{
			desc.Filter = TEXTURE_FILTER_MIN_MAG_MIP_LINEAR;
			desc.AddressU = TEXTURE_ADDRESS_WRAP;
			desc.AddressV = TEXTURE_ADDRESS_WRAP;
			desc.AddressW = TEXTURE_ADDRESS_WRAP;			
			return renderer.CreateSamplerState(desc);
		}
		case ResourceTypes::SamplerStates::BlackBorder:
		{
			desc.Filter = TEXTURE_FILTER_MIN_MAG_MIP_LINEAR;
			desc.AddressU = TEXTURE_ADDRESS_BORDER;
			desc.AddressV = TEXTURE_ADDRESS_BORDER;
			desc.AddressW = TEXTURE_ADDRESS_BORDER;
			for (int i = 0; i < 4; i++)
				desc.BorderColor[i] = 0;
			return renderer.CreateSamplerState(desc);
		}
		case ResourceTypes::SamplerStates::PointBlackBorder:
		{
			desc.Filter = TEXTURE_FILTER_MIN_MAG_MIP_POINT;
			desc.AddressU = TEXTURE_ADDRESS_BORDER;
			desc.AddressV = TEXTURE_ADDRESS_BORDER;
			desc.AddressW = TEXTURE_ADDRESS_BORDER;
			for (int i = 0; i < 4; i++)
				desc.BorderColor[i] = 0;
			return renderer.CreateSamplerState(desc);
		}
		default:
			Logger::Log(FB_ERROR_LOG_ARG, FormatString("Resource Provider - Unknown sampler states (%d)", ResourceType_SamplerStates).c_str());
			return 0;
		}
	}

	SamplerStatePtr GetSamplerState(int ResourceType_SamplerStates){
		auto it = mSamplerStates.Find(ResourceType_SamplerStates);
		if (it == mSamplerStates.end()){
			auto state = CreateSamplerState(ResourceType_SamplerStates);
			if (!state){
				Logger::Log(FB_ERROR_LOG_ARG, "Resource Provider - Failed to create a depth stencil state(%d)", ResourceType_SamplerStates);
				return 0;
			}
			mSamplerStates[ResourceType_SamplerStates] = state;
			return state;
		}

		return it->second;
	}

	int GetNumToneMaps() const{
		return NumToneMaps;
	}

	int GetNumLuminanceMaps() const{
		return NumLuminanceMaps;
	}
};