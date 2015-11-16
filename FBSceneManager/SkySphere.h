#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	DECLARE_SMART_PTR(SkySphere);
	class SkySphere{
		DECLARE_PIMPL_NON_COPYABLE(SkySphere);
		SkySphere();

	public:
		static SkySpherePtr Create();
		static void CreateSharedEnvRT();
		static void DeleteSharedEnvRT();

		~SkySphere();
	};
}