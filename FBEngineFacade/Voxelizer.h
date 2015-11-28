#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	class Vec3I;
	FB_DECLARE_SMART_PTR(MeshObject);
	FB_DECLARE_SMART_PTR(Voxelizer);
	class FB_DLL_ENGINEFACADE Voxelizer{
		FB_DECLARE_PIMPL_NON_COPYABLE(Voxelizer);
		Voxelizer();
		~Voxelizer();

	public:
		static VoxelizerPtr Create();
		typedef std::vector<fastbird::Vec3I> HULLS;

		bool RunVoxelizer(const char* filename, UINT numVoxels, bool swapYZ, bool oppositCull);
		MeshObjectPtr GetMeshObject() const;		
		const HULLS& GetHulls() const;
	};
}