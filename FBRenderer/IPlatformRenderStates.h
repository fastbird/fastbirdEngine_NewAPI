#pragma once
#include "FBCommonHeaders/Types.h"
#include "RendererEnums.h"
namespace fastbird
{
	DECLARE_SMART_PTR(IPlatformRasterizerState);
	class IPlatformRasterizerState
	{
	public:
		virtual void Bind() = 0;
		virtual void SetDebugName(const char* name) = 0;

	protected:
		~IPlatformRasterizerState() {}
	};

	DECLARE_SMART_PTR(IPlatformBlendState);
	class IPlatformBlendState
	{
	public:
		virtual void Bind() = 0;
		virtual void SetDebugName(const char* name) = 0;

	protected:
		~IPlatformBlendState() {}
	};

	DECLARE_SMART_PTR(IPlatformDepthStencilState);
	class IPlatformDepthStencilState
	{
	public:		
		virtual void Bind(unsigned stencilRef) = 0;
		virtual void SetDebugName(const char* name) = 0;

	protected:
		~IPlatformDepthStencilState() {}
	};
	
	DECLARE_SMART_PTR(IPlatformSamplerState);
	class IPlatformSamplerState
	{
	public:
		virtual void Bind(BINDING_SHADER shader, int slot) = 0;
		virtual void SetDebugName(const char* name) = 0;

	protected:
		~IPlatformSamplerState() {}
	};
}