#include "stdafx.h"
#include "DirectionalLight.h"
using namespace fastbird;

class DirectionalLight::Impl{
public:
	Vec3 mDirection;
	Vec3 mDiffuse;
	Vec3 mSpecular;
	Real mTheta;
	Real mPhi;
	Real mIntensity;

	// for interpolation
	Real mInterpolPhi[2]; // 0 cur, //1 dest
	Real mInterpolTheta[2];
	Real mInterpolIntensity[2];
	Vec3 mInterpolDiffuse[2];
	Real mInterpolTime[2];
	bool mInterpolating;

	//----------------------------------------------------------------------------
	Impl()
		: mTheta(0)
		, mPhi(0)
		, mInterpolating(false)
	{
	}

	void AddTheta(Real radian){
		if (radian == 0.f)
			return;
		mTheta += radian;
		if (mTheta > PI)
		{
			mTheta = mTheta - PI;
		}

		mDirection = SphericalToCartesian(mTheta, mPhi);
	}
	void AddPhi(Real radian){
		if (radian == 0.f)
			return;
		mPhi += radian;
		if (mPhi >= TWO_PI)
		{
			mPhi = mPhi - TWO_PI;
		}

		mDirection = SphericalToCartesian(mTheta, mPhi);
	}

	void SetPosition(const Vec3& pos){
		mDirection = pos;
		mDirection.Normalize();

		mTheta = acos(mDirection.z);
		mPhi = atan2(mDirection.y, mDirection.x);
		mPhi += mPhi < 0 ? TWO_PI : 0;
	}

	//----------------------------------------------------------------------------
	void SetDiffuse(const Vec3& diffuse)
	{
		mDiffuse = diffuse;
	}

	//----------------------------------------------------------------------------
	void SetSpecular(const Vec3& specular)
	{
		mSpecular = specular;
	}

	//----------------------------------------------------------------------------
	void SetIntensity(Real intensity)
	{
		mIntensity = intensity;
	}

	//----------------------------------------------------------------------------
	void PrepareInterpolation(Real destTheta, Real destPhi, Real destIntensity, const Vec3& destDiffuse, Real time)
	{
		mInterpolPhi[0] = mPhi;
		mInterpolTheta[0] = mTheta;
		mInterpolIntensity[0] = mIntensity;
		mInterpolDiffuse[0] = mDiffuse;

		mInterpolPhi[1] = destPhi;
		mInterpolTheta[1] = destTheta;
		mInterpolIntensity[1] = destIntensity;
		mInterpolDiffuse[1] = destDiffuse;

		mInterpolTime[0] = time;
		mInterpolTime[1] = 0.f;
		mInterpolating = true;
	}

	void AddInterpolTime(Real time)
	{
		mInterpolTime[1] += time;
	}

	const Vec3& GetInterpolDir(unsigned target) const
	{
		assert(target <= 1);
		return SphericalToCartesian(mInterpolTheta[target], mInterpolPhi[target]);
	}
	Real GetInterpolIntensity(unsigned target) const
	{
		assert(target <= 1);
		return mInterpolIntensity[target];
	}
	const Vec3& GetInterpolDiffuse(unsigned target) const
	{
		assert(target <= 1);
		return mInterpolDiffuse[target];
	}

	Real GetTheta() const
	{
		return mTheta;
	}
	Real GetPhi() const
	{
		return mPhi;
	}

	void Update(Real dt)
	{
		if (mInterpolating)
		{
			mInterpolTime[1] += dt;
			if (mInterpolTime[1] > mInterpolTime[0])
			{
				mInterpolTime[1] = mInterpolTime[0];
				mInterpolating = false;
			}

			Real normTime = mInterpolTime[1] / mInterpolTime[0];
			mPhi = Lerp(mInterpolPhi[0], mInterpolPhi[1], normTime);
			mTheta = Lerp(mInterpolTheta[0], mInterpolTheta[1], normTime);
			mIntensity = Lerp(mInterpolIntensity[0], mInterpolIntensity[1], normTime);
			mDiffuse = Lerp(mInterpolDiffuse[0], mInterpolDiffuse[1], normTime);
			mDirection = SphericalToCartesian(mTheta, mPhi);
		}
	}

