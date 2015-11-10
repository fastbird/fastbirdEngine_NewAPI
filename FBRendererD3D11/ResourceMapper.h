#pragma once
#include "FBRenderer/RenderStates.h"

namespace fastbird{	

	class ResourceMapper{
	public:
		static MapData Map(ID3D11Buffer* buffer, MAP_TYPE type, UINT subResource, MAP_FLAG flag);
		static void Unmap(ID3D11Buffer* buffer);
		

	};
}