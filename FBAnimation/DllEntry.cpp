#include "stdafx.h"
#include "FBTimer/Timer.h"
namespace fastbird{
	fastbird::Timer* gpTimer = 0;
	class FBAnimationDllentry{
	public:
		FBAnimationDllentry(){
			gpTimer = Timer::GetMainTimer().get();
		}		
	};
	static FBAnimationDllentry entry;
}