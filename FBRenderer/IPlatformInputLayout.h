#pragma once
#include "InputElementDesc.h"
#include "FBCommonHeaders/Types.h"
namespace fastbird
{
	DECLARE_SMART_PTR(IPlatformInputLayout);
	class IPlatformInputLayout
	{
	public:
		virtual void Bind() = 0;
		virtual void SetDebugName(const char*) = 0;

	protected:
		~IPlatformInputLayout(){}
	};
}