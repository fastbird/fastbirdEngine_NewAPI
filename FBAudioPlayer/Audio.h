#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	
	DECLARE_SMART_PTR(Audio);
	class FB_DLL_PUBLIC Audio{
		DECLARE_PIMPL_NON_COPYABLE(Audio);
		Audio();

	public:	
		static AudioPtr Create();
		void Clear();
		void InitOgg();
		// returning processed
		bool ProcessOggHeaderPacket(ogg_stream_state* unknownStream, ogg_packet* packet);
		bool PlayAudio(const char* path);
		void Update(float dt);

		// internal purpose
		bool ProcessOggHeader();
		int GetVorbisSerialno() const;
		ogg_stream_state* GetStreamStatePtr();
	};
}