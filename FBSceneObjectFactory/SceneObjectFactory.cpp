#include "stdafx.h"
#include "SceneObjectFactory.h"
#include "FBCommonHeaders/VectorMap.h"
#include "FBCommonHeaders/Helpers.h"
#include "FBStringLib/StringLib.h"
#include "FBDebugLib/Logger.h"
#include "FBColladaImporter/ColladaImporter.h"
#include "FBRenderer/Renderer.h"
#include "FBRenderer/ResourceProvider.h"
#include "FBFileSystem/FileSystem.h"
#include "FBAnimation/AnimationData.h"
#include "FBAnimation/Animation.h"
#include "FBTimer/Timer.h"
#include "MeshObject.h"
#include "MeshGroup.h"
#include "SkySphere.h"
using namespace fastbird;

Timer* fastbird::gpTimer = 0;
class SceneObjectFactory::Impl
{
public:
	// T itself should be shared_ptr
	template <class T>
	struct DataHolder{
		unsigned mNumCloned;
		T mObject;
	};

	// key is lower case.
	VectorMap<std::string, DataHolder<MeshObjectPtr> > mMeshObjects;
	VectorMap<std::string, DataHolder<MeshGroupPtr> > mMeshGroups;
	bool mNoMesh;

	SkySphereWeakPtr mNextEnvUpdateSky;
	Impl()
		: mNoMesh(false)
	{
		gpTimer = Timer::GetMainTimer().get();
		SkySphere::CreateSharedEnvRT();
	}
	~Impl(){
		SkySphere::DestroySharedEnvRT();
	}

	void SetEnableMeshLoad(bool enable){
		mNoMesh = !enable;
	}

	Vec3 ConvertCollada(const collada::Vec3& src){
		return Vec3((Real)src.x, (Real)src.y, (Real)src.z);
	}
	Vec2 ConvertCollada(const collada::Vec2& src){
		return Vec2((Real)src.x, (Real)src.y);
	}
	std::vector<MeshObject::ModelTriangle> ConvertCollada(const std::vector<collada::ModelTriangle>& data){
		std::vector<MeshObject::ModelTriangle> ret;
		ret.reserve(data.size());
		for (auto src : data){
			ret.push_back(MeshObject::ModelTriangle());
			auto& d = ret.back();
			for (int i = 0; i<3; ++i) 
				d.v[i] = src.v[i];
			d.v0Proj = ConvertCollada(src.v0Proj);
			d.v1Proj = ConvertCollada(src.v1Proj);
			d.v2Proj = ConvertCollada(src.v2Proj);
			d.faceNormal = ConvertCollada(src.faceNormal);
			d.d = src.d;
			d.dominantAxis = src.dominantAxis;
		}
		return ret;
	}

	std::vector<Vec2> ConvertCollada(const std::vector<collada::Vec2>& data){
		std::vector<Vec2> ret;
		ret.reserve(data.size());
#if defined(FB_DOUBLE_PRECISION)
		for (auto v : data){
			ret.push_back(Vec2(data.x, data.y, data.z));
		}
#else
		ret.resize(data.size());
		memcpy(&ret[0], &data[0], sizeof(Vec2) * data.size());
#endif
		return ret;
	}

	std::vector<Vec3> ConvertCollada(const std::vector<collada::Vec3>& data){
		std::vector<Vec3> ret;
		ret.reserve(data.size());
#if defined(FB_DOUBLE_PRECISION)
		for (auto v : data){
			ret.push_back(Vec3(data.x, data.y, data.z));
		}
#else
		ret.resize(data.size());
		memcpy(&ret[0], &data[0], sizeof(Vec3) * data.size());
#endif
		return ret;
	}

	Quat ConvertToQuternion(const collada::Vec4& src){
		return Quat((Real)src.w, (Real)src.x, (Real)src.y, (Real)src.z);
	}

	Transformation ConvertCollada(const collada::Location& src){
		Transformation transformation;
		auto trans = ConvertCollada(src.mPos);
		auto scale = ConvertCollada(src.mScale);
		auto quat = ConvertToQuternion(src.mQuat);
		transformation.SetScale(scale);
		transformation.SetRotation(quat);
		transformation.SetTranslation(trans);
		return transformation;
	}

