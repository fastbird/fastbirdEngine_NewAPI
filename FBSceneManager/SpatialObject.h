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

#pragma once
#include "SceneObject.h"
#include "FBCommonHeaders/Types.h"
#include "FBMathLib/Math.h"
#include "FBMathLib/BoundingVolume.h"
namespace fastbird{	
	DECLARE_SMART_PTR(Animation);
	DECLARE_SMART_PTR(SpatialObject);
	class FB_DLL_PUBLIC SpatialObject : public SceneObject{
		DECLARE_PIMPL_CLONEABLE(SpatialObject);		

	protected:
		SpatialObject();
		~SpatialObject();

	public:		
		void SetRadius(Real r);
		Real GetRadius() const;
		void SetDistToCam(Real dist);
		Real GetDistToCam() const;
		const Vec3& GetPosition() const;
		const Vec3& GetScale() const;
		void SetPosition(const Vec3& pos);
		BoundingVolumePtr GetBoundingVolume();
		BoundingVolumePtr GetBoundingVolumeWorld();
		const Transformation& GetLocation() const;
		const Transformation& GetAnimatedLocation() const;
		void SetLocation(const Transformation& t);
		bool GetTransformChanged() const;
		void ClearTransformChanged();
		void SetAnimation(AnimationPtr anim);
		void UpdateAnimation(TIME_PRECISION dt);
		bool IsPlayingAction() const;
	};
}