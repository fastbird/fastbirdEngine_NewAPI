#pragma once
#include "FBMathLib/Transformation.h"
namespace fastbird
{
	namespace FBColShape
	{
		enum Enum
		{
			SPHERE,
			CUBE,
			MESH,

			Num
		};

		static const char* strings[] =
		{ "SPHERE", "CUBE", "MESH" };

		static const char* ConvertToString(Enum e);
		static Enum ConvertToEnum(const char* str);
	}

	typedef std::pair<FBColShape::Enum, Transformation> COL_SHAPE;

	DECLARE_SMART_PTR(MeshObject);
	DECLARE_SMART_PTR(FBCollisionShape);
	class FBCollisionShape
	{
		DECLARE_PIMPL(FBCollisionShape);
		FBCollisionShape(FBColShape::Enum e, const Transformation& t, MeshObjectPtr colMesh);

	public:
		static FBCollisionShapePtr Create(FBColShape::Enum e, const Transformation& t, MeshObjectPtr colMesh);
		FBCollisionShape(const FBCollisionShape& other);
		FBCollisionShape& operator=(const FBCollisionShape& other);
		~FBCollisionShape();
		

		void SetCollisionMesh(MeshObjectPtr colMesh);
		MeshObjectPtr GetCollisionMesh() const;
		BoundingVolumePtr GetBV() const;
		FBColShape::Enum GetColShape() const;
		Vec3 GetOffset() const;
		Quat GetRot() const;
		Vec3 GetScale() const;
		typedef std::pair<bool, Real> IResult;
		IResult Intersects(const Ray3& ray, const Transformation& objT) const;
		bool TestCollision(BoundingVolumeConstPtr pBV, const Transformation& objT) const;
		Vec3 GetRandomPosInVolume(const Vec3* nearWorld, const Transformation& objT) const;

	private:
		


	};
}