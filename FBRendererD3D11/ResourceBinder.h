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

namespace fastbird{
	class VertexBufferD3D11;
	class IndexBufferD3D11;
	class InputLayoutD3D11;

	class RasterizerStateD3D11;
	class BlendStateD3D11;
	class DepthStencilStateD3D11;
	class SamplerStateD3D11;

	class ResourceBinder{
	public:
		static void Bind(VertexBufferD3D11* vertexBufferD3D11);
		static void Bind(IndexBufferD3D11* indexBuffer);
		static void Bind(InputLayoutD3D11* inputLayout);
		static void Bind(RasterizerStateD3D11* rasterizerState);
		static void Bind(BlendStateD3D11* blendState);
		static void Bind(DepthStencilStateD3D11* depthStencilState);
		static void Bind(SamplerStateD3D11* samplerState);
		static void Bind(TextureD3D11* samplerState);
	};
}