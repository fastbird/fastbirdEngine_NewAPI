#include "stdafx.h"
#include "SkySphere.h"
#include "FBRenderer/RenderStates.h"
#include "FBRenderer/RenderTarget.h"
#include "FBRenderer/Renderer.h"
#include "FBRenderer/ResourceProvider.h"
#include "FBRenderer/Material.h"
#include "FBRenderer/Texture.h"
#include "FBSceneManager/Scene.h"
#include "FBSceneManager/DirectionalLight.h"
#include "FBSceneManager/Camera.h"
#include "SceneObjectFactory.h"

using namespace fastbird;
static RenderTargetPtr sRT;
static ScenePtr sScene;
static const int ENV_SIZE = 1024;
class SkySphere::Impl{
public:
	SkySphere* mSelf;
	SkySphereWeakPtr mSelfPtr;
	MaterialPtr mMaterial;
	MaterialPtr mMaterialOCC;
	VertexBufferPtr mVB;
	IndexBufferPtr mIB;
	Vec4 mMaterialParamCur[5];
	Vec4 mMaterialParamDest[5];
	float mCurInterpolationTime;
	float mInterpolationTime;
	bool mInterpolating;
	bool mUseAlphaBlend;
	
	float mAlpha;
	static const unsigned NumConsts = 7;
	Vec4 mIrradConsts[NumConsts];
	Vec4 mIrradCoeff[9];
	unsigned mLastPreRendered;
	

	Impl(SkySphere* self)
		: mSelf(self)
		, mCurInterpolationTime(0)
		, mInterpolating(false)
		, mUseAlphaBlend(false)
		, mAlpha(1.0f)
		, mLastPreRendered(0)
	{
	}

	// IRenderable
	void SetMaterial(const char* filepath, int pass){
		auto material = Renderer::GetInstance().CreateMaterial(filepath);		
		if (material)
		{
			if (pass == RENDER_PASS::PASS_NORMAL)
				mMaterial = material;
			else if (pass == RENDER_PASS::PASS_GODRAY_OCC_PRE)
				mMaterialOCC = material;
			else{
				Logger::Log(FB_ERROR_LOG_ARG, "You can set sky mateirl only for NORMAL AND GODRAY_OCC Pass.");
				assert(0);
			}
		}
	}

	void SetMaterial(MaterialPtr pMat, int pass){
		if (pass == RENDER_PASS::PASS_NORMAL)
			mMaterial = pMat;
		else if (pass == RENDER_PASS::PASS_GODRAY_OCC_PRE)
			mMaterialOCC = pMat;
		else{
			Logger::Log(FB_ERROR_LOG_ARG, "You can set sky mateirl only for NORMAL AND GODRAY_OCC Pass.");
			assert(0);
		}
	}

	MaterialPtr GetMaterial(int pass = 0) const{
		if (pass == RENDER_PASS::PASS_NORMAL)
			return mMaterial;
		else if (pass == RENDER_PASS::PASS_GODRAY_OCC_PRE)
			return mMaterialOCC;
		else{
			Logger::Log(FB_ERROR_LOG_ARG, "You can get sky mateirls only for NORMAL AND GODRAY_OCC Pass.");
			assert(0);
		}

		return 0;
	}

	void SetVertexBuffer(VertexBufferPtr pVertexBuffer){
		Logger::Log(FB_ERROR_LOG_ARG, "You cannot set vertex buffer for SkySphere");
	}

	void SetIndexBuffer(IndexBufferPtr pIndexBuffer){
		Logger::Log(FB_ERROR_LOG_ARG, "You cannot set index buffer for SkySphere");
	}

	// override the input layout defined in material
	void SetInputLayout(InputLayoutPtr i){
		Logger::Log(FB_ERROR_LOG_ARG, "You cannot set input layout for SkySphere");
	}

	void PreRender(const RenderParam& param, RenderParamOut* paramOut){
		if (mSelf->HasObjFlag(SceneObjectFlag::Hide))
			return;

		if (mLastPreRendered == gpTimer->GetFrame())
			return;
		mLastPreRendered = gpTimer->GetFrame();

		if (mInterpolating)
		{
			mCurInterpolationTime += gpTimer->GetDeltaTime();
			float normTime = mCurInterpolationTime / mInterpolationTime;
			if (normTime >= 1.0f)
			{
				normTime = 1.0f;
				mInterpolating = false;
				if (!mUseAlphaBlend)
				{
					SceneObjectFactory::GetInstance().UpdateEnvMapInNextFrame(mSelfPtr.lock());
				}
			}

			for (int i = 0; i < 5; i++)
			{
				mMaterial->SetMaterialParameters(i, Lerp(mMaterialParamCur[i], mMaterialParamDest[i], normTime));
				if (mMaterialOCC)
				{
					mMaterialOCC->SetMaterialParameters(i, Lerp(mMaterialParamCur[i], mMaterialParamDest[i], normTime));
				}
			}
		}
	}

