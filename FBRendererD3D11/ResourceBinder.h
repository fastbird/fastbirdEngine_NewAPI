#pragma once

namespace fastbird{
	class IndexBufferD3D11;
	class ResourceBinder{
	public:
		static void SetIndexBuffer(IndexBufferD3D11* indexBuffer);
	};
}