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
#include "SpatialObject.h"
#include "Scene.h"
#include "FBMathLib/BoundingVolume.h"
#include "FBAnimation/Animation.h"
#include "FBAnimation/AnimationData.h"
using namespace fastbird;

class SpatialObject::Impl{
public:
	Transformation mLocation;
	TransformationPtr mAnimatedLocation;
	BoundingVolumePtr mBoundingVolume;
	BoundingVolumePtr mBoundingVolumeWorld;
	Real mDistToCam;
	AnimationPtr mAnim;
	Vec3 mPreviousPosition;
	bool mTransformChanged;


	//---------------------------------------------------------------------------
	Impl()
		: mBoundingVolume(BoundingVolume::Create())
		, mBoundingVolumeWorld(BoundingVolume::Create())
		, mPreviousPosition(0, 0, 0)
		, mTransformChanged(true)
	{
	}

	Impl(const Impl& other)
		: Impl()
	{
		mLocation = other.mLocation;
		if (other.mAnimatedLocation){
			mAnimatedLocation = Transformation::Create();
			*mAnimatedLocation = *other.mAnimatedLocation;
		}
		*mBoundingVolume = *other.mBoundingVolume;
		*mBoundingVolumeWorld = *other.mBoundingVolumeWorld;
		mDistToCam = other.mDistToCam;
		if (other.mAnim){
			mAnim = other.mAnim->Clone();
		}
		mTransformChanged = other.mTransformChanged;
	}

	void SetRadius(Real r){
		mBoundingVolume->SetRadius(r);
		mBoundingVolumeWorld->SetRadius(r);
	}

	Real GetRadius() const{
		return mBoundingVolumeWorld->GetRadius();
	}

	void SetDistToCam(Real dist){
		mDistToCam = dist;
	}

	Real GetDistToCam() const{
		return mDistToCam;
	}

	const Vec3& GetPosition() const{
		return mLocation.GetTranslation();
	}

	const Vec3& GetPreviousPosition() const{
		return mPreviousPosition;
	}

	const Vec3& GetScale() const{
		return mLocation.GetScale();
	}

	Vec3 GetDirection() const{
		return mLocation.GetForward();
	}

	const Quat& GetRotation() const{
		return mLocation.GetRotation();
	}

	void SetPosition(const Vec3& pos){
		mPreviousPosition = mLocation.GetTranslation();
		mLocation.SetTranslation(pos);
		mBoundingVolumeWorld->SetCenter(mBoundingVolume->GetCenter() + pos);
		mTransformChanged = true;
	}

	void SetRotation(const Quat& rot){
		mLocation.SetRotation(rot);
		mTransformChanged = true;
	}

	void SetScale(const Vec3& scale){
		mBoundingVolumeWorld->SetRadius(mBoundingVolume->GetRadius() * std::max(scale.x, std::max(scale.y, scale.z)));
		mLocation.SetScale(scale);
		mTransformChanged = true;
	}

	void SetDirection(const Vec3& dir){
		mLocation.SetDirection(dir);
		mTransformChanged = true;
	}

	void SetDirectionAndRight(const Vec3& dir, const Vec3& right){
		mLocation.SetDirectionAndRight(dir, right);
		mTransformChanged = true;
	}

	BoundingVolumePtr GetBoundingVolume(){
		return mBoundingVolume;
	}

	BoundingVolumePtr GetBoundingVolumeWorld(){
		return mBoundingVolumeWorld;
	}

	const Transformation& GetLocation() const{
		return mLocation;
	}

	const Transformation& GetAnimatedLocation() const{
		return mAnim ? *mAnimatedLocation : mLocation;
	}

	AnimationPtr GetAnimation() const{
		return mAnim;
	}

	void SetLocation(const Transformation& t){
		mLocation = t;
	}

	bool GetTransformChanged() const{
		return mTransformChanged;
	}

	void ClearTransformChanged(){
		mTransformChanged = false;
	}

	void SetAnimation(AnimationPtr anim){
		mAnim = anim;
		if (!mAnimatedLocation){
			mAnimatedLocation = Transformation::Create();
		}
		mAnimatedLocation->MakeIdentity();
	}

	void UpdateAnimation(TIME_PRECISION dt){
		if (mAnim){
			mAnim->Update(dt);
			if (mAnim->Changed())
				*mAnimatedLocation = mLocation * mAnim->GetResult();
		}
	}

	bool IsPlayingAction() const{
		return mAnim && mAnim->IsPlaying();
	}

