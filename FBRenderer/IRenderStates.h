#pragma once
#include <memory>
#include "RendererEnums.h"
#if !defined(DECLARE_SMART_PTR)
#define DECLARE_SMART_PTR(className) \
	class className;\
	typedef std::shared_ptr<className> className##Ptr;\
	typedef std::weak_ptr<className> className##WeakPtr
#endif

namespace fastbird
{
	//------------------------------------------------------------------------
	class IRasterizerState
	{
	public:
		virtual ~IRasterizerState() {}
		virtual void Bind() = 0;

		virtual void SetDebugName(const char* name) = 0;
	};
	DECLARE_SMART_PTR(IRasterizerState);

	//------------------------------------------------------------------------
	class IBlendState
	{
	public:
		virtual ~IBlendState() {}
		virtual void Bind() = 0;
		virtual void SetDebugName(const char* name) = 0;
	};
	DECLARE_SMART_PTR(IBlendState);

	//------------------------------------------------------------------------
	class IDepthStencilState
	{
	public:
		virtual ~IDepthStencilState() {}
		virtual void Bind(unsigned stencilRef) = 0;
		virtual void SetDebugName(const char* name) = 0;
	};
	DECLARE_SMART_PTR(IDepthStencilState);

	//------------------------------------------------------------------------
	class ISamplerState
	{
	public:
		virtual ~ISamplerState() {}
		virtual void Bind(BINDING_SHADER shader, int slot) = 0;
		virtual void SetDebugName(const char* name) = 0;
	};
	DECLARE_SMART_PTR(ISamplerState);
}