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
#ifndef _IndexBuffer_header_included_
#define _IndexBuffer_header_included_

#include "FBCommonHeaders/Types.h"
#include "FBRenderer/IPlatformIndexBuffer.h"

namespace fastbird
{
	DECLARE_SMART_PTR(IndexBufferD3D11);
	class IndexBufferD3D11 : public IPlatformIndexBuffer
	{
		DECLARE_PIMPL_NON_COPYABLE(IndexBufferD3D11);
		IndexBufferD3D11();

	public:

		static IndexBufferD3D11Ptr Create();
		
		virtual bool IsReady() const;
		virtual void Bind();
		virtual INDEXBUFFER_FORMAT GetFormat() const { return mFormat; }
		DXGI_FORMAT GetFormatD3D11() const { return mFormatD3D11; }
		unsigned GetSize() const { return mSize; }
		unsigned inline GetNumIndices() const { return mNumIndices; }
		
		unsigned GetOffset() const { return mOffset; }
		void SetOffset(unsigned offset) { mOffset = offset; }

		IndexBufferD3D11(unsigned numIndices, INDEXBUFFER_FORMAT format);
		virtual ~IndexBufferD3D11();

	private:
		friend class RendererD3D11;
		static IndexBufferD3D11* CreateInstance(unsigned numIndices, INDEXBUFFER_FORMAT format);
		ID3D11Buffer* GetHardwareBuffer() const;
		void SetHardwareBuffer(ID3D11Buffer* pIndexBuffer);


	private:
		ID3D11Buffer* m_pIndexBuffer;
		unsigned mSize;
		unsigned mNumIndices;
		unsigned mOffset;
		INDEXBUFFER_FORMAT mFormat;
		DXGI_FORMAT mFormatD3D11;

	};
}

#endif //_IndexBuffer_header_included_