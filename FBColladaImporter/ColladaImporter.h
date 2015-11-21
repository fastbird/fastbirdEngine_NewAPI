#pragma once
#include "FBCommonHeaders/platform.h"
#include "FBColladaImporter/FBColladaData.h"
namespace COLLADAFW
{
	class Mesh;
	class MeshVertexData;
	class Node;
	class FloatOrDoubleArray;
}
namespace fastbird
{
	namespace collada{
		struct Mesh;
		typedef std::shared_ptr<Mesh> MeshPtr;
		struct MeshGroup;
		typedef std::shared_ptr<MeshGroup> MeshGroupPtr;
	}
	struct ImportOptions;		
	DECLARE_SMART_PTR(ColladaImporter);		
	class FB_DLL_COLLADA ColladaImporter
	{
		DECLARE_PIMPL_NON_COPYABLE(ColladaImporter);

	protected:
		ColladaImporter();
		~ColladaImporter();

	public:

		static ColladaImporterPtr Create();

		struct ImportOptions{
			bool mSwapYZ;
			bool mOppositeCull;
			bool mUseIndexBuffer;
			bool mMergeMaterialGroups;
			bool mUseMeshGroup;

			ImportOptions()
				: mSwapYZ(false)
				, mOppositeCull(true)
				, mUseIndexBuffer(false)
				, mMergeMaterialGroups(false)
				, mUseMeshGroup(false)
			{
			}
		};
			
		bool ImportCollada(const char* filepath);
		bool ImportCollada(const char* filepath, const ImportOptions& options);			
		collada::MeshPtr GetMeshObject() const;
		collada::MeshGroupPtr GetMeshGroup() const;
	};
}