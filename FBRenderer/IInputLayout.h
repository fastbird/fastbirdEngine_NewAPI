#pragma once
#include "InputElementDesc.h"
namespace fastbird
{
	class IInputLayout
	{
	public:
		virtual ~IInputLayout(){}
		virtual void SetDescs(const INPUT_ELEMENT_DESCS& descs) = 0;
		virtual const INPUT_ELEMENT_DESCS& GetDescs() const = 0;

		virtual bool HasVertexComponent(VERTEX_COMPONENT com) = 0;

		virtual void Bind() = 0;

		// do not call directly. use FB_SET_DEVICE_DEBUG_NAME define.
		virtual void SetDebugName(const char*) = 0;
	};
}