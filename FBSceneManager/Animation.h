#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	class Transformation;
	DECLARE_SMART_PTR(AnimationData);
	DECLARE_SMART_PTR(Animation);
	/** Represents an animation instance using the AnimationData.
	*/
	class Animation{
		DECLARE_PIMPL_CLONEABLE(Animation);
		Animation();

	public:
		static AnimationPtr Create();
		static AnimationPtr Create(const Animation& other);
		~Animation();

		AnimationPtr Clone() const;
		void PlayAction(const std::string& name, bool immediate, bool reverse);
		bool IsActionDone(const char* action) const;
		bool IsPlaying() const;
		void Update(float dt);
		const Transformation& GetResult() const;
		bool Changed() const;
		void SetAnimationData(AnimationDataPtr data);

	};
}