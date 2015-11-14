#include "stdafx.h"
#include "PointLightManager.h"
#include "PointLight.h"
#include "FBCommonHeaders/Helpers.h"
#include "../EssentialEngineData/shaders/Constants.h"
using namespace fastbird;

class PointLightManager::Impl{
public:
	typedef std::vector< PointLightPtr > PointLights;
	PointLights mPointLights;

	PointLightPtr CreatePointLight(const Vec3& pos, Real range, const Vec3& color, Real intensity, Real lifeTime, bool manualDeletion)
	{
		mPointLights.push_back(PointLight::Create(pos, range, color, intensity, lifeTime, manualDeletion));
		return mPointLights.back();
	}
	void DeletePointLight(PointLightPtr pointLight)
	{
		DeleteValuesInVector(mPointLights, pointLight);
	}

	void Update(Real dt)
	{
		static std::vector<unsigned> deleted;

		for (auto it = mPointLights.begin(); it != mPointLights.end();){
			auto p = *it;
			bool deleted = false;
			if (p->GetLifeTime() > 0)
			{
				auto lifeTime = p->GetOlder(dt);
				if (lifeTime <= 0 && !p->GetManualDeletion())
				{
					it = mPointLights.erase(it);
					deleted = true;
				}
			}
			if (!deleted){
				++it;
			}
		}		
	}

	void GatherPointLightData(BoundingVolume* aabb, const Transformation& transform, POINT_LIGHT_CONSTANTS* plConst)
	{
		struct GatheredData
		{
			GatheredData(Real distSQ, unsigned idx)
				:mDistanceSQ(distSQ), mIndex(idx)
			{

			}
			Real mDistanceSQ;
			unsigned mIndex;
		};
		static std::vector<GatheredData> gathered;
		gathered.reserve(50);

		unsigned i = 0;
		for (auto& p : mPointLights)
		{			
			if (!p->GetEnabled())
				continue;
			Ray3 ray(p->GetPosition(), transform.GetTranslation() - p->GetPosition());
			Ray3 localRay = transform.ApplyInverse(ray);
			auto iresult = localRay.intersects(aabb);
			Real distSQ = Squared(iresult.second);
			Real range = p->GetRange();
			if (distSQ < (range*range))
			{
				gathered.push_back(GatheredData(distSQ, i));
			}
			++i;
		}

		std::sort(gathered.begin(), gathered.end(), [](const GatheredData& a, const GatheredData& b){
			return a.mDistanceSQ < b.mDistanceSQ;
		}
		);

		plConst->gPointLightColor[0].w = 0;
		int count = std::min(3, (int)gathered.size());
		for (int i = 0; i < count; i++)
		{
			PointLightPtr p = mPointLights[gathered[i].mIndex];
			plConst->gPointLightPos[i] = Vec4(p->GetPosition(), p->GetRange());			
			plConst->gPointLightColor[i] = Vec4(p->GetColorPowered(), (Real)count);
		}

		gathered.clear();
	}

	unsigned GetNumPointLights() const
	{
		return mPointLights.size();
	}
};