	void Render(const RenderParam& param, RenderParamOut* paramOut){
		if (mSelf->HasObjFlag(SceneObjectFlag::Hide))
			return;

		if (!mMaterial)
		{
			assert(0);
			return;
		}

		auto& renderer = Renderer::GetInstance();
		renderer.SetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		renderer.UnbindInputLayout();
		renderer.UnbindVertexBuffers();
		if (mMaterialOCC && param.mRenderPass == RENDER_PASS::PASS_GODRAY_OCC_PRE)
		{
			RenderEventMarker mark("SkySphere_OCC");
			mMaterialOCC->Bind(false);
			renderer.Draw(3, 0);
		}
		else if (param.mRenderPass == RENDER_PASS::PASS_NORMAL)
		{
			RenderEventMarker mark("SkySphere");			
			mMaterial->Bind(false);
			if (mUseAlphaBlend)
			{
				renderer.GetResourceProvider()->BindBlendState(ResourceTypes::BlendStates::AlphaBlend);
			}
			renderer.Draw(3, 0);
		}
	}

	void PostRender(const RenderParam& param, RenderParamOut* paramOut){
		
	}

	void SetEnableHighlight(bool enable){		
	}

	// SkySphere
	Vec3 NormalFromCubePixelCoord(int face, int w, int h, float halfWidth)
	{
		Vec3 n;
		switch (face)
		{
		case 0:
			n.x = halfWidth;
			n.y = halfWidth - w;
			n.z = halfWidth - h;
			break;
		case 1:
			n.x = -halfWidth;
			n.y = w - halfWidth;
			n.z = halfWidth - h;
			break;
		case 2: // up
			n.x = w - halfWidth;
			n.y = h - halfWidth;
			n.z = halfWidth;
			break;
		case 3: // down
			n.x = w - halfWidth;
			n.y = halfWidth - h;
			n.z = -halfWidth;
			break;
		case 4: // front
			n.x = w - halfWidth;
			n.y = halfWidth;
			n.z = halfWidth - h;
			break;
		case 5: // back
			n.x = halfWidth - w;
			n.y = -halfWidth;
			n.z = halfWidth - h;
			break;
		}
		return n.NormalizeCopy();
	}