	COLLISION_INFOS ConvertCollada(const collada::COLLISION_INFOS& colInfos){
		COLLISION_INFOS ret;
		for (auto& it : colInfos){
			ret.push_back(CollisionInfo());
			auto& d = ret.back();
			d.mColShapeType = (FBColShape::Enum)it.mColShapeType;
			d.mTransform = ConvertCollada(it.mTransform);
			d.mCollisionMesh = ConvertMeshData(it.mCollisionMesh, false, false);			
		}
		return ret;
	}

	MeshObjectPtr ConvertMeshData(collada::MeshPtr meshData, bool buildTangent, bool keepDataInMesh){
		if (!meshData)
			return 0;

		auto mesh = MeshObject::Create();
		mesh->SetName(meshData->mName.c_str());
		mesh->StartModification();
		for (auto& group : meshData->mMaterialGroups){
			std::vector<Vec3> data = ConvertCollada(group.second.mPositions);
			mesh->SetPositions(group.first, &data[0], data.size());
			data = ConvertCollada(group.second.mNormals);
			mesh->SetNormals(group.first, &data[0], data.size());
			std::vector<Vec2> data2 = ConvertCollada(group.second.mUVs);
			mesh->SetUVs(group.first, &data2[0], data2.size());
			std::vector<MeshObject::ModelTriangle> triangles = ConvertCollada(group.second.mTriangles);
			mesh->SetTriangles(group.first, &triangles[0], triangles.size());	
			MaterialPtr material;
			if (!group.second.mMaterialPath.empty()){
				material = Renderer::GetInstance().CreateMaterial(group.second.mMaterialPath.c_str());
			}
			else{
				material = Renderer::GetInstance().GetResourceProvider()->GetMaterial(
					ResourceTypes::Materials::Missing);
			}
			mesh->SetMaterialFor(group.first, material);
			if (buildTangent){
				mesh->GenerateTangent(group.first, &group.second.mIndexBuffer[0], group.second.mIndexBuffer.size());
			}
		}
		mesh->EndModification(keepDataInMesh);

		if (meshData->mAnimationData){			
			auto animation = Animation::Create();
			animation->SetAnimationData(meshData->mAnimationData);
			mesh->SetAnimation(animation);
		}

		for (auto& it : meshData->mAuxiliaries){
			AUXILIARY aux;
			aux.first = it.first;			
			aux.second = ConvertCollada(it.second);
			mesh->AddAuxiliaries(aux);
		}
		mesh->SetCollisionShapes(ConvertCollada(meshData->mCollisionInfo));


		return mesh;
	}

	MeshObjectPtr CreateMeshObject(const char* daeFilePath){
		CreateMeshObject(daeFilePath, MeshImportDesc());
	}

	MeshObjectPtr CreateMeshObject(const char* daeFilePath, const MeshImportDesc& desc){
		if (!ValidCStringLength(daeFilePath)) {
			Logger::Log(FB_ERROR_LOG_ARG, "Invalid arg.");
			return 0;
		}
		std::string filepath(daeFilePath);
		if (mNoMesh)
		{
			filepath = "es/objects/defaultCube.dae";
		}
		std::string filepathKey = filepath;
		ToLowerCase(filepathKey);
		

		auto it = mMeshObjects.Find(filepathKey);
		if (it != mMeshObjects.end()){
			it->second.mNumCloned++;
			return it->second.mObject->Clone();
		}

		auto pColladaImporter = ColladaImporter::Create();
		ColladaImporter::ImportOptions option;
		option.mMergeMaterialGroups = desc.mergeMaterialGroups;
		option.mOppositeCull = desc.oppositeCull;
		option.mSwapYZ = desc.yzSwap;
		option.mUseIndexBuffer = desc.useIndexBuffer;
		option.mUseMeshGroup = false;
		pColladaImporter->ImportCollada(filepath.c_str(), option);
		auto meshData = pColladaImporter->GetMeshObject();
		auto meshObject = ConvertMeshData(meshData, desc.generateTangent, desc.keepMeshData);
		if (meshObject)
		{
			meshObject->SetName(filepath.c_str());
			mMeshObjects[filepathKey] = DataHolder < MeshObjectPtr > {1, meshObject};
			return meshObject->Clone();
		}
		else
		{
			if (mNoMesh){
				Logger::Log(FB_ERROR_LOG_ARG, FormatString("Failed to load a mesh(%s, %s)", filepath.c_str(), daeFilePath).c_str());
			}
			else{
				Logger::Log(FB_ERROR_LOG_ARG, FormatString("Failed to load a mesh(%s)", filepath.c_str()).c_str());
			}
			return 0;
		}
	}

