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
	class IPlatformTexture;

	struct DrawQuadParam{
		int x, y; 
		int width, height;
		float r, g, b, a;
		bool updateRs; // default true.
	};

	struct DrawQuadLineParam{
		int x, y; 
		int width, height;
		int x2, y2;
		int width2, height2;
		float r, g, b, a;
	};

	struct DrawQuadWithTextureParam{
		int x, y; 
		int width, height;
		float r, g, b, a;
		IPlatformTexture* texture; 
		//IMaterial* materialOverride = 0 -- should bind in Renderer layer.
	};

	struct DrawQuadWithTextureUVParam{
		int x, y;
		int width, height; 
		float uStart, vStart, uEnd, vEnd;		
		float r, g, b, a;
		IPlatformTexture* texture;
		//IMaterial* materialOverride = 0 -- should bind in Renderer layer.
	};

	struct DrawBillboardWorldQuadParam{
		float x, y, z;
		float width, height;
		float offsetX, offsetY;
		unsigned color;
		//IMaterial* pMat -- shold bind in Renderer layer.
	};

	struct DrawTriangleParam{
		float x, y, z;
		float x2, y2, z2;
		float x3, y3, z3;
		float r, g, b, a;
		//IMaterial* pMat -- shold bind in Renderer layer.
	};
}