#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	class Transformation;
	class Vec3;
	class Quat;
	DECLARE_SMART_PTR(AnimationData);
	/** Represents an unique animation set.
	*/
	class AnimationData{
		DECLARE_PIMPL_NON_COPYABLE(AnimationData);
		AnimationData();

	public:
		static AnimationDataPtr Create();
		~AnimationData();

		enum PosComp
		{
			X,
			Y,
			Z
		};
		struct Action
		{			
			std::string mName;
			float mStartTime;
			float mEndTime;
			float mLength;
			bool mLoop;
			const Vec3* mPosStartEnd[2];
			const Quat* mRotStartEnd[2];

			Action();
		};

		void AddPosition(float time, float v, PosComp comp);
		void AddScale(float time, float v, PosComp comp);
		void AddRotEuler(float time, float v, PosComp comp);
		bool HasPosAnimation() const;
		bool HasRotAnimation() const;
		bool HasScaleAnimation() const;
		void SetName(const char* name);
		const char* GetName() const;
		void PickPos(float time, bool cycled, const Vec3** prev, const Vec3** next, float& interpol);
		void PickRot(float time, bool cycled, const Quat** prev, const Quat** next, float& interpol);
		void ToLocal(const Transformation& tolocal);
		bool ParseAction(const char* filename);
		Action* GetAction(const char* name) const;


	};
}