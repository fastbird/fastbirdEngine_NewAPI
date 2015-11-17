/*
 -----------------------------------------------------------------------------
 This source file is part of fastbird engine
 For the latest info, see http://www.jungwan.net/
 
 Copyright (c) 2013-2015 Jungwan Byun
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
*/

#pragma once
#include <set>
#include "FBCommonHeaders/Types.h"
#include "ShaderDefines.h"
#include "IPlatformShader.h"
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