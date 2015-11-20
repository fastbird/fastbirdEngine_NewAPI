#include "stdafx.h"
#include "FBColladaData.h"
#include "FBDebugLib/Logger.h"
#include "FBStringLib/StringLib.h"

namespace fastbird{
	namespace collada{
		ColShape ConvertColShapeStringToEnum(const char* colShape){
			if (strcmp(colShape, "sphere") == 0){
				return ColShapeSphere;
			}
			else if(strcmp(colShape, "cube") == 0){
				return ColShapeCube;
			}
			else if (strcmp(colShape, "mesh") == 0){
				return ColShapeMesh;
			}
			else{
				Logger::Log(FB_ERROR_LOG_ARG, FormatString("Cannot convert colshape string(%s) to enum", colShape).c_str());
				return ColShapeSphere;
			}
		}
		Vec3 Vec3::operator - (const Vec3& r) const{
			return Vec3(x - r.x, y - r.y, z - r.z);
		}
		Vec3 Vec3::Cross(const Vec3& rVector) const{
			return Vec3(
				y * rVector.z - z * rVector.y,
				z * rVector.x - x * rVector.z,
				x * rVector.y - y * rVector.x);
		}
		float Vec3::Dot(const Vec3& vec) const{
			return x * vec.x + y * vec.y + z * vec.z;
		}

		float Vec3::Normalize(){
			float length = sqrt(x*x + y*y + z*z);
			if (length > 0.0f)
			{
				float invLength = 1.f / length;
				x *= invLength;
				y *= invLength;
				z *= invLength;
			}

			return length;
		}

		Vec3 Vec3::NormalizeCopy() const{
			Vec3 result = *this;
			result.Normalize();
			return result;
		}
	}
}