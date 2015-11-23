#include "stdafx.h"
#include "BulletFilterCallback.h"
#include "IFilterCallback.h"
using namespace fastbird;
BulletFilterCallback::BulletFilterCallback(IFilterCallback* callback) : mAppCallback(callback)
{

}

bool BulletFilterCallback::needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const{
	bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
	collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);

	if (!collides || !mAppCallback)
		return collides;

	btCollisionObject* objA = (btCollisionObject*)proxy0->m_clientObject;
	btCollisionObject* objB = (btCollisionObject*)proxy1->m_clientObject;
	RigidBody* rigidBodyA = (RigidBody*)objA->getUserPointer();
	RigidBody* rigidBodyB = (RigidBody*)objB->getUserPointer();
	if (!rigidBodyA || !rigidBodyB)
		return collides;

	return mAppCallback->needCollision(rigidBodyA, rigidBodyB);
}