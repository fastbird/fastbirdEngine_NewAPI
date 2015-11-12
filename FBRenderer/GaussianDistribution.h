#pragma once
#include "FBCommonHeaders/platform.h"
#include "FBMathLib/Vec4.h"
namespace fastbird
{
	DECLARE_SMART_PTR(GaussianDist);
	class GaussianDist
	{
	public:
		Vec4 mGaussianDistOffsetX[15];
		Vec4 mGaussianDistWeightX[15];
		Vec4 mGaussianDistOffsetY[15];
		Vec4 mGaussianDistWeightY[15];

		void Calc(unsigned width, unsigned height, float devi, float multiplier);
	};
}