	void NotifyTransformChanged(){
		mTransformChanged = true;
	}

	void SetBoundingVolume(const BoundingVolume& src){
		*mBoundingVolume = src;
		*mBoundingVolumeWorld = *mBoundingVolume;
		mBoundingVolumeWorld->SetCenter(mBoundingVolume->GetCenter() + mLocation.GetTranslation());
		auto scale = mLocation.GetScale();
		mBoundingVolumeWorld->SetRadius(mBoundingVolume->GetRadius() * std::max(scale.x, std::max(scale.y, scale.z)));
	}

	void MergeBoundingVolume(const BoundingVolumePtr src){
		mBoundingVolume->Merge(src.get());
		*mBoundingVolumeWorld = *mBoundingVolume;
		mBoundingVolumeWorld->SetCenter(mBoundingVolume->GetCenter() + mLocation.GetTranslation());
		auto scale = mLocation.GetScale();
		mBoundingVolumeWorld->SetRadius(mBoundingVolume->GetRadius() * std::max(scale.x, std::max(scale.y, scale.z)));
	}
};

//---------------------------------------------------------------------------
SpatialObject::SpatialObject()
	: mImpl(new Impl)
{
}

SpatialObject::SpatialObject(const SpatialObject& other)
	: mImpl(new Impl(*other.mImpl)){

}

SpatialObject::~SpatialObject(){

}

void SpatialObject::SetRadius(Real r){
	mImpl->SetRadius(r);
}

Real SpatialObject::GetRadius() const{
	return mImpl->GetRadius();
}

void SpatialObject::SetDistToCam(Real dist){
	mImpl->SetDistToCam(dist);
}

Real SpatialObject::GetDistToCam() const{
	return mImpl->GetDistToCam();
}

const Vec3& SpatialObject::GetPosition() const{
	return mImpl->GetPosition();
}

const Vec3& SpatialObject::GetPreviousPosition() const{
	return mImpl->GetPreviousPosition();
}

const Vec3& SpatialObject::GetScale() const{
	return mImpl->GetScale();
}

Vec3 SpatialObject::GetDirection() const{
	return mImpl->GetDirection();
}

const Quat& SpatialObject::GetRotation() const{
	return mImpl->GetRotation();
}

void SpatialObject::SetPosition(const Vec3& pos){
	mImpl->SetPosition(pos);
}

void SpatialObject::SetRotation(const Quat& rot){
	mImpl->SetRotation(rot);
}

void SpatialObject::SetScale(const Vec3& scale){
	mImpl->SetScale(scale);
}

void SpatialObject::SetDirection(const Vec3& dir){
	mImpl->SetDirection(dir);
}

void SpatialObject::SetDirectionAndRight(const Vec3& dir, const Vec3& right){
	mImpl->SetDirectionAndRight(dir, right);
}

BoundingVolumePtr SpatialObject::GetBoundingVolume(){
	return mImpl->GetBoundingVolume();
}

BoundingVolumePtr SpatialObject::GetBoundingVolumeWorld(){
	return mImpl->GetBoundingVolumeWorld();
}


const Transformation& SpatialObject::GetLocation() const{
	return mImpl->GetLocation();
}

const Transformation& SpatialObject::GetAnimatedLocation() const{
	return mImpl->GetAnimatedLocation();
}

AnimationPtr SpatialObject::GetAnimation() const{
	return mImpl->GetAnimation();
}

void SpatialObject::SetLocation(const Transformation& t){
	mImpl->SetLocation(t);
}

bool SpatialObject::GetTransformChanged() const{
	return mImpl->GetTransformChanged();
}

void SpatialObject::ClearTransformChanged(){
	mImpl->ClearTransformChanged();
}

void SpatialObject::SetAnimation(AnimationPtr anim){
	mImpl->SetAnimation(anim);
}

void SpatialObject::UpdateAnimation(TIME_PRECISION dt){
	mImpl->UpdateAnimation(dt);
}

bool SpatialObject::IsPlayingAction() const{
	return mImpl->IsPlayingAction();
}

void SpatialObject::NotifyTransformChanged(){
	return mImpl->NotifyTransformChanged();
}

void SpatialObject::SetBoundingVolume(const BoundingVolume& src){
	mImpl->SetBoundingVolume(src);
}

void SpatialObject::MergeBoundingVolume(const BoundingVolumePtr src){
	mImpl->MergeBoundingVolume(src);
}