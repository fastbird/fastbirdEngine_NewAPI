#pragma once
#include <set>
#include "FBCommonHeaders/Types.h"
#include "ShaderDefines.h"
namespace fastbird
{
	DECLARE_SMART_PTR(IPlatformShader);
	DECLARE_SMART_PTR(Shader);
	class FB_DLL_PUBLIC Shader
	{
		DECLARE_PIMPL(Shader);
		Shader();

	public:
		static ShaderPtr Create();
		static void ReloadShader(const char* name);
		static void ReloadShader(const char* name, const SHADER_DEFINES& shaderDefines);

		~Shader();		
		void Bind();
		bool IsValid() const;
		bool GetCompileFailed() const;
		void* GetVSByteCode(unsigned& size) const;
		const char* GetName() const;
		void SetShaderDefines(const SHADER_DEFINES& defines);
		const SHADER_DEFINES& GetShaderDefines() const;
		void ApplyShaderDefines();
		int GetBindingShaders() const;
		void SetDebugName(const char* debugName);
		bool CheckIncludes(const char* shaderHeaderFile) const;
		void SetPlatformShader(IPlatformShaderPtr shader);
		IPlatformShaderPtr GetPlatformShader() const;
	};
}