#pragma once
#include "FBCommonHeaders/Types.h"
#include "FBSceneManager/SceneObjectFlag.h"
namespace fastbird{
	struct ModelTriangle;
	typedef std::vector< std::pair<std::string, Transformation> > AUXILIARIES;
	FB_DECLARE_SMART_PTR(BoundingVolume);
	FB_DECLARE_SMART_PTR(Material);
	FB_DECLARE_SMART_PTR(FacadeMeshObject);
	class FB_DLL_ENGINEFACADE FacadeMeshObject{
		FB_DECLARE_PIMPL_NON_COPYABLE(FacadeMeshObject);
		FacadeMeshObject();
		~FacadeMeshObject();

	public:
		static FacadeMeshObjectPtr Create();
		bool LoadDae(const char* daePath);

		void SetGameType(int type);
		void SetGameId(unsigned id);
		void SetGamePtr(void* ptr);
		/// Combination of SceneObjectFlag::Enum
		void ModifyObjFlag(unsigned flag, bool enable);		
		void SetEnableHighlight(bool enable);
		MaterialPtr GetMaterial() const;
		void SetMaterial(MaterialPtr material);
		void SetMaterial(const char* path);
		bool AttachToScene();
		bool DetachFromScene();
		bool IsAttached() const;
		void SetAlpha(float alpha);
		void SetForceAlphaBlending(bool enable, float alpha, float forceGlow, bool disableDepth);
		void SetAmbientColor(const Color& color);
		const AUXILIARIES* GetAuxiliaries() const;
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
		Vec3 GetRandomPosInVolume(const Vec3* nearWorld);
		void PlayAction(const char* action, bool immediate, bool reverse);
		bool IsPlayingAction() const;		
	};
}