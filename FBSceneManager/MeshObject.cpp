/*
 -----------------------------------------------------------------------------
 This source file is part of fastbird engine
 For the latest info, see http://www.jungwan.net/
 
 Copyright (c) 2013-2015 Jungwan Byun
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "MeshObject.h"
#include "Animation.h"
#include "AnimationData.h"
#include "SceneManager.h"
#include "Camera.h"
#include "FBRenderer/Renderer.h"
#include "FBRenderer/RenderOptions.h"
#include "FBRenderer/RenderStates.h"
#include "FBRenderer/VertexBuffer.h"
#include "FBRenderer/IndexBuffer.h"
#include "FBRenderer/Material.h"
#include "FBRenderer/RenderTarget.h"
#include "FBStringLib/StringLib.h"
#include "../EssentialEngineData/shaders/Constants.h"
using namespace fastbird;

class MeshObject::Impl{
public:
	struct MaterialGroup
	{
		MaterialPtr mMaterial;
		MaterialPtr mForceAlphaMaterial;
		VertexBufferPtr mVBPos;
		VertexBufferPtr mVBNormal;
		VertexBufferPtr mVBUV;
		VertexBufferPtr mVBColor;
		VertexBufferPtr mVBTangent;
		IndexBufferPtr mIndexBuffer;
		std::vector<Vec3> mPositions;
		std::vector<Vec3> mNormals;
		std::vector<Vec2> mUVs;
		std::vector<ModelTriangle> mTriangles;
		std::vector<DWORD> mColors;
		std::vector<Vec3> mTangents;
	};

	MeshObjectWeakPtr mSelf;
	InputLayoutPtr mInputLayoutOverride;
	PRIMITIVE_TOPOLOGY mTopology;
	OBJECT_CONSTANTS mObjectConstants;
	POINT_LIGHT_CONSTANTS mPointLightConstants;
	// if you have only one MaterialGroup,
	// this vector will not be used.
	std::vector<MaterialGroup> mMaterialGroups; // can call it as SubMeshes.

	AUXILIARIES mAuxil;
	AUXILIARIES* mAuxCloned;
	bool mModifying;
	bool mRenderHighlight;
	RasterizerStatePtr mHighlightRasterizeState;

	typedef std::vector< FBCollisionShapePtr > COLLISION_SHAPES;
	COLLISION_SHAPES mCollisions;
	COLLISION_SHAPES* mCollisionsCloned;
	BoundingVolumePtr mAABB;
	FRAME_PRECISION mLastPreRendered;

	bool mUseDynamicVB[BUFFER_TYPE_NUM];
	bool mForceAlphaBlending;

	//---------------------------------------------------------------------------
	Impl()
		: mAuxCloned(0)
		, mRenderHighlight(false)
		, mCollisionsCloned(0)
		, mForceAlphaBlending(false)
		, mLastPreRendered(0)
	{
		mTopology = PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		mObjectConstants.gWorld.MakeIdentity();
		auto& renderer = Renderer::GetInstance();
		SetMaterial(renderer.GetMissingMaterial(), 0);
		for (int i = 0; i < BUFFER_TYPE_NUM; ++i)
		{
			mUseDynamicVB[i] = false;
		}
		mAABB = BoundingVolume::Create(BoundingVolume::BV_AABB);
	}

	~Impl()
	{
		DeleteCollisionShapes();
	}

	void SetMaterial(const char* name, int pass){
		CreateMaterialGroupFor(0);
		auto& renderer = Renderer::GetInstance();
		mMaterialGroups[0].mMaterial = renderer.CreateMaterial(name);		
	}
	
	void SetMaterial(MaterialPtr pMat, int pass){
		CreateMaterialGroupFor(0);
		mMaterialGroups[0].mMaterial = pMat;
	}
	
	MaterialPtr GetMaterial(int pass) const{
		if (!mMaterialGroups.empty())
		{
			return mForceAlphaBlending ? mMaterialGroups[0].mForceAlphaMaterial : mMaterialGroups[0].mMaterial;
		}
	}
	
	void SetVertexBuffer(VertexBufferPtr pVertexBuffer){		
		Logger::Log(FB_ERROR_LOG_ARG, "You cannot directly set a vertex buffer to a mesh object.\
									  Use StartModification() / EndModification() functino.");
	}
	
	void SetIndexBuffer(IndexBufferPtr pIndexBuffer){
		Logger::Log(FB_ERROR_LOG_ARG, "Use SetIndexBuffer(int matGroupIdx, IIndexBuffer* pIndexBuffer) function instead.");
	}
	
	// override the input layout defined in material
	void SetInputLayout(InputLayoutPtr i){
		// Don't need to support it now.
	}

	void PreRender(const RenderParam& renderParam, RenderParamOut* renderParamOut){
		auto self = mSelf.lock();
		if (self->HasObjFlag(SceneObjectFlag::Hide))
			return;
		auto currentFrame = gpTimer->GetFrame();
		if (mLastPreRendered == currentFrame)
			return;

		mLastPreRendered = currentFrame;
		auto& animatedLocation = self->GetAnimatedLocation();
		animatedLocation.GetHomogeneous(mObjectConstants.gWorld);
		auto& renderer = Renderer::GetInstance();
		renderer.GatherPointLightData(self->GetAABB().get(), animatedLocation, &mPointLightConstants);
	}
	
	void Render(const RenderParam& renderParam, RenderParamOut* renderParamOut){
		auto& renderer = Renderer::GetInstance();		
		auto renderOption = renderer.GetOptions();
		if (renderOption->r_noMesh)
			return;
		auto self = mSelf.lock();
		if (self->HasObjFlag(SceneObjectFlag::Hide))
			return;

		auto radius = self->GetRadius();
		auto distToCam = self->GetDistToCam();
		if (distToCam > 70 && radius < 0.5f)
			return;

		if (distToCam > 100 && radius < 2.0f)
			return;

		if (distToCam > 250 && radius < 5.0f)
			return;

		renderer.BeginEvent("MeshObject");		

		mObjectConstants.gWorldView = renderer.GetCamera()->GetMatrix(Camera::View) * mObjectConstants.gWorld;
		mObjectConstants.gWorldViewProj = renderer.GetCamera()->GetMatrix(Camera::ViewProj) * mObjectConstants.gWorld;
		renderer.UpdateObjectConstantsBuffer(&mObjectConstants, true);

		if (renderParam.mRenderPass == RENDER_PASS::PASS_NORMAL)
			renderer.UpdatePointLightConstantsBuffer(&mPointLightConstants);

		renderer.SetPrimitiveTopology(mTopology);

		bool noDedicatedHighlight = !self->HasObjFlag(SceneObjectFlag::HighlightDedi);
		bool renderDepthPath = !self->HasObjFlag(SceneObjectFlag::NoDepthPath);
		bool useDepthPassAndNormalHighlight = noDedicatedHighlight && renderDepthPath;
		if (renderParam.mRenderPass == RENDER_PASS::PASS_SHADOW && useDepthPassAndNormalHighlight)
		{
			for(auto& it : mMaterialGroups)
			{
				if (!it.mMaterial || !it.mVBPos || it.mMaterial->IsNoShadowCast())
					continue;
				if (!it.mMaterial->BindSubPass(RENDER_PASS::PASS_SHADOW, true))
				{
					renderer.SetPositionInputLayout();
					renderer.SetShadowMapShader();
				}
				RenderMaterialGroup(&it, true);
			}
			return;
		}
		
		if (renderParam.mRenderPass == RENDER_PASS::PASS_DEPTH && useDepthPassAndNormalHighlight)
		{
			for(auto& it: mMaterialGroups)
			{
				if (!it.mMaterial || !it.mVBPos || it.mMaterial->IsNoShadowCast())
					continue;
				bool materialReady = false;
				if (it.mMaterial->BindSubPass(RENDER_PASS::PASS_DEPTH, false))
				{
					renderer.SetPositionInputLayout();
					materialReady = true;
				}
				else
				{
					renderer.SetDepthWriteShader();
					materialReady = true;
				}

				if (materialReady)
				{
					RenderMaterialGroup(&it, true);
				}
			}
			return;
		}
		// PASS_GODRAY_OCC_PRE
		if (renderParam.mRenderPass == RENDER_PASS::PASS_GODRAY_OCC_PRE && useDepthPassAndNormalHighlight && !mForceAlphaBlending)
		{
			renderer.SetPositionInputLayout();
			for(auto& it: mMaterialGroups)
			{
				if (!it.mMaterial || !it.mVBPos || it.mMaterial->IsNoShadowCast())
					continue;

				if (it.mMaterial->GetBindingShaders() & BINDING_SHADER_GS) {
					renderer.SetOccPreGSShader();
				}
				else {
					renderer.SetOccPreShader();
				}
				it.mMaterial->BindMaterialParams();

				RenderMaterialGroup(&it, true);
			}

			return;
		}

		// NORMAL PASS
		if (renderParam.mRenderPass == RENDER_PASS::PASS_NORMAL)
		{
			if (noDedicatedHighlight)
			{
				bool includeInputLayout = true;
				if (mInputLayoutOverride)
				{
					mInputLayoutOverride->Bind();
					includeInputLayout = false;
				}
				if (mForceAlphaBlending && !mMaterialGroups.empty()){
					auto it = mMaterialGroups.begin();
					renderer.SetPositionInputLayout();

					bool hasSubMat = it->mMaterial->BindSubPass(RENDER_PASS::PASS_DEPTH_ONLY, false);
					if (hasSubMat){
						// write only depth
						for(auto& it : mMaterialGroups)
						{
							it.mMaterial->BindSubPass(RENDER_PASS::PASS_DEPTH_ONLY, false);
							RenderMaterialGroup(&it, true);
						}
					}
					else{
						renderer.RestoreDepthStencilState();
						renderer.SetOneBiasedDepthRS();
						renderer.SetNoColorWriteState();

						renderer.SetDepthOnlyShader();
						// write only depth
						for(auto& it:mMaterialGroups)
						{
							RenderMaterialGroup(&it, true);
						}
					}
				}
				for(auto& it : mMaterialGroups)
				{
					auto material = mForceAlphaBlending ? it.mForceAlphaMaterial : it.mMaterial;
					if (!material || !it.mVBPos)
						continue;

					material->Bind(includeInputLayout);
					RenderMaterialGroup(&it, false);
					material->Unbind();
				}
			}
			bool mainRt = renderer.IsMainRenderTarget();
			// HIGHLIGHT
			if (mRenderHighlight && mainRt && !mForceAlphaBlending)
			{
				// draw silhouett to samll buffer
				auto rt = renderer.GetCurrentRenderTarget();
				assert(rt);
				if (rt->SetSmallSilouetteBuffer())
				{
					for(auto& it: mMaterialGroups)
					{
						if (!it.mVBPos)
							continue;
						bool materialReady = false;
						if (it.mMaterial && it.mMaterial->BindSubPass(RENDER_PASS::PASS_DEPTH, false))
						{
							renderer.SetPositionInputLayout();
							materialReady = true;
						}
						else
						{
							renderer.SetPositionInputLayout();
							renderer.SetDepthWriteShader();
							materialReady = true;
						}
						if (materialReady)
						{
							RenderMaterialGroup(&it, true);
						}
					}
				}
				if (rt->SetBigSilouetteBuffer())
				{
					for(auto& it: mMaterialGroups)
					{
						if (!it.mVBPos)
							continue;
						bool materialReady = false;
						if (it.mMaterial && it.mMaterial->BindSubPass(RENDER_PASS::PASS_DEPTH, false))
						{
							renderer.SetPositionInputLayout();
							materialReady = true;
						}
						else
						{
							renderer.SetPositionInputLayout();
							renderer.SetDepthWriteShader();
							materialReady = true;
						}
						if (materialReady)
						{
							RenderMaterialGroup(&it, true);
						}
					}
				}
				renderParamOut->mSilouetteRendered = true;
				rt->BindTargetOnly(true);
			}
			// debug
			if (renderOption->r_gameId && self->GetGameId() != -1){
				char buf[255];
				sprintf_s(buf, "%u", self->GetGameId());
				renderer.Draw3DText(self->GetPosition(), buf, Color::White);
			}
		}
	}
	
	void PostRender(const RenderParam& renderParam, RenderParamOut* renderParamOut){
		
	}
	

	void SetEnableHighlight(bool enable){
		mRenderHighlight = enable;
	}

	AnimationPtr GetAnimation() const { 
		auto self = mSelf.lock();
		return self->GetAnimation();
	}

	MeshObjectPtr Clone() const{
		auto self = mSelf.lock();
		MeshObjectPtr cloned = MeshObject::Create(*self);
		return cloned;

		/*cloned->mInputLayoutOverride = mInputLayoutOverride;
		cloned->mName = mName;
		cloned->mTopology = mTopology;
		cloned->mObjectConstants = mObjectConstants;
		FB_FOREACH(it, mMaterialGroups)
		{
			size_t idx = std::distance(mMaterialGroups.begin(), it);
			if (idx >= cloned->mMaterialGroups.size())
			{
				cloned->mMaterialGroups.push_back(MaterialGroup());
			}
			MaterialGroup& mg = cloned->mMaterialGroups.back();
			mg.mMaterial = it->mMaterial;
			mg.mVBPos = it->mVBPos;
			mg.mVBNormal = it->mVBNormal;
			mg.mVBUV = it->mVBUV;
			mg.mVBColor = it->mVBColor;
			mg.mVBTangent = it->mVBTangent;
			mg.mIndexBuffer = it->mIndexBuffer;
			mg.mPositions = it->mPositions;
		}
		cloned->mAuxCloned = mAuxCloned ? mAuxCloned : (AUXILIARIES*)&mAuxil;
		cloned->mCollisionsCloned = mCollisionsCloned ? mCollisionsCloned : (COLLISION_SHAPES*)&mCollisions;
		cloned->mAABB = mAABB;
		return cloned;*/
	}

	void RenderSimple(){
		for(auto& it: mMaterialGroups)
		{
			if (!it.mVBPos)
				continue;
			RenderMaterialGroup(&it, true);
		}
	}

	void ClearMeshData(){
		for(auto& it: mMaterialGroups)
		{
			it.mPositions.clear();
			it.mNormals.clear();
			it.mUVs.clear();
			it.mColors.clear();

			it.mPositions.resize(0);
			it.mNormals.resize(0);
			it.mUVs.resize(0);
			it.mColors.resize(0);
		}
	}

	void StartModification(){
		mModifying = true;
	}

	void AddTriangle(int matGroupIdx, const Vec3& pos0, const Vec3& pos1, const Vec3& pos2){
		CreateMaterialGroupFor(matGroupIdx);
		mMaterialGroups[matGroupIdx].mPositions.push_back(pos0);
		mMaterialGroups[matGroupIdx].mPositions.push_back(pos1);
		mMaterialGroups[matGroupIdx].mPositions.push_back(pos2);
	}

	void AddQuad(int matGroupIdx, const Vec3 pos[4], const Vec3 normal[4]){
		CreateMaterialGroupFor(matGroupIdx);
		mMaterialGroups[matGroupIdx].mPositions.push_back(pos[0]);
		mMaterialGroups[matGroupIdx].mPositions.push_back(pos[1]);
		mMaterialGroups[matGroupIdx].mPositions.push_back(pos[2]);

		mMaterialGroups[matGroupIdx].mPositions.push_back(pos[2]);
		mMaterialGroups[matGroupIdx].mPositions.push_back(pos[1]);
		mMaterialGroups[matGroupIdx].mPositions.push_back(pos[3]);


		mMaterialGroups[matGroupIdx].mNormals.push_back(normal[0]);
		mMaterialGroups[matGroupIdx].mNormals.push_back(normal[1]);
		mMaterialGroups[matGroupIdx].mNormals.push_back(normal[2]);

		mMaterialGroups[matGroupIdx].mNormals.push_back(normal[2]);
		mMaterialGroups[matGroupIdx].mNormals.push_back(normal[1]);
		mMaterialGroups[matGroupIdx].mNormals.push_back(normal[3]);
	}

	void AddQuad(int matGroupIdx, const Vec3 pos[4], const Vec3 normal[4], const Vec2 uv[4]){
		AddQuad(matGroupIdx, pos, normal);
		mMaterialGroups[matGroupIdx].mUVs.push_back(uv[0]);
		mMaterialGroups[matGroupIdx].mUVs.push_back(uv[1]);
		mMaterialGroups[matGroupIdx].mUVs.push_back(uv[2]);

		mMaterialGroups[matGroupIdx].mUVs.push_back(uv[2]);
		mMaterialGroups[matGroupIdx].mUVs.push_back(uv[1]);
		mMaterialGroups[matGroupIdx].mUVs.push_back(uv[3]);
	}

	void SetPositions(int matGroupIdx, const Vec3* p, size_t numVertices){
		CreateMaterialGroupFor(matGroupIdx);
		mMaterialGroups[matGroupIdx].mPositions.assign(p, p + numVertices);
	}

	void SetNormals(int matGroupIdx, const Vec3* n, size_t numNormals){
		CreateMaterialGroupFor(matGroupIdx);
		mMaterialGroups[matGroupIdx].mNormals.assign(n, n + numNormals);
	}

	void SetUVs(int matGroupIdx, const Vec2* uvs, size_t numUVs){
		CreateMaterialGroupFor(matGroupIdx);
		mMaterialGroups[matGroupIdx].mUVs.assign(uvs, uvs + numUVs);
	}

	void SetTriangles(int matGroupIdx, const ModelTriangle* tris, size_t numTri){
		CreateMaterialGroupFor(matGroupIdx);
		mMaterialGroups[matGroupIdx].mTriangles.assign(tris, tris + numTri);
	}

	void SetColors(int matGroupIdx, const DWORD* colors, size_t numColors){
		CreateMaterialGroupFor(matGroupIdx);
		mMaterialGroups[matGroupIdx].mColors.assign(colors, colors + numColors);
	}

	void SetTangents(int matGroupIdx, const Vec3* t, size_t numTangents){
		CreateMaterialGroupFor(matGroupIdx);
		mMaterialGroups[matGroupIdx].mTangents.assign(t, t + numTangents);
	}

	void SetIndices(int matGroupIdx, const UINT* indices, size_t numIndices){
		auto& renderer = Renderer::GetInstance();
		CreateMaterialGroupFor(matGroupIdx);
		if (numIndices <= std::numeric_limits<USHORT>::max())
		{
			std::vector<USHORT> sIndices(indices, indices + numIndices);
			mMaterialGroups[matGroupIdx].mIndexBuffer =
				renderer.CreateIndexBuffer(&sIndices[0], numIndices, INDEXBUFFER_FORMAT_16BIT);
		}
		else
		{
			mMaterialGroups[matGroupIdx].mIndexBuffer =
				renderer.CreateIndexBuffer((void*)indices, numIndices, INDEXBUFFER_FORMAT_32BIT);
		}
	}

	void SetIndices(int matGroupIdx, const USHORT* indices, size_t numIndices){
		CreateMaterialGroupFor(matGroupIdx);
		auto& renderer = Renderer::GetInstance();
		mMaterialGroups[matGroupIdx].mIndexBuffer = renderer.CreateIndexBuffer((void*)indices, numIndices, INDEXBUFFER_FORMAT_16BIT);
	}

	void SetIndexBuffer(int matGroupIdx, IndexBufferPtr pIndexBuffer){
		CreateMaterialGroupFor(matGroupIdx);
		mMaterialGroups[matGroupIdx].mIndexBuffer = pIndexBuffer;
	}

	Vec3* GetPositions(int matGroupIdx, size_t& outNumPositions){
		CreateMaterialGroupFor(matGroupIdx);
		outNumPositions = mMaterialGroups[matGroupIdx].mPositions.size();
		if (outNumPositions)
			return &(mMaterialGroups[matGroupIdx].mPositions[0]);
		else
			return 0;
	}

	Vec3* GetNormals(int matGroupIdx, size_t& outNumNormals){
		CreateMaterialGroupFor(matGroupIdx);
		outNumNormals = mMaterialGroups[matGroupIdx].mNormals.size();
		if (outNumNormals)
			return &(mMaterialGroups[matGroupIdx].mNormals[0]);
		else
			return 0;
	}

	Vec2* GetUVs(int matGroupIdx, size_t& outNumUVs){
		CreateMaterialGroupFor(matGroupIdx);
		outNumUVs = mMaterialGroups[matGroupIdx].mUVs.size();
		if (outNumUVs)
			return &(mMaterialGroups[matGroupIdx].mUVs[0]);
		else
			return 0;
	}

	void GenerateTangent(int matGroupIdx, UINT* indices, size_t num){
		assert(mModifying);
		assert(matGroupIdx < (int)mMaterialGroups.size());
		MaterialGroup& group = mMaterialGroups[matGroupIdx];
		if (group.mUVs.empty())
			return;
		group.mTangents.assign(group.mPositions.size(), Vec3(1, 0, 0));
		if (num)
		{
			for (size_t i = 0; i < num; i += 3)
			{
				Vec3 p1 = group.mPositions[indices[i]];
				Vec3 p2 = group.mPositions[indices[i + 1]];
				Vec3 p3 = group.mPositions[indices[i + 2]];
				Vec2 uv1 = group.mUVs[indices[i]];
				Vec2 uv2 = group.mUVs[indices[i + 1]];
				Vec2 uv3 = group.mUVs[indices[i + 2]];
				Vec3 tan = CalculateTangentSpaceVector(p1, p2, p3,
					uv1, uv2, uv3);
				group.mTangents[indices[i]] = tan;
				group.mTangents[indices[i + 1]] = tan;
				group.mTangents[indices[i + 2]] = tan;
			}
		}
		else
		{
			size_t nump = group.mPositions.size();
			for (size_t i = 0; i < nump; i += 3)
			{
				Vec3 p1 = group.mPositions[i];
				Vec3 p2 = group.mPositions[i + 1];
				Vec3 p3 = group.mPositions[i + 2];
				Vec2 uv1 = group.mUVs[i];
				Vec2 uv2 = group.mUVs[i + 1];
				Vec2 uv3 = group.mUVs[i + 2];
				Vec3 tan = CalculateTangentSpaceVector(p1, p2, p3,
					uv1, uv2, uv3);
				group.mTangents[i] = tan;
				group.mTangents[i + 1] = tan;
				group.mTangents[i + 2] = tan;
			}
		}
	}

	void EndModification(bool keepMeshData){
		mModifying = false;
		mAABB->StartComputeFromData();
		for(auto& it: mMaterialGroups)
		{
			auto& renderer = Renderer::GetInstance();
			if (!it.mPositions.empty())
			{
				it.mVBPos = renderer.CreateVertexBuffer(
					&it.mPositions[0], sizeof(Vec3), it.mPositions.size(),
					mUseDynamicVB[BUFFER_TYPE_POSITION] ? BUFFER_USAGE_DYNAMIC : BUFFER_USAGE_IMMUTABLE,
					mUseDynamicVB[BUFFER_TYPE_POSITION] ? BUFFER_CPU_ACCESS_WRITE : BUFFER_CPU_ACCESS_NONE);
				mAABB->AddComputeData(&it.mPositions[0], it.mPositions.size());
			}
			else
			{
				it.mVBPos = 0;
			}
			if (!it.mNormals.empty())
			{
				assert(it.mPositions.size() == it.mNormals.size());
				it.mVBNormal = renderer.CreateVertexBuffer(
					&it.mNormals[0], sizeof(Vec3), it.mNormals.size(),
					mUseDynamicVB[BUFFER_TYPE_NORMAL] ? BUFFER_USAGE_DYNAMIC : BUFFER_USAGE_IMMUTABLE,
					mUseDynamicVB[BUFFER_TYPE_NORMAL] ? BUFFER_CPU_ACCESS_WRITE : BUFFER_CPU_ACCESS_NONE);
			}
			else
			{
				it.mVBNormal = 0;
			}
			if (!it.mUVs.empty())
			{
				assert(it.mPositions.size() == it.mUVs.size());
				it.mVBUV = renderer.CreateVertexBuffer(
					&it.mUVs[0], sizeof(Vec2), it.mUVs.size(),
					mUseDynamicVB[BUFFER_TYPE_UV] ? BUFFER_USAGE_DYNAMIC : BUFFER_USAGE_IMMUTABLE,
					mUseDynamicVB[BUFFER_TYPE_UV] ? BUFFER_CPU_ACCESS_WRITE : BUFFER_CPU_ACCESS_NONE);
			}
			else
			{
				it.mVBUV = 0;
			}

			if (!it.mColors.empty())
			{
				assert(it.mPositions.size() == it.mColors.size());
				it.mVBColor = renderer.CreateVertexBuffer(
					&it.mColors[0], sizeof(DWORD), it.mColors.size(),
					mUseDynamicVB[BUFFER_TYPE_COLOR] ? BUFFER_USAGE_DYNAMIC : BUFFER_USAGE_IMMUTABLE,
					mUseDynamicVB[BUFFER_TYPE_COLOR] ? BUFFER_CPU_ACCESS_WRITE : BUFFER_CPU_ACCESS_NONE);
			}
			else
			{
				it.mVBColor = 0;
			}

			if (!it.mTangents.empty())
			{
				assert(it.mPositions.size() == it.mTangents.size());
				it.mVBTangent = renderer.CreateVertexBuffer(
					&it.mTangents[0], sizeof(Vec3), it.mTangents.size(),
					mUseDynamicVB[BUFFER_TYPE_TANGENT] ? BUFFER_USAGE_DYNAMIC : BUFFER_USAGE_IMMUTABLE,
					mUseDynamicVB[BUFFER_TYPE_TANGENT] ? BUFFER_CPU_ACCESS_WRITE : BUFFER_CPU_ACCESS_NONE);
			}
			else
			{
				it.mVBTangent = 0;
			}


		}
		mAABB->EndComputeFromData();
		auto self = mSelf.lock();
		auto boundingVolume = self->GetBoundingVolume();
		boundingVolume->SetCenter(mAABB->GetCenter());
		boundingVolume->SetRadius(mAABB->GetRadius());
		auto boundingVolumeWorld = self->GetBoundingVolumeWorld();
		boundingVolumeWorld->SetCenter(boundingVolume->GetCenter() + self->GetPosition());
		const auto& s = self->GetScale();
		boundingVolumeWorld->SetRadius(boundingVolume->GetRadius() * std::max(std::max(s.x, s.y), s.z));;

		if (!keepMeshData)
			ClearMeshData();
	}

	void SetTopology(PRIMITIVE_TOPOLOGY topology){
		mTopology = topology;
	}

	PRIMITIVE_TOPOLOGY GetTopology(){
		return mTopology;
	}

	const AUXILIARIES& GetAuxiliaries() const { 
		return mAuxCloned ? *mAuxCloned : mAuxil; 
	}

	void SetAuxiliaries(const AUXILIARIES& aux) { 
		mAuxil = aux; 
	}

	void AddCollisionShape(const COL_SHAPE& data){
		FBCollisionShapePtr cs = FBCollisionShape::Create(data.first, data.second, 0);
		mCollisions.push_back(cs);
	}

	void SetCollisionShapes(COLLISION_INFOS& colInfos){
		DeleteCollisionShapes();
		for (const auto& var : colInfos)
		{
			mCollisions.push_back(FBCollisionShape::Create(var.mColShapeType, var.mTransform, var.mCollisionMesh));
		}
	}

	
	void SetCollisionMesh(MeshObjectPtr colMesh){
		assert(!mCollisions.empty());
		mCollisions.back()->SetCollisionMesh(colMesh);
	}

	unsigned GetNumCollisionShapes() const { 
		return mCollisionsCloned ? mCollisionsCloned->size() : 0; 
	}

	bool HasCollisionShapes() const {
		return mCollisionsCloned ? !mCollisionsCloned->empty() : false;
	}

	FBCollisionShapeConstPtr GetCollisionShape(unsigned idx) const { 
		return mCollisionsCloned ? (*mCollisionsCloned)[idx] : 0; 
	}

	bool CheckNarrowCollision(BoundingVolumeConstPtr pBV) const{
		unsigned num = GetNumCollisionShapes();
		if (!num)
			return true;
		auto self = mSelf.lock();
		auto& location = self->GetLocation();
		for (unsigned i = 0; i < num; ++i)
		{
			FBCollisionShapeConstPtr cs = GetCollisionShape(i);
			
			bool collide = cs->TestCollision(pBV, location);
			if (collide) {
				return true;
			}
		}

		return false;
	}

	Ray3::IResult CheckNarrowCollisionRay(const Ray3& ray) const{
		unsigned num = GetNumCollisionShapes();
		if (!num)
			return Ray3::IResult(false, 0.f);

		auto self = mSelf.lock();
		Real minDist = FLT_MAX;
		bool collided = false;
		auto& location = self->GetLocation();
		for (unsigned i = 0; i < num; ++i)
		{
			auto cs = GetCollisionShape(i);
			auto result = cs->Intersects(ray, location);
			if (result.first)
			{
				collided = true;
				if (minDist > result.second)
					minDist = result.second;
			}
		}

		return Ray3::IResult(collided, minDist);
	}

	Vec3 GetRandomPosInVolume(const Vec3* nearWorld = 0) const{
		unsigned num = GetNumCollisionShapes();
		if (!num)
			return Vec3::ZERO;

		auto index = Random((unsigned)0, num - 1);
		assert(index < num);
		auto cs = GetCollisionShape(index);
		assert(cs);		
		return cs->GetRandomPosInVolume(nearWorld, mSelf.lock()->GetLocation());
	}

	void DeleteCollisionShapes(){
		mCollisions.clear();
	}

	void SetUseDynamicVB(BUFFER_TYPE type, bool useDynamicVB){
		mUseDynamicVB[type] = useDynamicVB;
	}

	MapData MapVB(BUFFER_TYPE type, size_t materialGroupIdx){
		if (!mUseDynamicVB[type])
		{
			assert(0);
			return MapData();
		}
		if (materialGroupIdx >= mMaterialGroups.size())
		{
			assert(0);
			return MapData();
		}

		MaterialGroup& mg = mMaterialGroups[materialGroupIdx];
		switch (type)
		{
		case BUFFER_TYPE_POSITION:
			return mg.mVBPos->Map(MAP_TYPE_WRITE, 0, MAP_FLAG_NONE);
		case BUFFER_TYPE_NORMAL:
			return mg.mVBNormal->Map(MAP_TYPE_WRITE_NO_OVERWRITE, 0, MAP_FLAG_NONE);
		case BUFFER_TYPE_UV:
			return mg.mVBUV->Map(MAP_TYPE_WRITE, 0, MAP_FLAG_NONE);
		case BUFFER_TYPE_COLOR:
			return mg.mVBColor->Map(MAP_TYPE_WRITE, 0, MAP_FLAG_NONE);
		case BUFFER_TYPE_TANGENT:
			return mg.mVBTangent->Map(MAP_TYPE_WRITE, 0, MAP_FLAG_NONE);
		}
		assert(0);

		return MapData();
	}

	void UnmapVB(BUFFER_TYPE type, size_t materialGroupIdx){
		if (!mUseDynamicVB[type])
		{
			assert(0);
			return;
		}
		if (materialGroupIdx >= mMaterialGroups.size())
		{
			assert(0);
			return;
		}
		MaterialGroup& mg = mMaterialGroups[materialGroupIdx];
		switch (type)
		{
		case BUFFER_TYPE_POSITION:
			return mg.mVBPos->Unmap();
		case BUFFER_TYPE_NORMAL:
			return mg.mVBNormal->Unmap();
		case BUFFER_TYPE_UV:
			return mg.mVBUV->Unmap();
		case BUFFER_TYPE_COLOR:
			return mg.mVBColor->Unmap();
		case BUFFER_TYPE_TANGENT:
			return mg.mVBTangent->Unmap();
		}
		assert(0);
	}

	bool RayCast(const Ray3& ray, Vec3& location, const ModelTriangle** outTri = 0) const{
		auto& sceneMgr = SceneManager::GetInstance();
		auto self = mSelf.lock();
		auto mesh = sceneMgr.GetMeshArcheType(self->GetName());
		assert(mesh);
		const Real maxRayDistance = 100000;
		Real tMin = maxRayDistance;
		const Real epsilon = 0.001f;
		ModelIntersection rayTriIntersection;
		rayTriIntersection.valid = false;

		for (const auto& mg : mesh->mImpl->mMaterialGroups)
		{
			for (const auto& tri : mg.mTriangles)
			{
				Real NdotD = tri.faceNormal.Dot(ray.GetDir());
				if (abs(NdotD) < epsilon)
				{
					// ray is parallel or nearly parallel to polygon plane
					continue;
				}

				Real t = (tri.d - tri.faceNormal.Dot(ray.GetOrigin())) / NdotD;
				if (t <= 0)
				{
					// ray origin is behind the triangle;
					continue;
				}
				if (t >= tMin)
				{
					continue;
				}
				Vec3 intersectionPoint = ray.GetOrigin() + ray.GetDir() * t;
				// find the interpolation parameters alpha and beta using 2D projections
				Real alpha;
				Real beta;
				Vec2 P;    // projection of the intersection onto an axis aligned plane
				switch (tri.dominantAxis)
				{
				case 0:
					P.x = intersectionPoint.y;
					P.y = intersectionPoint.z;
					break;
				case 1:
					P.x = intersectionPoint.x;
					P.y = intersectionPoint.z;
					break;
				case 2:
				default:
					P.x = intersectionPoint.x;
					P.y = intersectionPoint.y;
				}
				Real u0, u1, u2, v0, v1, v2;
				u0 = P.x - tri.v0Proj.x;
				v0 = P.y - tri.v0Proj.y;
				u1 = tri.v1Proj.x - tri.v0Proj.x;
				u2 = tri.v2Proj.x - tri.v0Proj.x;
				v1 = tri.v1Proj.y - tri.v0Proj.y;
				v2 = tri.v2Proj.y - tri.v0Proj.y;
				if (abs(u1) < epsilon)
				{
					beta = u0 / u2;
					if ((beta >= 0) && (beta <= 1))
					{
						alpha = (v0 - beta * v2) / v1;
						if ((alpha >= 0) && ((alpha + beta) <= 1))
						{
							rayTriIntersection.valid = true;
							rayTriIntersection.alpha = alpha;
							rayTriIntersection.beta = beta;
							rayTriIntersection.pTri = &tri;
							tMin = t;
						}
					}
				}
				else
				{
					beta = (v0*u1 - u0*v1) / (v2*u1 - u2*v1);
					if ((beta >= 0) && (beta <= 1))
					{
						alpha = (u0 - beta * u2) / u1;
						if ((alpha >= 0) && ((alpha + beta) <= 1))
						{
							rayTriIntersection.valid = true;
							rayTriIntersection.alpha = alpha;
							rayTriIntersection.beta = beta;
							rayTriIntersection.pTri = &tri;
							tMin = t;
						}
					}
				}
			}
			if (!rayTriIntersection.valid)
			{
				if (outTri)
					*outTri = NULL;
				return false;
			}
			else
			{
				// compute the location using barycentric coordinates
				const ModelTriangle* pTri = rayTriIntersection.pTri;
				Real alpha = rayTriIntersection.alpha;
				Real beta = rayTriIntersection.beta;
				location.x = ((1 - (alpha + beta)) * mg.mPositions[pTri->v[0]].x) +
					alpha * mg.mPositions[pTri->v[2]].x +
					beta * mg.mPositions[pTri->v[2]].x;
				location.y = ((1 - (alpha + beta)) * mg.mPositions[pTri->v[0]].y) +
					alpha * mg.mPositions[pTri->v[1]].y +
					beta * mg.mPositions[pTri->v[2]].y;
				location.z = ((1 - (alpha + beta)) * mg.mPositions[pTri->v[0]].z) +
					alpha * mg.mPositions[pTri->v[1]].z +
					beta * mg.mPositions[pTri->v[2]].z;
				if (outTri)
				{
					*outTri = pTri;
				}
				return true;
			}
		}
		return false;
	}

	BoundingVolumeConstPtr GetAABB() const { 
		return mAABB; 
	}

	void ClearVertexBuffers(){
		for (auto& it : mMaterialGroups)
		{
			it.mVBPos = 0;
		}
	}

	void SetAlpha(Real alpha){
		for (auto& it : mMaterialGroups)
		{
			if (it.mMaterial)
			{
				BLEND_DESC bdesc;
				bdesc.RenderTarget[0].BlendEnable = alpha != 1.0f;
				bdesc.RenderTarget[0].BlendOp = BLEND_OP_ADD;
				bdesc.RenderTarget[0].SrcBlend = BLEND_SRC_ALPHA;
				bdesc.RenderTarget[0].DestBlend = BLEND_INV_SRC_ALPHA;
				it.mMaterial->SetBlendState(bdesc);
				auto diffuse = it.mMaterial->GetDiffuseColor();
				diffuse.w = alpha;
				it.mMaterial->SetDiffuseColor(diffuse);
			}
		}
	}

	void SetForceAlphaBlending(bool enable, Real alpha, Real forceGlow = 0.f, bool disableDepth = false){
		mForceAlphaBlending = enable;
		if (mForceAlphaBlending){
			for (auto& it : mMaterialGroups)
			{
				if (it.mMaterial)
				{
					if (!it.mForceAlphaMaterial){
						it.mForceAlphaMaterial = it.mMaterial->Clone();						
						if (forceGlow != 0.f){
							it.mForceAlphaMaterial->AddShaderDefine("_FORCE_GLOW", FormatString(".2f", forceGlow).c_str());
							it.mForceAlphaMaterial->ApplyShaderDefines();
						}
					}
					BLEND_DESC bdesc;
					bdesc.RenderTarget[0].BlendEnable = alpha != 1.0f;
					bdesc.RenderTarget[0].BlendOp = BLEND_OP_ADD;
					bdesc.RenderTarget[0].SrcBlend = BLEND_SRC_ALPHA;
					bdesc.RenderTarget[0].DestBlend = BLEND_INV_SRC_ALPHA;
					it.mForceAlphaMaterial->SetBlendState(bdesc);
					auto diffuse = it.mForceAlphaMaterial->GetDiffuseColor();
					diffuse.w = alpha;
					it.mForceAlphaMaterial->SetDiffuseColor(diffuse);

					RASTERIZER_DESC rs;
					rs.CullMode = CULL_MODE_NONE;
					it.mForceAlphaMaterial->SetRasterizerState(rs);

					DEPTH_STENCIL_DESC ds;
					ds.DepthWriteMask = DEPTH_WRITE_MASK_ZERO;
					if (disableDepth){
						ds.DepthEnable = false;
					}
					it.mForceAlphaMaterial->SetDepthStencilState(ds);

					it.mForceAlphaMaterial->SetTransparent(true);
				}
			}
		}
	}

	void SetAmbientColor(const Color& color){
		for (auto& it : mMaterialGroups)
		{
			if (it.mMaterial)
			{
				it.mMaterial->SetAmbientColor(color.GetVec4());
			}
			if (it.mForceAlphaMaterial){
				it.mForceAlphaMaterial->SetAmbientColor(color.GetVec4());
			}
		}
	}

	void CreateMaterialGroupFor(int matGroupIdx){
		if ((int)mMaterialGroups.size() <= matGroupIdx)
		{
			mMaterialGroups.push_back(MaterialGroup());
			auto& renderer = Renderer::GetInstance();
			mMaterialGroups.back().mMaterial = renderer.GetMissingMaterial();
		}
	}
	void RenderMaterialGroup(MaterialGroup* it, bool onlyPos){
		assert(it);
		if (!it || !it->mMaterial || !it->mVBPos)
			return;
		auto& renderer = Renderer::GetInstance();
		if (onlyPos)
		{
			const unsigned int numBuffers = 1;

			VertexBufferPtr buffers[numBuffers] = { it->mVBPos };
			unsigned int strides[numBuffers] = { it->mVBPos->GetStride() };
			unsigned int offsets[numBuffers] = { 0 };
			renderer.SetVertexBuffer(0, numBuffers, buffers, strides, offsets);
			if (it->mIndexBuffer)
			{
				renderer.SetIndexBuffer(it->mIndexBuffer);
				renderer.DrawIndexed(it->mIndexBuffer->GetNumIndices(), 0, 0);
			}
			else
			{
				renderer.Draw(it->mVBPos->GetNumVertices(), 0);
			}
		}
		else
		{
			const unsigned int numBuffers = 5;

			VertexBufferPtr buffers[numBuffers] = { it->mVBPos, it->mVBNormal, it->mVBUV, it->mVBColor, it->mVBTangent };
			unsigned int strides[numBuffers] = { it->mVBPos->GetStride(),
				it->mVBNormal ? it->mVBNormal->GetStride() : 0,
				it->mVBUV ? it->mVBUV->GetStride() : 0,
				it->mVBColor ? it->mVBColor->GetStride() : 0,
				it->mVBTangent ? it->mVBTangent->GetStride() : 0 };
			unsigned int offsets[numBuffers] = { 0, 0, 0, 0, 0 };
			renderer.SetVertexBuffer(0, numBuffers, buffers, strides, offsets);
			if (it->mIndexBuffer)
			{
				renderer.SetIndexBuffer(it->mIndexBuffer);
				renderer.DrawIndexed(it->mIndexBuffer->GetNumIndices(), 0, 0);
			}
			else
			{
				renderer.Draw(it->mVBPos->GetNumVertices(), 0);
			}
		}
	}
};