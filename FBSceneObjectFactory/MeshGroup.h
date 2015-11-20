#pragma once
#include "FBCommonHeaders/platform.h"
#include "FBSceneManager/SpatialObject.h"
#include "FBRenderer/IRenderable.h"
#include "MeshAuxiliary.h"
#include "CollisionInfo.h"
namespace fastbird
{
	class Color;
	class AnimationData;
	DECLARE_SMART_PTR(MeshObject);
	DECLARE_SMART_PTR(MeshGroup);
	class MeshGroup : public SpatialObject, public IRenderable
	{
		DECLARE_PIMPL_CLONEABLE(MeshGroup);

	protected:
		MeshGroup();
		~MeshGroup();
		
	public:
		static MeshGroupPtr Create();
		static MeshGroupPtr Create(const MeshGroup& other);

		//---------------------------------------------------------------------------
		// IRenderable
		//---------------------------------------------------------------------------
		void SetMaterial(const char* filepath, int pass);
		void SetMaterial(MaterialPtr pMat, int pass);
		MaterialPtr GetMaterial(int pass = 0) const;		
		void SetVertexBuffer(VertexBufferPtr pVertexBuffer);
		void SetIndexBuffer(IndexBufferPtr pIndexBuffer);
		// override the input layout defined in material
		void SetInputLayout(InputLayoutPtr i);
		void PreRender(const RenderParam& param, RenderParamOut* paramOut);
		void Render(const RenderParam& param, RenderParamOut* paramOut);
		void PostRender(const RenderParam& param, RenderParamOut* paramOut);
		void SetEnableHighlight(bool enable);		

		//---------------------------------------------------------------------------
		// Own functions
		//---------------------------------------------------------------------------		
		MeshGroupPtr Clone() const;
		// order of inserting meshes is important. parent first.
		// transformation is in parent space.
		size_t AddMesh(MeshObjectPtr mesh, const Transformation& transform, size_t parent);
		const char* GetNameOfMesh(size_t idx);
		size_t GetNumMeshes() const;
		void AddMeshRotation(size_t idx, const Quat& rot);
		const Quat& GetMeshRotation(size_t idx) const;
		void SetMeshRotation(size_t idx, const Quat& rot);
		const Vec3& GetMeshOffset(size_t idx) const;
		/// Returns auxiliaries data
		/// You do not own the returned pointer.
		const AUXILIARIES* GetAuxiliaries(size_t idx) const;
		void SetAuxiliaries(size_t idx, const AUXILIARIES& aux);
		void AddAuxiliary(const AUXILIARY& aux);
		void AddAuxiliary(size_t idx, const AUXILIARY& v);
		void SetCollisionShapes(COLLISION_INFOS& colInfos);
		void AddCollisionShape(size_t idx, std::pair<FBColShape::Enum, Transformation>& data);						
		// force == false
		void UpdateTransform(const RenderParam& param, RenderParamOut* paramOut);
		void UpdateTransform(const RenderParam& param, RenderParamOut* paramOut, bool force);
		void PlayAction(const std::string& name, bool immediate, bool reverse);
		bool IsActionDone(const char* action) const;
		bool IsPlayingAction() const;
		Transformation GetToLocalTransform(unsigned meshIdx);
		Transformation GetToLocalTransform(const char* meshName);
		void SetAlpha(float alpha);
		void SetForceAlphaBlending(bool enable, float alpha, float forceGlow = 0.f, bool disableDepth = false);
		void SetAmbientColor(const Color& color);
		MeshObjectPtr GetMeshObject(unsigned idx);
		unsigned GetNumCollisionShapes() const;
		const FBCollisionShapeConstPtr GetCollisionShape(unsigned idx) const;
		bool HasCollisionShapes() const;
		unsigned GetNumCollisionShapes(unsigned idx) const;
	};
}