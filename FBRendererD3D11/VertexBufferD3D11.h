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
#ifndef _VertexBuffer_header_included_
#define _VertexBuffer_header_included_

#include "FBRenderer/IPlatformVertexBuffer.h"
namespace fastbird
{
	DECLARE_SMART_PTR(VertexBufferD3D11);
	class VertexBufferD3D11 : public IPlatformVertexBuffer
	{
		DECLARE_PIMPL_NON_COPYABLE(VertexBufferD3D11);
		VertexBufferD3D11();
		ID3D11Buffer* mVertexBuffer;		

	public:
		static VertexBufferD3D11Ptr Create();
		virtual ~VertexBufferD3D11();

		virtual void Bind();
		virtual bool IsReady() const;
		virtual MapData Map(MAP_TYPE type, UINT subResource, MAP_FLAG flag);
		virtual void Unmap();
		void SetHardwareBuffer(ID3D11Buffer* buffer);
		ID3D11Buffer* GetHardwareBuffer() const;
		
	};
}

#endif //_VertexBuffer_header_included_