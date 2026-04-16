#pragma once
#include "../enemy/enemyManager.h"
#include"../shot/shotManager.h"
#include "../player/playerManager.h"
#include "../goal/goal.h"
#include "../map/mapBase.h"
#include "../item/itemManager.h"
#include "../attack/attackManager.h"
#include "../../lib/model/3DObject.h"

class CCollisionManager
{
private:

	//オブジェクト同士の当たり判定
	static void CheckHitObjectToObject(CObject& _objectA, CObject& _objectB);

	//collとオブジェクトの当たり判定
	static void CheckHitCollToObject(CObject& _collObject,CObject& _object);
public:

	//プレイヤーと攻撃判定の当たり判定
	static void CheckHitPlayerToPlayerAttack(CPlayerManager& _playerManager,
		CAttackManager& _attackManager);

	//プレイヤーとプレイヤーの当たり判定
	static void CheckHitPlayerToPlayer(CPlayerManager& _playerManager);

	//プレイヤーとマップの当たり判定
	static void CheckHitPlayerToMap(CPlayerManager& _playerManager, CMapBase* _map);

	//アイテムとマップの当たり判定
	static void CheckHitItemToMap(CItemManager& _itemManager, CMapBase* _map);

	//アイテムとプレイヤーの当たり判定
	static void CheckHitPlayerToItem(CPlayerManager& _playerManager, CItemManager& _itemManager);

	//アイテムとアイテムの当たり判定
	static void CheckHitItemToItem(CItemManager& _itemManager);

	//プレイヤーと弾の当たり判定
	static void CheckHitPlayerToShot(CPlayerManager& _playerManager, CShotManager& _shotManager);

	//CPUの視界とプレイヤーの当たり判定
	static void CheckHitCpuPlayerFOVToPlayer(CPlayerManager& _playerManager);

	//CPUの視界とアイテムの当たり判定
	static void CheckHitCpuPlayerFOVToItem(CPlayerManager& _playerManager, CItemManager& _itemManager);

};