	MeshObjectConstPtr GetMeshArcheType(const char* name){
		if (!ValidCStringLength(name)){
			Logger::Log(FB_ERROR_LOG_ARG, "Invalid Arg");
			return 0;
		}
		std::string filepathKey(name);
		ToLowerCase(filepathKey);
		auto it = mMeshObjects.Find(filepathKey);
		if (it != mMeshObjects.end()){
			return it->second.mObject;
		}
		return 0;
	}

	MeshGroupPtr ConvertMeshGroupData(collada::MeshGroupPtr groupData, bool buildTangent, bool keepDataInMesh){
		MeshGroupPtr meshGroup = MeshGroup::Create();
		for (auto& it : groupData->mMeshes){
			auto mesh = ConvertMeshData(it.second.mMesh, buildTangent, keepDataInMesh);
			meshGroup->AddMesh(mesh, ConvertCollada(it.second.mTransformation), it.second.mParentMeshIdx);
		}

		for (auto& it : groupData->mAuxiliaries){
			AUXILIARY aux;
			aux.first = it.first;
			aux.second = ConvertCollada(it.second);
			meshGroup->AddAuxiliaries(aux);
		}
		meshGroup->SetCollisionShapes(ConvertCollada(groupData->mCollisionInfo));
	}

	MeshGroupPtr CreateMeshGroup(const char* file){
		return CreateMeshGroup(file, MeshImportDesc());
	}

	MeshGroupPtr CreateMeshGroup(const char* daeFilePath, const MeshImportDesc& desc){
		if (!ValidCStringLength(daeFilePath)) {
			Logger::Log(FB_ERROR_LOG_ARG, "Invalid arg.");
			return 0;
		}
		std::string filepath(daeFilePath);
		if (mNoMesh)
		{
			filepath = "es/objects/defaultCube.dae";
		}
		std::string filepathKey = filepath;
		ToLowerCase(filepathKey);

		auto it = mMeshGroups.Find(filepathKey);
		if (it != mMeshGroups.end()){
			it->second.mNumCloned++;
			return it->second.mObject->Clone();
		}

		auto pColladaImporter = ColladaImporter::Create();
		ColladaImporter::ImportOptions option;
		option.mMergeMaterialGroups = desc.mergeMaterialGroups;
		option.mOppositeCull = desc.oppositeCull;
		option.mSwapYZ = desc.yzSwap;
		option.mUseIndexBuffer = desc.useIndexBuffer;
		option.mUseMeshGroup = true;
		pColladaImporter->ImportCollada(filepath.c_str(), option);
		auto meshData = pColladaImporter->GetMeshGroup();
		auto meshGroup = ConvertMeshGroupData(meshData, desc.generateTangent, desc.keepMeshData);
		if (meshGroup)
		{
			meshGroup->SetName(filepath.c_str());
			mMeshGroups[filepathKey] = DataHolder < MeshGroupPtr > {1, meshGroup};
			return meshGroup->Clone();
		}
		else
		{
			if (mNoMesh){
				Logger::Log(FB_ERROR_LOG_ARG, FormatString("Failed to load a mesh group(%s, %s)", filepath.c_str(), daeFilePath).c_str());
			}
			else{
				Logger::Log(FB_ERROR_LOG_ARG, FormatString("Failed to load a mesh group(%s)", filepath.c_str()).c_str());
			}
			return 0;
		}
	}

	SkySpherePtr CreateSkySphere(){
		
	}

	BillboardQuadPtr CreateBillboardQuad(){
		
	}

	DustRendererPtr CreateDustQuad(){
		
	}

	TrailObjectPtr CreateTrailObject(){
		
	}

	void UpdateEnvMapInNextFrame(SkySpherePtr sky){
		mNextEnvUpdateSky = sky;
	}

	void Update(TIME_PRECISION dt){
		auto sky = mNextEnvUpdateSky.lock();
		if (sky){
			mNextEnvUpdateSky.reset();
			sky->UpdateEnvironmentMap(Vec3(0, 0, 0));
		}
	}
};