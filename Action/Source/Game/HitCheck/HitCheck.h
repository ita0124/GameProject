#pragma once
#include "Lib/Collision/Collision.h"
#include "Game/Base/Object/ObjectBase.h"
#include "Game/Manager/Platform/PlatformManager.h"
#include "Game/Manager/MobEnemy/MobEnemyManager.h"
#include "Game/Manager/Gimmick/GimmickManager.h"
#include "Lib/DxLibFont/DxLibFont.h"

class HitCheck {
public:
	//オブジェクト同士の押し合い当たり判定
	static void ObjectToObjectPush(ObjectBase& _ObjectA, ObjectBase& _ObjectB);
	//オブジェクトの攻撃当たり判定
	static void ObjectToObjectAttack(ObjectBase& _Object, ObjectBase& _AttackObject);
	//オブジェクトの位置関係判定
	static void ObjectToObjectRelativePos(ObjectBase& _Object, ObjectBase& _RelativePosObject);
	//オブジェクトとフィールドの当たり判定
	static void ObjectToField(ObjectBase& _Object, ObjectBase& _Field);
	//オブジェクトと足場の当たり判定
	static void ObjectToPlatform(ObjectBase& _Object, PlatformManager& _PlatformManager);
	//オブジェクトとギミックの当たり判定
	static void ObjectToGimmick(ObjectBase& _Object, GimmickManager& _GimmickManager);
	//モブ敵とオブジェクトの押し合い当たり判定
	static void MobEnemyToObjectPush(MobEnemyManager& _MobEnemyManager, ObjectBase& _Object);
	//モブ敵に対するオブジェクトの攻撃当たり判定
	static void MobEnemyToObjectAttack(MobEnemyManager& _MobEnemyManager, ObjectBase& _AttackObject);
	//オブジェクトとモブ敵の攻撃当たり判定
	static void ObjectToMobEnemyAttack(ObjectBase& _Object, MobEnemyManager& _MobEnemyManager);
	//モブ敵と足場の当たり判定
	static void MobEnemyToPlatform(MobEnemyManager& _MobEnemyManager, PlatformManager& _PlatformManager);
};