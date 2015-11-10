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
	};
}