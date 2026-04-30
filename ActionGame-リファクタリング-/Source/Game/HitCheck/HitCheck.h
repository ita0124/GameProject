#pragma once
#include "Lib/Collision/Collision.h"
#include "Game/Base/Object/ObjectBase.h"
#include "Game/Manager/Platform/PlatformManager.h"

class HitCheck {
public:
	//オブジェクト同士の押し合い当たり判定
	static void ObjectToObjectPush(ObjectBase& _ObjectA, ObjectBase& _ObjectB);
	//オブジェクト同士の攻撃当たり判定
	static void ObjectToObjectAttack(ObjectBase& _ObjectA, ObjectBase& _ObjectB);
	//Collとオブジェクトの当たり判定
	static void CollToObject(ObjectBase&_CollObject,ObjectBase &_Object);
	//オブジェクトと足場の当たり判定
	static void ObjectToPlatform(ObjectBase& _Object, PlatformManager &_PlatformManager);
};