	void GenerateRadianceCoef(TexturePtr pTex)
	{
		int maxLod = (int)log2((float)ENV_SIZE);

		const float basisCoef[5] = { 0.282095f,
			0.488603f,
			1.092548f,
			0.315392f,
			0.546274f };

		const int usingLod = 2; // if ENV_SIZE == 1024, we are using 256 size.
		const int width = (int)(ENV_SIZE / pow(2, usingLod));
		float halfWidth = width / 2.f;
		auto& renderer = Renderer::GetInstance();
		TexturePtr pStaging = renderer.CreateTexture(0, width, width, PIXEL_FORMAT_R8G8B8A8_UNORM,
			BUFFER_USAGE_STAGING, BUFFER_CPU_ACCESS_READ, TEXTURE_TYPE_CUBE_MAP);
		for (int i = 0; i < 6; i++)
		{
			unsigned subResource = i * (maxLod + 1) + usingLod;
			pTex->CopyToStaging(pStaging, i, 0, 0, 0, subResource, 0);
		}
		pStaging->SaveToFile("envSub.dds");

		// prefiltering
		float lightCoef[3][9];
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				lightCoef[i][j] = 0;
			}
		}

		for (int i = 0; i < 6; i++)
		{
			unsigned subResource = i;
			MapData data = pStaging->Map(subResource, MAP_TYPE_READ, MAP_FLAG_NONE);
			if (data.pData)
			{
				DWORD* colors = (DWORD*)data.pData;
				DWORD* row, *pixel;
				for (int h = 0; h < width; h++)
				{
					row = colors + width * h;
					for (int w = 0; w < width; w++)
					{

						Vec3 n = NormalFromCubePixelCoord(i, w, h, halfWidth);

						pixel = row + w;
						Color::RGBA* rgba256 = (Color::RGBA*)pixel;
						Vec3 rgb(rgba256->r / 255.0f, rgba256->g / 255.0f, rgba256->b / 255.0f);
						// 00
						lightCoef[0][0] += rgb.x * basisCoef[0];
						lightCoef[1][0] += rgb.y * basisCoef[0];
						lightCoef[2][0] += rgb.z * basisCoef[0];

						// 1-1
						float yC = (basisCoef[1] * n.y);
						lightCoef[0][1] += rgb.x * yC;
						lightCoef[1][1] += rgb.y * yC;
						lightCoef[2][1] += rgb.z * yC;

						// 10
						yC = (basisCoef[1] * n.z);
						lightCoef[0][2] += rgb.x * yC;
						lightCoef[1][2] += rgb.y * yC;
						lightCoef[2][2] += rgb.z * yC;

						// 11
						yC = (basisCoef[1] * n.x);
						lightCoef[0][3] += rgb.x * yC;
						lightCoef[1][3] += rgb.y * yC;
						lightCoef[2][3] += rgb.z * yC;

						// 2-2
						yC = (basisCoef[2] * n.x*n.y);
						lightCoef[0][4] += rgb.x * yC;
						lightCoef[1][4] += rgb.y * yC;
						lightCoef[2][4] += rgb.z * yC;

						// 2-1
						yC = (basisCoef[2] * n.y*n.z);
						lightCoef[0][5] += rgb.x * yC;
						lightCoef[1][5] += rgb.y * yC;
						lightCoef[2][5] += rgb.z * yC;

						// 20
						yC = (basisCoef[3] * (3.0f*n.z*n.z - 1.f));
						lightCoef[0][6] += rgb.x * yC;
						lightCoef[1][6] += rgb.y * yC;
						lightCoef[2][6] += rgb.z * yC;

						// 21
						yC = (basisCoef[2] * (n.x*n.z));
						lightCoef[0][7] += rgb.x * yC;
						lightCoef[1][7] += rgb.y * yC;
						lightCoef[2][7] += rgb.z * yC;

						// 22
						yC = (basisCoef[4] * (n.x*n.x - n.y*n.y));
						lightCoef[0][8] += rgb.x * yC;
						lightCoef[1][8] += rgb.y * yC;
						lightCoef[2][8] += rgb.z * yC;
					}
				}
			}
			pStaging->Unmap(subResource);
		}

		float avg = 1.0f / (width*width * 6);
		for (int i = 0; i < 9; i++)
		{
			lightCoef[0][i] *= avg;
			lightCoef[1][i] *= avg;
			lightCoef[2][i] *= avg;
			mIrradCoeff[i] = Vec4(lightCoef[0][i], lightCoef[1][i], lightCoef[2][i], 1);
		}

		//const float sqrtPi = sqrtf(PI);
		//const float fC[5] = { 1.f / (2.f*sqrtPi),
		//	sqrt(3.0f) / (3.f*sqrtPi),
		//	sqrt(15.f) / (8.f*sqrtPi),
		//	sqrt(5.f) / (16.0f*sqrtPi),
		//	0.5f * (sqrt(15.f) / (8.f*sqrtPi)),
		//};

		//Vec4 vLightYCoeff[3];
		//for (int ic = 0; ic < 3; ic++)
		//{
		//	vLightYCoeff[ic].x = -fC[1] * lightCoef[ic][3];
		//	vLightYCoeff[ic].y = -fC[1] * lightCoef[ic][1];
		//	vLightYCoeff[ic].z = fC[1] * lightCoef[ic][2];
		//	vLightYCoeff[ic].w = fC[0] * lightCoef[ic][0] - fC[3] * lightCoef[ic][6];
		//}

		//mIrradConsts[0] = vLightYCoeff[0]; // Ar
		//mIrradConsts[1] = vLightYCoeff[1]; // Ag
		//mIrradConsts[2] = vLightYCoeff[2]; // Ab

		//for (int ic = 0; ic < 3; ic++)
		//{
		//	vLightYCoeff[ic].x = fC[2] * lightCoef[ic][4];
		//	vLightYCoeff[ic].y = -fC[2] * lightCoef[ic][5];
		//	vLightYCoeff[ic].z = 3.0f * fC[3] * lightCoef[ic][6];
		//	vLightYCoeff[ic].w = -fC[2] * lightCoef[ic][7];
		//}

		//mIrradConsts[3] = vLightYCoeff[0]; // Br
		//mIrradConsts[4] = vLightYCoeff[1]; // Bg
		//mIrradConsts[5] = vLightYCoeff[2]; // Bb

		//mIrradConsts[6].x = fC[4] * lightCoef[0][8];
		//mIrradConsts[6].y = fC[4] * lightCoef[1][8];
		//mIrradConsts[6].z = fC[4] * lightCoef[2][8];
		//mIrradConsts[6].w = 1.0f;
	}

	void UpdateEnvironmentMap(const Vec3& origin){
		if (!sRT)
		{
			Logger::Log(FB_ERROR_LOG_ARG, "No render target found.");
			return;
		}

		auto& renderer = Renderer::GetInstance();
		TexturePtr pTexture = sRT->GetRenderTargetTexture();
		auto dest = sRT->GetScene()->GetDirectionalLight(0);
		auto src = renderer.GetMainRenderTarget()->GetScene()->GetDirectionalLight(0);
		dest->CopyLight(src);

		sRT->GetScene()->AttachSkySphere(mSelfPtr.lock());
		sRT->GetCamera()->SetPosition(origin);
		sRT->GetCamera()->SetFOV(HALF_PI);
		sRT->GetCamera()->SetAspectRatio(1.0f);
		Vec3 dirs[] = {
			Vec3(1, 0, 0), Vec3(-1, 0, 0),
			Vec3(0, 0, 1), Vec3(0, 0, -1),
			Vec3(0, 1, 0), Vec3(0, -1, 0),
		};
		for (int i = 0; i < 6; i++)
		{
			sRT->GetCamera()->SetDirection(dirs[i]);
			sRT->Render(i);
		}
		// this is for unbind the environment map from the output slot.
		renderer.GetMainRenderTarget()->BindTargetOnly(false);

		pTexture->GenerateMips();
		//pTexture->SaveToFile("environment.dds");
		// for bight test.
		//ITexture* textureFile = gFBEnv->pRenderer->CreateTexture("data/textures/brightEnv.jpg");
		GenerateRadianceCoef(pTexture);
		renderer.SetEnvironmentTexture(pTexture);
		renderer.UpdateRadConstantsBuffer(mIrradCoeff);
		sRT->GetScene()->DetachSkySphere();
	}

	void SetInterpolationData(unsigned index, const Vec4& data){
		assert(index < 5);
		if (index == 2)
		{
			mMaterialParamDest[index] = data;
			mMaterialParamDest[index].w = mAlpha;
		}
		else
		{
			mMaterialParamDest[index] = data;
		}
	}

	void PrepareInterpolation(float time, SkySpherePtr startFrom){
		MaterialPtr srcMaterial = mMaterial;
		if (startFrom)
			srcMaterial = startFrom->GetMaterial();
		for (int i = 0; i < 5; i++)
		{
			mMaterialParamCur[i] = srcMaterial->GetMaterialParameter(i);
		}

		mInterpolationTime = time;
		mCurInterpolationTime = 0;
		mInterpolating = true;
	}

	void SetUseAlphaBlend(bool use){
		mUseAlphaBlend = use;
	}

	void SetAlpha(float alpha){
		Vec4 param = mMaterial->GetMaterialParameter(3);
		mAlpha = param.w = alpha;
		mMaterial->SetMaterialParameters(3, param);
		mMaterialParamDest[3].w = alpha;
		mMaterialParamDest[2].w = alpha;
	}

	float GetAlpha() const{
		return mAlpha;
	}
};

//---------------------------------------------------------------------------
void SkySphere::CreateSharedEnvRT(){
	if (!sRT)
	{
		RenderTargetParam param;
		param.mEveryFrame = false;
		param.mSize = Vec2I(ENV_SIZE, ENV_SIZE);
		param.mPixelFormat = PIXEL_FORMAT_R8G8B8A8_UNORM;
		param.mShaderResourceView = true;
		param.mMipmap = true;
		param.mCubemap = true;
		param.mWillCreateDepth = false;
		param.mUsePool = true;
		auto& renderer = Renderer::GetInstance();
		sRT = renderer.CreateRenderTarget(param);
		sScene = Scene::Create("SkySphereScene");
		sRT->RegisterScene(sScene);

		sRT->GetRenderPipeline().SetMinimum();
	}
}
void SkySphere::DestroySharedEnvRT(){

}

IMPLEMENT_STATIC_CREATE(SkySphere);
SkySphere::SkySphere()
	: mImpl(new Impl)
{

}
