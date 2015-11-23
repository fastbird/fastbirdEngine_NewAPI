#pragma once

class btTriangleMesh;
namespace fastbird
{
	class CollisionShapes{
	public:
		enum Enum {
			Box,
			Sphere,
			Cylinder,
			Capsule,
			StaticMesh,
			DynamicMesh,
			Convex,

			Num,
		};

		static const char* strings[];
		static const char* ConvertToString(Enum a);
		static Enum ConvertToEnum(const char* str);
	};

	DECLARE_SMART_PTR_STRUCT(CollisionShape);
	struct CollisionShape
	{
	protected:
		CollisionShape();
		~CollisionShape();
	public:
		virtual void ChangeScale(const Vec3& scale);

		CollisionShapes::Enum mType;
		Vec3 mPos;
		Quat mRot;
		Vec3 mScale;
		void* mUserPtr;
	};

	DECLARE_SMART_PTR_STRUCT(BoxShape);
	struct BoxShape : public CollisionShape
	{
	private:
		BoxShape();
		~BoxShape();

	public:
		static BoxShapePtr Create();
		void ChangeScale(const Vec3& scale);
		Vec3 mExtent;		
	};

	DECLARE_SMART_PTR_STRUCT(SphereShape);
	struct SphereShape : public CollisionShape
	{
	private:
		SphereShape();
		~SphereShape();

	public:
		static SphereShapePtr Create();
		void ChangeScale(const Vec3& scale);
		float mRadius;
	};

	DECLARE_SMART_PTR_STRUCT(CylinderShape);
	struct CylinderShape : public CollisionShape
	{
	private:
		CylinderShape();
		~CylinderShape();

	public:
		static CylinderShapePtr Create();
		void ChangeScale(const Vec3& scale);
		Vec3 mExtent;
	};

	DECLARE_SMART_PTR_STRUCT(CapsuleShape);
	struct CapsuleShape : public CollisionShape
	{
	private:
		CapsuleShape();
		~CapsuleShape();

	public:
		static CapsuleShapePtr Create();
		void ChangeScale(const Vec3& scale);
		float mRadius;
		float mHeight;
	};

	DECLARE_SMART_PTR_STRUCT(MeshShape);
	struct MeshShape : public CollisionShape
	{
	private:
		MeshShape();
		~MeshShape();

	public:
		static MeshShapePtr Create();
		void ChangeScale(const Vec3& scale);

		Vec3* mVertices;
		unsigned mNumVertices;
		btTriangleMesh* GetTriangleMesh();

	private:
		void CreateTriangleMesh();
		btTriangleMesh* mTriangleMesh;
	};

	class CollisionShapeMan
	{
	public:
		static BoxShapePtr CreateBoxShape(const Vec3& pos, const Quat& rot, const Vec3& actorScale,const Vec3& extent, void* userPtr = 0);
		static SphereShapePtr CreateSphereShape(const Vec3& pos, const Quat& rot, const Vec3& actorScale, float radius, void* userPtr = 0);
		static CylinderShapePtr CreateCylinderShape(const Vec3& pos, const Quat& rot, const Vec3& actorScale, const Vec3& extent, void* userPtr = 0);
		static CapsuleShapePtr CreateCylinderShape(const Vec3& pos, const Quat& rot, const Vec3& actorScale, float radius, float height, void* userPtr = 0);
		static MeshShapePtr CreateMeshShape(const Vec3& pos, const Quat& rot, Vec3* vertices, unsigned numVertices, const Vec3& scale,
			bool staticObj, void* userPtr = 0);
		static MeshShapePtr CreateConvexMeshShape(const Vec3& pos, const Quat& rot, Vec3* vertices, unsigned numVertices, 
			const Vec3& scale, void* userPtr = 0);		
	};
}