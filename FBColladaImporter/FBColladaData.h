#pragma once
#include "FBCommonHeaders/Types.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
namespace fastbird{
	DECLARE_SMART_PTR(AnimationData);
	namespace collada{
		typedef std::vector<unsigned> IndexBuffer;
		enum ColShape
		{
			ColShapeSphere,
			ColShapeCube,
			ColShapeMesh,
			ColShapeNum
		};
		/// \a colShape is lower case
		ColShape ConvertColShapeStringToEnum(const char* colShape);
		
		struct Vec2{
			float x, y;
		};

		struct Vec3{
			float x, y, z;

			Vec3(){

			}
			Vec3(float x_, float y_, float z_)
				:x(x_), y(y_), z(z_)
			{
			}

			Vec3 operator- (const Vec3& r) const;
			Vec3 Cross(const Vec3& rVector) const;
			float Dot(const Vec3& vec) const;
			float Normalize();
			Vec3 NormalizeCopy() const;
		};

		struct Vec4{
			float x, y, z, w;

			Vec4(){
			}

			Vec4(float x_, float y_, float z_, float w_)
				: x(x_), y(y_), z(z_), w(w_)
			{
			}
		};

		struct ModelTriangle {
			unsigned        v[3];
			// cached data for optimized ray-triangle intersections
			Vec2   v0Proj;           // 2D projection of vertices along the dominant axis
			Vec2   v1Proj;
			Vec2   v2Proj;
			Vec3   faceNormal;
			float  d;                // distance from triangle plane to origin
			int    dominantAxis;     // dominant axis of the triangle plane
		};

		struct MaterialGroup{
			std::string mMaterialPath;
			IndexBuffer mIndexBuffer;
			std::vector<Vec3> mPositions;
			std::vector<Vec3> mNormals;
			std::vector<Vec2> mUVs;
			std::vector<ModelTriangle> mTriangles;
			std::vector<Vec3> mTangents;
		};

		struct Mesh;
		typedef std::shared_ptr<Mesh> MeshPtr;

		struct Location{
			Vec3 mScale;
			Vec4 mQuat; // w, x, y, z order.
			Vec3 mPos;
		};
		struct CollisionInfo
		{
			ColShape mColShapeType;
			Location mTransform;
			MeshPtr mCollisionMesh;

			CollisionInfo(){

			}

			CollisionInfo(ColShape type, const Location& transform, MeshPtr colMesh)
				:mColShapeType(type), mTransform(transform), mCollisionMesh(colMesh)
			{

			}
		};
		typedef std::vector< CollisionInfo > COLLISION_INFOS;

		
		typedef std::vector< std::pair<std::string, Location> > AUXILIARIES;

		struct Mesh{
			std::string mName;
			std::map<int, MaterialGroup> mMaterialGroups;			
			// Animation Data is vaild while the ColladaImporter is alive.
			AnimationDataPtr mAnimationData;
			AUXILIARIES mAuxiliaries;
			COLLISION_INFOS mCollisionInfo;
		};

		struct MeshGroup
		{
			struct Data{
				int mParentMeshIdx;
				MeshPtr mMesh;
				Location mTransformation; // In Local Space(Parent Space)				
			};
			std::map<int, Data> mMeshes;
			AUXILIARIES mAuxiliaries;			
			COLLISION_INFOS mCollisionInfo;
		};
		typedef std::shared_ptr<MeshGroup> MeshGroupPtr;		

		typedef std::vector<unsigned> IndexBuffer;

		namespace DEFAULT_INPUTS{
			struct POSITION_NORMAL_TEXCOORD_V
			{
				POSITION_NORMAL_TEXCOORD_V() {}
				POSITION_NORMAL_TEXCOORD_V(const Vec3& _p, const Vec3& _n,
					const Vec2 _uv)
					: p(_p), n(_n), uv(_uv){}

				bool operator==(const POSITION_NORMAL_TEXCOORD_V& other) const
				{
					return memcmp(this, &other, sizeof(POSITION_NORMAL_TEXCOORD_V)) == 0;					
				}

				bool operator<(const POSITION_NORMAL_TEXCOORD_V& other) const
				{
					return memcmp(this, &other, sizeof(POSITION_NORMAL_TEXCOORD_V)) < 0;
				}
				Vec3 p;	// 12
				Vec3 n;	// 12
				Vec2 uv;	// 8
			};
			typedef POSITION_NORMAL_TEXCOORD_V V_PNT;
		}
	}
}