#pragma once
#include <BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>
namespace fastbird{
	struct IFilterCallback;
	struct BulletFilterCallback : public btOverlapFilterCallback
	{
		IFilterCallback* mAppCallback;
		BulletFilterCallback(IFilterCallback* callback);
		virtual bool needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const;
	};
}