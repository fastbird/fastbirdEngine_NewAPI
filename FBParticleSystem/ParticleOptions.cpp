#include "stdafx.h"
#include "ParticleOptions.h"
#include "FBConsole/Console.h"
using namespace fastbird;

FB_IMPLEMENT_STATIC_CREATE(ParticleOptions)
ParticleOptions::ParticleOptions(){
	MoveEditParticle = 0;
	FB_REGISTER_CVAR(MoveEditParticle, MoveEditParticle, CVAR_CATEGORY_CLIENT, "MoveEditParticle");
}

ParticleOptions::~ParticleOptions(){

}