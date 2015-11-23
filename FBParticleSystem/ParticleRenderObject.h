#pragma once
#include "FBCommonHeaders/Types.h"
#include "FBSceneManager/SpatialSceneObject.h"
namespace fastbird
{
	struct ParticleRenderKey;
	DECLARE_SMART_PTR(Material);
	DECLARE_SMART_PTR(ParticleRenderObject);
	class ParticleRenderObject : public SpatialSceneObject
	{
		DECLARE_PIMPL_NON_COPYABLE(ParticleRenderObject);
		ParticleRenderObject();
		~ParticleRenderObject();
		
	public:
		static const int MAX_SHARED_VERTICES;
		static ParticleRenderObjectPtr GetRenderObject(ParticleRenderKey& key, bool& created);
		static void ClearParticles();
		static void EndUpdateParticles();
		static void FinalizeRenderObjects();
		static size_t GetNumRenderObject();
		static size_t GetNumDrawCalls();
		static size_t GetNumPrimitives();

		//---------------------------------------------------------------------------
		// SceneObject Interfaces
		//---------------------------------------------------------------------------
		void PreRender(const RenderParam& param, RenderParamOut* paramOut);
		void Render(const RenderParam& param, RenderParamOut* paramOut);
		void PostRender(const RenderParam& param, RenderParamOut* paramOut);

		//---------------------------------------------------------------------------
		// OWN
		//---------------------------------------------------------------------------
		MaterialPtr GetMaterial() const;
		void Clear();
		void EndUpdate();
		void SetDoubleSided(bool set);
		void SetTexture(const char* texturePath);

		struct Vertex
		{
			Vec3 mPos;
			Vec4 mUDirection_Intensity;
			Vec3 mVDirection;
			Vec4 mPivot_Size;
			Vec4 mRot_Alpha_uv;
			Vec2 mUVStep;
			DWORD mColor;
		};
		Vertex* Map(UINT numVertices, unsigned& canWrite);
	};
}