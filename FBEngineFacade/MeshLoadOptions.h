#pragma once
namespace fastbird{
	struct MeshLoadOptions{
		bool yzSwap;
		bool oppositeCull;
		bool useIndexBuffer;
		bool mergeMaterialGroups;
		bool keepMeshData;
		bool generateTangent;

		MeshLoadOptions()
			: yzSwap(false), oppositeCull(true),
			useIndexBuffer(false), mergeMaterialGroups(false),
			keepMeshData(false), generateTangent(true)
		{

		}
	};
}