#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	FB_DECLARE_SMART_PTR(ParticleOptions);
	class ParticleOptions{
		ParticleOptions();
		~ParticleOptions();

	public:
		static ParticleOptionsPtr Create();

		int MoveEditParticle;
	};
}