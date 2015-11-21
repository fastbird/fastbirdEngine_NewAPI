#pragma once
#include "FBRenderer/RendererStructs.h"
namespace fastbird{
	struct ParticleRenderKey{
		char mTexturePath[256];
		BLEND_DESC mBDesc;
		bool mGlow;
		bool mDepthFade;
		char padding[2];

		ParticleRenderKey();
		ParticleRenderKey(const char* texturePath, const BLEND_DESC& desc, bool glow, bool depthFade);
		bool operator==(const ParticleRenderKey& other) const;
		bool operator<(const ParticleRenderKey& other) const;
	};
}