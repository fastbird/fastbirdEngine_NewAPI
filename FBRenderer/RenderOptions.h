#pragma once
#include "FBMathLib/Vec2I.h"
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	DECLARE_SMART_PTR(RenderOptions);
	class RenderOptions{
	public:
		int r_UI;
		int r_noObjectConstants;
		int r_noMesh;
		int r_noSky;
		int r_noParticleDraw;
		int r_particleProfile;
		int r_HDR;
		float r_HDRMiddleGray;
		int r_HDRCpuLuminance;
		int r_HDRFilmic;
		float r_BloomPower;
		float r_StarPower;
		int r_GodRay;
		float r_GodRayWeight;
		float r_GodRayDecay;
		float r_GodRayDensity;
		float r_GodRayExposure;
		int r_Glow;
		int r_ReportDeviceObjectLeak;
		int r_Shadow;
		int r_ShadowMapWidth;
		int r_ShadowMapHeight;
		float r_ShadowCamWidth;
		float r_ShadowCamHeight;
		float r_ShadowNear;
		float r_ShadowFar;
		float r_ShadowCamDist;

		int r_UseShaderCache;
		int r_GenerateShaderCache;
		int r_numRenderTargets;
		int r_numParticleEmitters;
		int r_debugDraw;
		int r_gameId;
		Vec2I r_resolution;
		int r_fullscreen; // 0 : window, 1 : full-screen, 2 : fake full-screen
		int r_noText;
	};
}