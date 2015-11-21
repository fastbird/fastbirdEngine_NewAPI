#include "stdafx.h"
#include "ParticleRenderKey.h"
namespace fastbird{
	ParticleRenderKey::ParticleRenderKey()
		: mGlow(false), mDepthFade(true)
	{
		memset(mTexturePath, 0, 256);
		memset(padding, 0, 2);
	}

	ParticleRenderKey::ParticleRenderKey(const char* texturePath, const BLEND_DESC& desc, bool glow, bool depthFade)
		:mGlow(glow), mBDesc(desc), mDepthFade(depthFade)
	{
		strcpy_s(mTexturePath, texturePath);
		memset(padding, 0, 2);
	}

	bool ParticleRenderKey::operator==(const ParticleRenderKey& other) const
	{
		return memcmp(this, &other, sizeof(ParticleRenderKey)) == 0;
	}

	bool ParticleRenderKey::operator<(const ParticleRenderKey& other) const
	{
		return memcmp(this, &other, sizeof(ParticleRenderKey)) < 0;
	}
}