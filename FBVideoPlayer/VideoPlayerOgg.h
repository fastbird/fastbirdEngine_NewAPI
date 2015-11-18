#pragma once
#include "FBCommonHeaders/Types.h"
#include "FBRenderer/IVideoPlayer.h"
#include <theora/theoradec.h>
namespace fastbird{
	DECLARE_SMART_PTR(VideoPlayerOgg);
	class VideoPlayerOgg : public IVideoPlayer{
		DECLARE_PIMPL_NON_COPYABLE(VideoPlayerOgg);
		VideoPlayerOgg();

	public:
		static VideoPlayerOggPtr Create();
		void Clear();

		//---------------------------------------------------------------------------
		// IVideoPlayer
		//---------------------------------------------------------------------------
		virtual bool PlayVideo(const char* path);
		virtual void StopVideo();
		virtual void RegisterVideoNotifier(VideoNotifierFunc func);
		virtual void SetDurationAfterFinish(TIME_PRECISION time);
		virtual void Update(TIME_PRECISION dt);
		virtual bool IsFinish() const;

		//---------------------------------------------------------------------------
		// Own
		//---------------------------------------------------------------------------


	private:
		friend void stripe_decoded(VideoPlayerOgg* player, th_ycbcr_buffer _src, int _fragy0, int _fragy_end);
		int buffer_data();
		int queue_page(ogg_page *page);
		void open_video(void);
		void StripeDecoded(th_ycbcr_buffer _src, int _fragy0, int _fragy_end);
		void Display();
	};
}