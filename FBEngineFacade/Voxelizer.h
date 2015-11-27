#pragma once
#include "FBCommonHeaders/Types.h"
namespace fastbird{
	class Vec3I;
	FB_DECLARE_SMART_PTR(MeshObject);
	FB_DECLARE_SMART_PTR(Voxelizer);
	class Voxelizer{
		FB_DECLARE_PIMPL_NON_COPYABLE(Voxelizer);
		Voxelizer();
		~Voxelizer();

	public:
		static VoxelizerPtr Create();

		virtual bool RunVoxelizer(const char* filename, UINT numVoxels, bool swapYZ, bool oppositCull);
		virtual MeshObjectPtr GetMeshObject() const;
		typedef std::vector<fastbird::Vec3I> HULLS;
		virtual const HULLS& GetHulls() const;
	};
}