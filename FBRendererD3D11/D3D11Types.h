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
#include "FBCommonHeaders/Types.h"

DECLARE_SMART_PTR_STRUCT(ID3D11Device);
DECLARE_SMART_PTR_STRUCT(IDXGIFactory1);
DECLARE_SMART_PTR_STRUCT(IDXGISwapChain);
DECLARE_SMART_PTR_STRUCT(ID3DX11ThreadPump);
DECLARE_SMART_PTR_STRUCT(ID3D11DeviceContext);
DECLARE_SMART_PTR_STRUCT(IDXGIOutput);
DECLARE_SMART_PTR_STRUCT(IDXGIAdapter1);

DECLARE_SMART_PTR_STRUCT(ID3D11Buffer);
DECLARE_SMART_PTR_STRUCT(ID3D11Texture2D);
DECLARE_SMART_PTR_STRUCT(ID3D11ShaderResourceView);
DECLARE_SMART_PTR_STRUCT(ID3D11RenderTargetView);
DECLARE_SMART_PTR_STRUCT(ID3D11DepthStencilView);

DECLARE_SMART_PTR_STRUCT(ID3D11InputLayout);

DECLARE_SMART_PTR_STRUCT(ID3D11VertexShader);
DECLARE_SMART_PTR_STRUCT(ID3D11HullShader);
DECLARE_SMART_PTR_STRUCT(ID3D11DomainShader);
DECLARE_SMART_PTR_STRUCT(ID3D11GeometryShader);
DECLARE_SMART_PTR_STRUCT(ID3D11PixelShader);

DECLARE_SMART_PTR_STRUCT(ID3D11RasterizerState);
DECLARE_SMART_PTR_STRUCT(ID3D11BlendState);
DECLARE_SMART_PTR_STRUCT(ID3D11DepthStencilState);
DECLARE_SMART_PTR_STRUCT(ID3D11SamplerState);