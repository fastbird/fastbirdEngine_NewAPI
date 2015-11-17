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

#include "stdafx.h"
#include "IndexBufferD3D11.h"
#include "FBRenderer/RendererEnums.h"
#include "ResourceBinder.h"

namespace fastbird
{
	IndexBufferD3D11* IndexBufferD3D11::CreateInstance(unsigned int numIndices, INDEXBUFFER_FORMAT format)
	{
		IndexBufferD3D11* pIndexBuffer = FB_NEW(IndexBufferD3D11)(numIndices, format);
		return pIndexBuffer;
	}

	IndexBufferD3D11::IndexBufferD3D11(unsigned numIndices, INDEXBUFFER_FORMAT format)
		: m_pIndexBuffer(0)
		, mFormat(format), mOffset(0), mNumIndices(numIndices)
	{
		switch(format)
		{
		case INDEXBUFFER_FORMAT_16BIT:
			mSize = numIndices * 2;
			mFormatD3D11 = DXGI_FORMAT_R16_UINT;
			break;
		case INDEXBUFFER_FORMAT_32BIT:
			mSize = numIndices * 4;
			mFormatD3D11 = DXGI_FORMAT_R32_UINT;
			break;

		}
		
	}

	IndexBufferD3D11::~IndexBufferD3D11()
	{
		SAFE_RELEASE(m_pIndexBuffer);
	}

	bool IndexBufferD3D11::IsReady() const
	{
		return m_pIndexBuffer != 0;
	}

	void IndexBufferD3D11::Bind()
	{
		assert(m_pIndexBuffer);
		if (!m_pIndexBuffer)
			return;

		ResourceBinder::Bind(this);
	}

	ID3D11Buffer* IndexBufferD3D11::GetHardwareBuffer() const
	{
		return m_pIndexBuffer;
	}

	void IndexBufferD3D11::SetHardwareBuffer(ID3D11Buffer* pIndexBuffer)
	{
		SAFE_RELEASE(m_pIndexBuffer);
		m_pIndexBuffer = pIndexBuffer;
	}





}