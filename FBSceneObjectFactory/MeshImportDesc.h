#pragma once
namespace fastbird {
	struct MeshImportDesc
	{
		MeshImportDesc()
			: yzSwap(false), oppositeCull(true),
			useIndexBuffer(false), mergeMaterialGroups(false),
			keepMeshData(false), generateTangent(true)
		{
		}
		bool yzSwap;
		bool oppositeCull;
		bool useIndexBuffer;
		bool mergeMaterialGroups;
		bool keepMeshData;
		bool generateTangent;
	};
}