#pragma once
#include "RendererEnums.h"
#include <vector>
namespace fastbird{
	struct TextureBinding{
		bool operator==(const TextureBinding& other) const;
		bool operator<(const TextureBinding& other) const;
		BINDING_SHADER mShader;
		int mSlot;
	};

	typedef std::vector<TextureBinding> TextureBindings;
}