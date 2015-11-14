#include "stdafx.h"
#include "TextureBinding.h"
using namespace fastbird;

bool TextureBinding::operator==(const TextureBinding& other) const{
	return memcmp(this, &other, sizeof(TextureBinding)) == 0;
}
bool TextureBinding::operator<(const TextureBinding& other) const{
	return memcmp(this, &other, sizeof(TextureBinding)) < 0;
}