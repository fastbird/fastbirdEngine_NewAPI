#pragma once

#include "FBCommonHeaders/Types.h"
#include "RayResult.h"

class btTypedConstraint;
class btCollisionShape;
namespace fastbird
{
	class Vec3;
	class Vec3I;
	class AABB;
	class Quat;
	class IDebugDrawer;
	class IWinBase;
	DECLARE_SMART_PTR(RigidBody);
	class IPhysicsInterface;
	DECLARE_SMART_PTR_STRUCT(CollisionShape);
	DECLARE_SMART_PTR_STRUCT(BoxShape);
	DECLARE_SMART_PTR_STRUCT(SphereShape);
	DECLARE_SMART_PTR_STRUCT(CylinderShape);
	DECLARE_SMART_PTR_STRUCT(CapsuleShape);
	DECLARE_SMART_PTR_STRUCT(MeshShape);
	struct IFilterCallback;
	typedef bool(*NeedCollisionForConvexCallback)(RigidBody* a, RigidBody* b);
	DECLARE_SMART_PTR(IPhysics);
	class FB_DLL_PHYSICS IPhysics
	{
	public:
		static IPhysicsPtr Create();
		static IPhysics& GetInstance();

		static const size_t MemAlign = 16;

		virtual ~IPhysics(){}

		virtual void Update(float dt) = 0;
		virtual void EnablePhysics() = 0;
		virtual void DisablePhysics() = 0;

		//virtual void CreateRigidBody(float mass, const Transformation& startTransform, 
			//btCollisionShape)

		virtual RigidBodyPtr CreateRigidBody(const char* collisionFile, float mass, IPhysicsInterface* obj) = 0;
		virtual RigidBodyPtr CreateRigidBody(IPhysicsInterface* colProvider) = 0;
		virtual RigidBodyPtr CreateRigidBodyForGroup(IPhysicsInterface* colProvider, const Vec3I& groupIdx) = 0;
		virtual RigidBodyPtr CreateTempRigidBody(CollisionShapePtr colShape) = 0;
		virtual RigidBodyPtr CreateTempRigidBody(CollisionShapePtr  shapes[], unsigned num) = 0;
		virtual void AddRef(btCollisionShape* colShape) = 0;
		virtual void Release(btCollisionShape* colShape) = 0;

		virtual void RemoveConstraint(btTypedConstraint* constraint) = 0;
		virtual void SetDebugDrawer(IDebugDrawer* debugDrawer) = 0;
		virtual void SetDebugMode(int debugMode) = 0;

		virtual void AttachBodies(RigidBodyPtr bodies[], unsigned num) = 0;
		virtual void AttachBodiesAlways(RigidBodyPtr bodies[], unsigned num) = 0;
		
		virtual void SetRayCollisionGroup(int group) = 0;
		virtual bool RayTestClosest(const Vec3& fromWorld, const Vec3& toWorld, int additionalGroupFlag, int mask, RayResultClosest& result, void* excepts[] = 0, unsigned numExcepts = 0) = 0;
		virtual bool RayTestWithAnObj(const Vec3& fromWorld, const Vec3& toWorld, int additionalGroupFlag, RayResultWithObj& result) = 0;
		// returning RayResultAll*
		// need to delete with IPhysics::Release(RayResultAll*)
		virtual RayResultAll* RayTestAll(const Vec3& fromWorld, const Vec3& toWorld, int additionalGroupFlag, int mask) = 0;

		virtual void Release(RayResultAll* r) = 0;

		virtual unsigned GetAABBOverlaps(const AABB& aabb, unsigned colMask, RigidBody* ret[], unsigned limit, RigidBody* except) = 0;

		virtual float GetDistanceBetween(RigidBodyPtr a, RigidBodyPtr b) = 0;

		virtual unsigned CreateBTSphereShape(float radius) = 0;
		virtual void DeleteBTShape(unsigned id) = 0;
		virtual void DrawDebugInfo() = 0;

		//-------------------------------------------------------------------
		// collision shape manager
		//-------------------------------------------------------------------
		virtual BoxShapePtr CreateBoxShape(const Vec3& pos, const Quat& rot, const Vec3& actorScale, const Vec3& extent, void* userPtr = 0) = 0;
		virtual SphereShapePtr CreateSphereShape(const Vec3& pos, const Quat& rot, const Vec3& actorScale, float radius, void* userPtr = 0) = 0;
		virtual CylinderShapePtr CreateCylinderShape(const Vec3& pos, const Quat& rot, const Vec3& actorScale, const Vec3& extent, void* userPtr = 0) = 0;
		virtual CapsuleShapePtr CreateCylinderShape(const Vec3& pos, const Quat& rot, const Vec3& actorScale, float radius, float height, void* userPtr = 0) = 0;
		virtual MeshShapePtr CreateMeshShape(const Vec3& pos, const Quat& rot, Vec3* vertices, unsigned numVertices, const Vec3& scale,
			bool staticObj, void* userPtr = 0) = 0;
		virtual MeshShapePtr CreateConvexMeshShape(const Vec3& pos, const Quat& rot, Vec3* vertices, unsigned numVertices,
			const Vec3& scale, void* userPtr = 0) = 0;
		
		virtual void RegisterFilterCallback(IFilterCallback* callback, NeedCollisionForConvexCallback func) = 0;		
	};
}