#include "stdafx.h"
#include "Material.h"
#include "Shader.h"
#include "InputLayout.h"
#include "RenderStates.h"
#include "ColorRamp.h"
#include "Renderer.h"
#include "Texture.h"
#include "EssentialEngineData/shaders/Constants.h"
#include "FBCommonHeaders/VectorMap.h"
#include "FBCommonHeaders/CowPtr.h"


using namespace fastbird;
class Material::Impl{
public:
	struct TextureBinding{
		BINDING_SHADER mShader;
		int mSlot;
	};
	struct TextureSignature
	{
		TextureSignature(TEXTURE_TYPE type, const char* filepath, const ColorRamp* cr)
			:mType(type), mColorRamp(cr)
		{
			if (filepath)
				mFilepath = filepath;
		}
		TEXTURE_TYPE mType;
		std::string mFilepath;
		const ColorRamp* mColorRamp;
	};
	typedef std::map<TexturePtr, ColorRamp> COLOR_RAMP_MAP_TYPE;

	std::mutex mMutex;

	//---------------------------------------------------------------------------
	// Data - Copy On Write.
	//---------------------------------------------------------------------------
	struct SharedData {
		SharedData()
			: mShaders(0)
			, mRenderPass(RENDER_PASS::PASS_NORMAL)
			, mReloading(false)
			, mReloadingTryCount(0)
		{
		}
		std::string mName;
		INPUT_ELEMENT_DESCS mInputElementDescs;
		InputLayoutPtr mInputLayout;
		int mShaders; // combination of enum BINDING_SHADER;
		std::vector<MaterialPtr> mSubMaterials;
		RENDER_PASS mRenderPass;
		bool mReloading;
		int mReloadingTryCount;
	};

	struct MaterialData{
		MaterialData() {
			memset(&mMaterialConstants, 0, sizeof(mMaterialConstants);
		}
		MATERIAL_CONSTANTS mMaterialConstants;
		typedef VectorMap<unsigned, Vec4> PARAMETER_VECTOR;
		PARAMETER_VECTOR mMaterialParameters;
		std::vector< TexturePtr > mTextures;
		VectorMap<TextureBinding, TexturePtr> mTexturesByBinding;
		COLOR_RAMP_MAP_TYPE mColorRampMap;		
	};

	struct RenderStatesData{
		RenderStatesData()
			: mTransparent(false)
			, mGlow(false)
			, mNoShadowCast(false), mDoubleSided(false)
		{
		}
		CowPtr<RenderStatesPtr> mRenderStates;
		bool mTransparent;
		bool mGlow;
		bool mNoShadowCast;
		bool mDoubleSided;
	};

	struct ShaderData{
		std::string mShaderFile;
		SHADER_DEFINES mShaderDefines;
		ShaderPtr mShader;
	};

	std::shared_ptr<SharedData> mSharedData;
	CowPtr<MaterialData> mMaterialData;
	CowPtr<RenderStatesData> mRenderStatesData;
	CowPtr<ShaderData> mShaderData;


	//---------------------------------------------------------------------------
	Impl(){
	}

	Impl(const Impl& other)
		: mSharedData(other.mSharedData)
		, mMaterialData(other.mMaterialData)
		, mRenderStatesData(other.mRenderStatesData)
		, mShaderData(other.mShaderData)
	{
	}

	TexturePtr CreateColorRampTexture(ColorRamp& cr){
		cr.GenerateColorRampTextureData(128);
		char imgBuf[128];
		unsigned int *pixels = (unsigned int *)imgBuf;
		for (unsigned x = 0; x < 128; x++)
		{
			pixels[127 - x] = cr[x].Get4Byte();
		}
		auto renderer = Renderer::GetInstance();
		TexturePtr pTexture = renderer->CreateTexture(pixels, 128, 1, PIXEL_FORMAT_R8G8B8A8_UNORM,
			BUFFER_USAGE_DYNAMIC, BUFFER_CPU_ACCESS_WRITE, TEXTURE_TYPE_DEFAULT); // default is right.
		mMaterialData->mColorRampMap.insert(COLOR_RAMP_MAP_TYPE::value_type(pTexture, cr));
		return pTexture;
	}

	

	bool FindTextureIn(BINDING_SHADER shader, int slot, TexturePtr& pTextureInTheSlot,
		// additional parameters. if match this function returns true.
		TextureSignature* pSignature = 0) const{
		if (pTextureInTheSlot){
			Logger::Log(FB_ERROR_LOG_ARG, "Invalid arg.");
			return false;
		}
		TextureBinding binding{ shader, slot };
		auto it = mTexturesByBinding.Find(binding);
		if (it != mTexturesByBinding.end()){
			pTextureInTheSlot = it->second;
		}		
		if (!pTextureInTheSlot)
			return false;

		if (pSignature)
		{
			pTextureInTheSlot;
			if (pSignature->mType != TEXTURE_TYPE_COUNT &&
				pSignature->mType != pTextureInTheSlot->GetType())
				return false;

			if (pSignature->mFilepath != pTextureInTheSlot->GetName())
				return false;

			if (pSignature->mColorRamp)
			{
				auto itFound = mColorRampMap.find(pTextureInTheSlot);
				if (itFound == mColorRampMap.end() ||
					!(itFound->second == *pSignature->mColorRamp))
				{
					return false; // not same
				}
			}
		}

		return true; // same
	}

	void RemoveInstance(Material* pInstance);
};


Material::Material()
	: mImpl(new Impl)
{
}