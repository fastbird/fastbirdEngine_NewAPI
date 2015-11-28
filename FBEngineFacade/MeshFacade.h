#pragma once
#include "FBCommonHeaders/Types.h"
#include "FBSceneManager/SceneObjectFlag.h"
#include "CollisionShapeInfo.h"
#include "MeshLoadOptions.h"
namespace fastbird{
	struct ModelTriangle;
	typedef std::vector< std::pair<std::string, Transformation> > AUXILIARIES;
	FB_DECLARE_SMART_PTR(SpatialObject);
	FB_DECLARE_SMART_PTR(BoundingVolume);
	FB_DECLARE_SMART_PTR(Material);
	FB_DECLARE_SMART_PTR(MeshFacade);
	FB_DECLARE_SMART_PTR(Scene);
	FB_DECLARE_SMART_PTR(IScene);
	class FB_DLL_ENGINEFACADE MeshFacade{
		FB_DECLARE_PIMPL_NON_COPYABLE(MeshFacade);
		MeshFacade();
		~MeshFacade();

	public:
		static MeshFacadePtr Create();		
		bool LoadMeshObject(const char* daePath);
		bool LoadMeshObject(const char* daePath, const MeshLoadOptions& options);
		bool LoadMeshGroup(const char* daePath);

		bool IsVaildMesh() const;
		bool IsMeshObject() const;
		bool IsMeshGroup() const;

		void SetGameType(int type);
		void SetGameId(unsigned id);
		void SetGamePtr(void* ptr);
		/// Combination of SceneObjectFlag::Enum
		void ModifyObjFlag(unsigned flag, bool enable);		
		void SetEnableHighlight(bool enable);
		MaterialPtr GetMaterial() const;
		void SetMaterialParameter(unsigned idx, const Vec4& value);
		void SetMaterial(MaterialPtr material);
		void SetMaterial(const char* path);
		bool AttachToScene();
		/// Attach to the current scene which is bound to the current render target.
		bool AttachToCurrentScene();
		bool AttachToScene(IScenePtr scene);
		/// not including RTT
		bool DetachFromScene();
		bool DetachFromScene(bool includingRtt);
		// attached any of scenes
		bool IsAttached() const;
		bool IsAttached(IScenePtr scene) const;
		void SetAlpha(float alpha);
		void SetForceAlphaBlending(bool enable, float alpha, float forceGlow, bool disableDepth);
		void SetAmbientColor(const Color& color);
		/// Returns auxiliaries for mesh object, and master auxiliaries for the mesh group
		const AUXILIARIES* GetAuxiliaries() const;		
		/// Returns auxiliaries for sub meshes in the mesh gorup.
		/// calling with -1 is same with calling GetAuxiliaries()
		const AUXILIARIES* GetAuxiliaries(unsigned idx) const;
		Transformation GetAuxiliaryWorldTransformation(const char* name, bool& outFound) const;
		Transformations GetAuxiliaryWorldTransformations(const char* name) const;
		Transformations GetAuxiliaryTransformations(const char* name) const;
		Vec3s GetAuxiliaryWorldPositions(const char* name) const;
		Vec3s GetAuxiliaryPositions(const char* name) const;
		
		
		const Transformation& GetTransformation() const;
		void SetTransformation(const Transformation& transform);
		void SetPosition(const Vec3& pos);
		void SetRotation(const Quat& rot);
		void SetScale(const Vec3& scale);
		const BoundingVolumePtr GetBoundingVolume() const;
		const BoundingVolumePtr GetBoundingVolumeWorld() const;
		bool RayCast(const Ray3& ray, Vec3& pos, const ModelTriangle** tri);
		bool CheckNarrowCollision(BoundingVolumePtr bv);
		Ray3::IResult CheckNarrowCollisionRay(const Ray3& ray);
		bool HasCollisionShapes() const;		
		CollisionShapeInfos GetCollisionShapeInfos() const;
		FBColShape::Enum GetColShapeType() const;
		const Vec3& GetColShapeOffset(unsigned idx) const;
		const Quat& GetColShapeRot(unsigned idx) const;
		const Vec3& GetColShapeScale(unsigned idx) const;
		const Vec3* GetMeshColShapePositions(unsigned dolShapeIdx, unsigned& outNumPosition) const;

		Vec3 GetRandomPosInVolume(const Vec3* nearWorld);
		void PlayAction(const char* action, bool immediate, bool reverse);
		bool IsPlayingAction() const;		
		bool IsActionDone(const char* action) const;

		unsigned GetNumMeshes() const;
		const Vec3& GetMeshOffset(unsigned idx) const;
		void SetMeshRotation(unsigned idx, const Quat& rot);		
		SpatialObjectPtr GetSpatialObject() const;
		void AddAsCloudVolume(ScenePtr scene);
	};
}