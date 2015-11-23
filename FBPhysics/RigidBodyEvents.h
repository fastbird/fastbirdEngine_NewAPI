#pragma once
namespace fastbird{
	struct RigidBodyEvents
	{
		RigidBodyEvents();
		float mForward;
		float mBackward;
		float mLeft;
		float mRight;
		float mUp;
		float mDown;

		float mRollRight; // clockWise
		float mRollLeft;

		float mHori;
		float mVert;
	};
}