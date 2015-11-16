#include "stdafx.h"
#include "FBCollisionShape.h"
#include "FBStringLib/StringLib.h"
#include "FBMathLib/AABB.h"
#include "FBMathLib/BVaabb.h"
#include "FBDebugLib/Logger.h"
#include <assert.h>
using namespace fastbird;

class FBCollisionShape::Impl{
public:
	FBColShape::Enum mColShape;
	BoundingVolumePtr mBV;
	MeshObjectPtr mColMesh;
	Transformation mTransformation;

	//---------------------------------------------------------------------------
	Impl(FBColShape::Enum e, const Transformation& t, MeshObjectPtr colMesh)
		: mColShape(e)
		, mColMesh(colMesh)
		, mTransformation(t)
	{
		switch (e)
		{
		case FBColShape::SPHERE:
		{
			mBV = BoundingVolume::Create(BoundingVolume::BV_SPHERE);
			auto scale = t.GetScale();
			if (scale.x != scale.z || scale.x != scale.y)
			{
				Logger::Log(FB_ERROR_LOG_ARG, "Collision Sphere should be uniform scaled!");
				assert(0);
			}
			mBV->SetRadius(1 * t.GetScale().x);
			mBV->SetCenter(Vec3::ZERO);
		}
		break;

		case FBColShape::CUBE:
		{
			mBV = BoundingVolume::Create(BoundingVolume::BV_AABB);
			AABB aabb;
			aabb.SetMax(Vec3(1, 1, 1) * t.GetScale());
			aabb.SetMin(Vec3(-1, -1, -1) * t.GetScale());
			BVaabb* bvaabb = (BVaabb*)mBV.get();
			bvaabb->SetAABB(aabb);
		}
		break;

		default:
			break;
		}
	}

	void SetCollisionMesh(MeshObjectPtr colMesh) { 
		mColMesh = colMesh; 
	}

	MeshObjectPtr GetCollisionMesh() const { 
		return mColMesh; 
	}

	BoundingVolumePtr GetBV() const { 
		return mBV; 
	}

	FBColShape::Enum GetColShape() const { 
		return mColShape; 
	}

	Vec3 GetOffset() const{
		return mTransformation.GetTranslation();
	}

	Quat GetRot() const{
		return mTransformation.GetRotation();
	}

	Vec3 GetScale() const{
		return mTransformation.GetScale();
	}

	typedef std::pair<bool, Real> IResult;
	IResult Intersects(const Ray3& ray, const Transformation& objT) const{
		if (!mBV)
			return IResult(false, FLT_MAX);
		Ray3 localRay = objT.ApplyInverse(ray);
		Ray3::IResult ret = localRay.Intersects(mBV);
		return ret;
	}

	bool TestCollision(BoundingVolumeConstPtr pBV, const Transformation& objT) const{
		if (!mBV)
			return false;

		auto newCenter = objT.ApplyInverse(pBV->GetCenter());
		Real newRad = pBV->GetRadius();
		BoundingVolumePtr localBV = BoundingVolume::Create();
		localBV->SetCenter(newCenter);
		localBV->SetRadius(newRad);
		return mBV->TestIntersection(localBV);
	}

	Vec3 GetRandomPosInVolume(const Vec3* nearWorld, const Transformation& objT) const{
		if (!mBV)
		{
			return Vec3(0, 0, 0);
		}

		if (nearWorld)
		{
			Vec3 nearLocal = objT.ApplyInverse(*nearWorld);
			return mBV->GetRandomPosInVolume(&nearLocal);
		}
		return mBV->GetRandomPosInVolume();
	}
};

//---------------------------------------------------------------------------
FBCollisionShapePtr FBCollisionShape::Create(FBColShape::Enum e, const Transformation& t, MeshObjectPtr colMesh){
	return FBCollisionShapePtr(new FBCollisionShape(e, t, colMesh), [](FBCollisionShape* obj){ delete obj; });
}

FBCollisionShape::FBCollisionShape(FBColShape::Enum e, const Transformation& t, MeshObjectPtr colMesh)
	: mImpl(new Impl(e, t, colMesh))
{
}

FBCollisionShape::FBCollisionShape(const FBCollisionShape& other)
: mImpl(new Impl(*other.mImpl))
{
}

FBCollisionShape& FBCollisionShape::operator = (const FBCollisionShape& other){
	*mImpl = *other.mImpl;
	return *this;
}

FBCollisionShape::~FBCollisionShape(){

}

void FBCollisionShape::SetCollisionMesh(MeshObjectPtr colMesh){
	mImpl->SetCollisionMesh(colMesh);
}

MeshObjectPtr FBCollisionShape::GetCollisionMesh() const{
	return mImpl->GetCollisionMesh();
}

BoundingVolumePtr FBCollisionShape::GetBV() const{
	return mImpl->GetBV();
}

FBColShape::Enum FBCollisionShape::GetColShape() const{
	return mImpl->GetColShape();
}

Vec3 FBCollisionShape::GetOffset() const{
	return mImpl->GetOffset();
}

Quat FBCollisionShape::GetRot() const{
	return mImpl->GetRot();
}

Vec3 FBCollisionShape::GetScale() const{
	return mImpl->GetScale();
}

FBCollisionShape::IResult FBCollisionShape::Intersects(const Ray3& ray, const Transformation& objT) const{
	return mImpl->Intersects(ray, objT);
}

bool FBCollisionShape::TestCollision(BoundingVolumeConstPtr pBV, const Transformation& objT) const{
	return mImpl->TestCollision(pBV, objT);
}

Vec3 FBCollisionShape::GetRandomPosInVolume(const Vec3* nearWorld, const Transformation& objT) const{
	return mImpl->GetRandomPosInVolume(nearWorld, objT);
}

//---------------------------------------------------------------------------
const char* FBColShape::ConvertToString(FBColShape::Enum e){
		assert(e >= SPHERE && e < Num);
		return strings[e];
}

FBColShape::Enum FBColShape::ConvertToEnum(const char* str){
	for (int i = 0; i < Num; ++i)
	{
		if (_stricmp(strings[i], str) == 0)
			return Enum(i);
	}
	assert(0);
	return Num;
}
