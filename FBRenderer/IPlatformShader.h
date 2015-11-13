#pragma once
#include "FBCommonHeaders/Types.h"
#include "ShaderDefines.h"
namespace fastbird
{
	DECLARE_SMART_PTR(IPlatformShader);
	class IPlatformShader
	{
	public:		
		virtual void Bind() = 0;
		virtual bool IsValid() const = 0;
		virtual bool GetCompileFailed() const = 0;
		virtual void* GetVSByteCode(unsigned& size) const = 0;		
		virtual const char* GetName() const = 0;
		virtual void SetShaderDefines(const SHADER_DEFINES& defines) = 0;
		virtual const SHADER_DEFINES& GetShaderDefines() const = 0;
		virtual void ApplyShaderDefines() = 0;
		virtual int GetBindingShaders() const = 0;
		virtual void SetDebugName(const char*) = 0;
		virtual bool CheckIncludes(const std::string& inc) = 0;

	protected:
		~IPlatformShader(){}
	};
}