	void CopyLight(DirectionalLight* other)
	{
		if (!other)
			return;

		mDirection = other->mImpl->mDirection;
		mDiffuse = other->mImpl->mDiffuse;
		mSpecular = other->mImpl->mSpecular;
		mTheta = other->mImpl->mTheta;
		mPhi = other->mImpl->mPhi;
		mIntensity = other->mImpl->mIntensity;
		for (int i = 0; i < 2; i++)
		{
			mInterpolPhi[i] = other->mImpl->mInterpolPhi[i];
			mInterpolTheta[i] = other->mImpl->mInterpolTheta[i];
			mInterpolIntensity[i] = other->mImpl->mInterpolIntensity[i];
			mInterpolDiffuse[i] = other->mImpl->mInterpolDiffuse[i];
			mInterpolTime[i] = other->mImpl->mInterpolTime[i];
		}
		mInterpolating = other->mImpl->mInterpolating;
	}

};

//----------------------------------------------------------------------------
DirectionalLight::DirectionalLight()
	:mImpl(new Impl)
{
}

//----------------------------------------------------------------------------
DirectionalLight::~DirectionalLight()
{
}

const Vec3& DirectionalLight::GetPosition(){
	return mImpl->mDirection;
}

const Vec3& DirectionalLight::GetDiffuse(){
	return mImpl->mDiffuse;
}

const Vec3& DirectionalLight::GetSpecular(){
	return mImpl->mSpecular;
}

Real DirectionalLight::GetIntensity() const{
	return mImpl->mIntensity;
}

Real DirectionalLight::GetAttenuation() const{
	return 0.;
}

Real DirectionalLight::GetExponent() const{
	return 0.;
}

//----------------------------------------------------------------------------
void DirectionalLight::SetPosition(const Vec3& pos){
	mImpl->SetPosition(pos);
}

//----------------------------------------------------------------------------
void DirectionalLight::SetDiffuse(const Vec3& diffuse){
	mImpl->SetDiffuse(diffuse);
}

//----------------------------------------------------------------------------
void DirectionalLight::SetSpecular(const Vec3& specular){
	mImpl->mSpecular = specular;
}

//----------------------------------------------------------------------------
void DirectionalLight::SetIntensity(Real intensity){
	mImpl->mIntensity = intensity;
}

//----------------------------------------------------------------------------
void DirectionalLight::AddTheta(Real radian){
	mImpl->AddTheta(radian);
}

//----------------------------------------------------------------------------
void DirectionalLight::AddPhi(Real radian){
	mImpl->AddPhi(radian);
}

//----------------------------------------------------------------------------
void DirectionalLight::PrepareInterpolation(Real destTheta, Real destPhi, Real destIntensity, const Vec3& destDiffuse, Real time){
	mImpl->PrepareInterpolation(destTheta, destPhi, destIntensity, destDiffuse, time);
}

void DirectionalLight::AddInterpolTime(Real time){
	mImpl->mInterpolTime[1] += time;
}

const Vec3& DirectionalLight::GetInterpolDir(unsigned target) const{
	return mImpl->GetInterpolDir(target);	
}
Real DirectionalLight::GetInterpolIntensity(unsigned target) const
{
	return mImpl->GetInterpolIntensity(target);
}
const Vec3& DirectionalLight::GetInterpolDiffuse(unsigned target) const
{
	return mImpl->GetInterpolDiffuse(target);
}

Real DirectionalLight::GetTheta() const{
	return mImpl->GetTheta();
}
Real DirectionalLight::GetPhi() const{
	return mImpl->GetPhi();
}

void DirectionalLight::Update(Real dt){
	mImpl->Update(dt);
}

void DirectionalLight::CopyLight(DirectionalLight* other_)
{
	mImpl->CopyLight(other_);
}