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
#ifndef _fastbird_ShaderD3D11_header_included_
#define _fastbird_ShaderD3D11_header_included_

#include "FBRenderer/IPlatformShader.h"
namespace fastbird
{
	DECLARE_SMART_PTR(ShaderD3D11);
	class ShaderD3D11 : public IPlatformShader
	{
		DECLARE_PIMPL_NON_COPYABLE(ShaderD3D11);
		ShaderD3D11();

	public:
		static ShaderD3D11Ptr Create(const char* name);
		~ShaderD3D11();

	public:
		
		void SetVertexShader(ID3D11VertexShader* pVertexShader);
		void SetVertexShaderBytecode(ID3DBlob* pVertexShaderBytecode);
		void SetVertexShaderBytecode(void* bytecode, size_t size);
		void SetGeometryShader(ID3D11GeometryShader* pGeometryShader);
		void SetHullShader(ID3D11HullShader* pHullShader);
		void SetDomainShader(ID3D11DomainShader* pDomainShader);
		void SetPixelShader(ID3D11PixelShader* pPixelShader);
		virtual void Bind();
		virtual void BindVS();
		virtual void BindGS();
		virtual void BindPS();
		virtual void BindDS();
		virtual void BindHS();
		virtual void* GetVSByteCode(unsigned& size) const;

		ID3D11VertexShader* GetVertexShader() const { return m_pVertexShader; }
		ID3D11GeometryShader* GetGeometryShader() const { return m_pGeometryShader; }
		ID3D11HullShader* GetHullShader() const { return m_pHullShader; }
		ID3D11DomainShader* GetDomainShader() const { return m_pDomainShader; }
		ID3D11PixelShader* GetPixelShader() const { return m_pPixelShader; }
		bool IsValid() const;
		virtual void SetDebugName(const char*);
		typedef std::set<std::string> INCLUDE_FILES;

		void SetIncludeFiles(const INCLUDE_FILES& ifs);
		void SetCompileFailed(bool failed);

	protected:
		ShaderD3D11( const char* name );

	private:
		ID3D11VertexShader* m_pVertexShader;
		ID3D11GeometryShader* m_pGeometryShader;
		ID3D11HullShader* m_pHullShader;
		ID3D11DomainShader* m_pDomainShader;
		ID3D11PixelShader* m_pPixelShader;
		bool mValid;
		char* mVSBytecode;
		size_t mBytecodeSize;
	};
}

#endif //_fastbird_ShaderD3D11_header_included_