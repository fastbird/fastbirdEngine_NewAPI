#pragma once
#include <functional>
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	DECLARE_SMART_PTR(IVideoPlayer);
	class IVideoPlayer{
	public:
		virtual ~IVideoPlayer(){}
		typedef std::function<void(IVideoPlayer*)> VideoNotifierFunc;
		virtual bool PlayVideo(const char* path) = 0;
		virtual void StopVideo() = 0;

		virtual void RegisterVideoNotifier(VideoNotifierFunc func) = 0;
		virtual void SetDurationAfterFinish(TIME_PRECISION time) = 0;
		// Update called internally.
		virtual void Update(TIME_PRECISION dt) = 0;

		virtual bool IsFinish() const = 0;
	};
}