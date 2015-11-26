#include "stdafx.h"
#include "ParticleOptions.h"
#include "FBConsole/Console.h"
using namespace fastbird;

ParticleOptions::ParticleOptions(){
	MoveEditParticle = 0;
	FB_REGISTER_CVAR(MoveEditParticle, MoveEditParticle, CVAR_CATEGORY_CLIENT, "MoveEditParticle");
}
