#include "collisionManager.h"
#include "../../lib/collision/collision.h"
#include "../FOV/FOV.h"
#include "../../lib/effekseer/effekseer.h"
#include "../system/effectData/effectData.h"
#include "../../lib/myMath/myMath.h"

//----------------------------------------------
//			オブジェクト同士の当たり判定
//----------------------------------------------
void CCollisionManager::CheckHitObjectToObject(CObject& _objectA, CObject& _objectB)
{
	//二つのオブジェクトが消えていたら処理をしない
	if (_objectA.GetActive() == false || _objectB.GetActive() == false)return;

	//オブジェクトのタイプが球か箱かで当たり判定を変更する
	//球と箱の当たり判定の場合
	if (_objectA.GetObjectType() == OBJECT_TYPE_BOX || _objectB.GetObjectType() == OBJECT_TYPE_BOX)
	{
		CObject* box = nullptr;
		CObject* sphere = nullptr;

		if (_objectA.GetObjectType() == OBJECT_TYPE_BOX)
		{
			box = &_objectA;
			sphere = &_objectB;
		}
		else
		{
			box = &_objectB;
			sphere = &_objectA;
		}

		//オブジェクト同士が当たっているか
		if (CCollision::CheckHitBoxToSohere(box->GetCenter(), box->GetSize(),
			sphere->GetCenter(), sphere->GetRad()) == true)
		{
			//それぞれ当たり判定の処理をする
			box->HitCalc(sphere);
			sphere->HitCalc(box);
		}

		//押し戻し処理-------------------------------------------------

		// 四角形の上下左右手前奥それぞれの座標を計算する
		float boxUp = box->GetCenter().y - box->GetSize().y * 0.5f;
		float boxDown = box->GetCenter().y + box->GetSize().y * 0.5f;
		float boxLeft = box->GetCenter().x - box->GetSize().x * 0.5f;
		float boxRight = box->GetCenter().x + box->GetSize().x * 0.5f;
		float boxFront = box->GetCenter().z - box->GetSize().z * 0.5f;
		float boxBack = box->GetCenter().z + box->GetSize().z * 0.5f;

		//値をmin～maxの間に収める
		auto Clamp = [](float v, float min, float max)
			{
				if (v < min)return min;
				if (v > max)return max;
				return v;
			};

		//球と箱の最近接点を求める
		VECTOR closest;
		closest.x = Clamp(sphere->GetCenter().x, boxLeft, boxRight);
		closest.y = Clamp(sphere->GetCenter().y, boxUp, boxDown);
		closest.z = Clamp(sphere->GetCenter().z, boxFront, boxBack);

		//最近接点から球の中心座標までの距離を求める
		VECTOR diff = VSub(sphere->GetCenter(), closest);

		//距離を求める
		float dist = VSize(diff);

		//求めた距離が球の半径の以上なら当たっていないので処理をしない
		if (dist <= sphere->GetRad() || dist != 0.0f)
		{
			//押し出し方向を正規化
			VECTOR pushDir = VScale(diff, 1.0f / dist);

			//めり込み量を求める
			float penetration = sphere->GetRad() - dist;

			//球を押し戻す---------------------------
			VECTOR spherePos = sphere->GetPos();

			spherePos = VAdd(spherePos, VScale(pushDir, penetration));

			sphere->SetPos(spherePos);

			//箱の上に乗っていたら重力をリセットする
			if (spherePos.y >= boxUp)
			{
				sphere->GravityReset();
			}
		}

		if (sphere->GetObjectName() == OBJECT_PLAYER)
		{
			CPlayer* player = dynamic_cast<CPlayer*>(sphere);

			//上から見たプレイヤーと箱の当たり判定をとる
			VECTOR playerPos = sphere->GetPos();
			playerPos.y = 0.0f;
			VECTOR boxPos = box->GetPos();
			boxPos.y = 0.0f;
			int boxWidth = static_cast<int>(box->GetSize().z);
			int boxHeight = static_cast<int>(box->GetSize().x);

			if (CCollision::ChekHitDotToSquare(playerPos, boxPos,
				boxWidth, boxHeight) == true)
			{
				
			}
		}

		//---------------------------------------


		//-------------------------------------------------------------

	}
	//球同士の当たり判定の場合
	else
	{
		//オブジェクト同士が当たっているか
		if (CCollision::CheckHitSphereToSphere(_objectA.GetCenter(), _objectA.GetRad(),
			_objectB.GetCenter(), _objectB.GetRad()) == true)
		{
			//それぞれ当たり判定の処理をする
			_objectA.HitCalc(&_objectB);
			_objectB.HitCalc(&_objectA);

		}

		//押し出しフラグがfalseなら処理をしない
		if (_objectA.GetIsPushed() == false ||
			_objectB.GetIsPushed() == false)
			return;

		//押し戻し処理-------------------------------------------------

		//本来離れてほしい距離を求める
		float len1 = _objectA.GetRad() + _objectB.GetRad();

		//実際に離れている距離を求める
		VECTOR objectAPos = _objectA.GetPos();
		VECTOR objectBPos = _objectB.GetPos();

		VECTOR dir = VSub(objectBPos, objectAPos);
		float len2 = VSize(dir);

		//めり込んでいたら
		if (len1 > len2)
		{
			//めり込み量を求める
			float len3 = len1 - len2;

			len3 = len3 * 0.5f;

			//移動させるベクトルを求める

			//方向ベクトルなので正規化する

			//オブジェクトBの押し出し----------------
			if (_objectB.GetIsPushed() == true)
			{
				dir = VNorm(dir);

				dir = VScale(dir, len3);

				objectBPos = VAdd(_objectB.GetPos(), dir);

				_objectB.SetPos(VAdd(_objectB.GetPos(), dir));
			}
			//---------------------------------------

			//オブジェクトAの押し出し----------------
			if (_objectA.GetIsPushed() == true)
			{
				VECTOR dir2 = VSub(objectAPos, objectBPos);
				dir2 = VNorm(dir2);
				dir2 = VScale(dir2, len3);

				objectAPos = VAdd(_objectA.GetPos(), dir2);

				_objectA.SetPos(VAdd(_objectA.GetPos(), dir2));
			}
			//---------------------------------------
		}

		//-------------------------------------------------------------

	}

}

//----------------------------------------------
//		  collとオブジェクトの当たり判定
//----------------------------------------------
void CCollisionManager::CheckHitCollToObject(CObject& _collObject, CObject& _object)
{
	//当たり判定情報が格納される構造体
	MV1_COLL_RESULT_POLY_DIM col;

	col = MV1CollCheck_Sphere(_collObject.GetHndl(), -1, _object.GetCenter(), _object.GetRad());

	//ポリゴンと当たっていたか
	if (col.HitNum != 0)
	{
		//押し戻しの計算-----------------------

		for (int j = 0; j < col.HitNum; j++)
		{

			//中心点から最近点を引き算
			VECTOR vLen = VSub(_object.GetCenter(), col.Dim[j].HitPosition);
			//取得した距離を三平方の定理の長さに変換
			float fLen = VSize(vLen);
			//実際にめり込んだ距離を計算
			fLen = _object.GetRad() - fLen;
			//法線をめり込んだ距離分掛け算する
			vLen = VScale(col.Dim[j].Normal, fLen);

			//プレイヤーの座標を計算した分だけ移動させる
			_object.SetPos(VAdd(_object.GetPos(), vLen));

			//法線を取得
			VECTOR normal = col.Dim[j].Normal;

			//法線の角度を取得
			float angle = atan2f(normal.y, normal.x);

			//角度が90度の場合足元にあるかを判断する
			if (angle == 90.0f * (DX_PI_F / 180.0f))
			{
				float fLenY = _object.GetCenter().y - col.Dim[j].HitPosition.y;

				//足元にある場合重力をリセットする
				if (_object.GetPos().y - col.Dim[j].HitPosition.y > 1.0f)
				{
					//重力をリセット
					_object.GravityReset();
				}
			}

		}
		//-------------------------------------

	}

	//毎回データを削除
	MV1CollResultPolyDimTerminate(col);

	//丸影の当たり判定
	auto GetShadowPos = [](VECTOR _ObjectPos,int _collHndl,CShadow* _shadow)
		{
			if (_shadow->GetIsPosUpdate() == true)return;
			//オブジェクトの中心から下方向に線を引く
			VECTOR startPos = _ObjectPos;
			startPos.y += 1000.0f;
			VECTOR endPos = _ObjectPos;
			endPos.y -= 1000.0f;

			//当たり判定情報が格納される構造体
			MV1_COLL_RESULT_POLY col;

			//collと線の当たり判定を取得
			col = MV1CollCheck_Line(_collHndl, -1, startPos, endPos);

			//当たっていたら
			if (col.HitFlag == TRUE)
			{
				//当たった座標を丸影にセットする
				_shadow->SetPos(col.HitPosition);
				_shadow->SetIsPosUpdate(true);
			}
			else
			{
				_shadow->SetPos(endPos);
			}

		};

	//プレイヤーの判定
	if (_object.GetObjectName() == OBJECT_PLAYER)
	{
		CPlayer* player = dynamic_cast<CPlayer*>(&_object);

		GetShadowPos(player->GetCenter(), _collObject.GetHndl(), player->GetShadow());

		//cpuなら前方に障害物があるか判断してジャンプする--------------
		if (player->GetIsCpu() == true &&
			player->GetState() != JUMP)
		{
			VECTOR vec = player->GetCenter();

			//障害物の判定を行う座標までの距離
			VECTOR defaultDir = { 0.0f,0.0f,-30.0f };
			//上記を行列に変換
			MATRIX dir = CMyMath::GetTranslateMatrix(defaultDir);
			//Y軸回転行列
			MATRIX mRotY = CMyMath::GetYawMatrix(player->GetRot().y);
			//行列の合成
			MATRIX res = CMyMath::MatMult(mRotY, dir);

			vec.x += res.m[0][3];
			vec.z += res.m[2][3];

			col = MV1CollCheck_Sphere(_collObject.GetHndl(), -1, vec, 5.0f);

			if (col.HitNum != 0)
			{
				player->SetState(JUMP);
			}
			//毎回データを削除
			MV1CollResultPolyDimTerminate(col);

		}
		//-------------------------------------------------------------
	}

	if (_object.GetObjectName() == OBJECT_ITEM)
	{
		CItemBase* item = dynamic_cast<CItemBase*>(&_object);
		
		GetShadowPos(item->GetCenter(), _collObject.GetHndl(),item->GetShadow());
	}
}

//----------------------------------------------
//	 	 プレイヤーと攻撃判定の当たり判定
//----------------------------------------------
void CCollisionManager::CheckHitPlayerToPlayerAttack(CPlayerManager& _playerManager,
	CAttackManager& _attackManager)
{
	for (int player_j = 0; player_j < _playerManager.GetPlayerNum(); player_j++)
	{
		//プレイヤーのクラスを取得
		CPlayer* player = _playerManager.GetPlayer(player_j);

		for (int attack_i = 0; attack_i < _attackManager.GetNum(); attack_i++)
		{
			//攻撃のクラスを取得
			CAttackBase* attack = _attackManager.GetAttack(attack_i);

			//当たり判定
			CheckHitObjectToObject(*player, *attack);
		}
	}

}

//----------------------------------------------
//		プレイヤーとプレイヤーの当たり判定
//----------------------------------------------
void CCollisionManager::CheckHitPlayerToPlayer(CPlayerManager& _playerManager)
{
	for (int player1_i = 0; player1_i < _playerManager.GetPlayerNum(); player1_i++)
	{
		//プレイヤー1のクラスを取得
		CPlayer* player1 = _playerManager.GetPlayer(player1_i);

		//プレイヤー1が死んでいたら処理をしない
		if (player1->GetActive() == false)continue;

		for (int player2_j = 0; player2_j < _playerManager.GetPlayerNum(); player2_j++)
		{
			//同じプレイヤー同士では処理をしない
			if (player1_i == player2_j)continue;

			//敵のクラスを取得
			CPlayer* player2 = _playerManager.GetPlayer(player2_j);

			//敵が死んでいたら処理をしない
			if (player2->GetActive() == false)continue;

			CheckHitObjectToObject(*player1, *player2);
			
		}
	}

}

//----------------------------------------------
//		  プレイヤーとマップの当たり判定
//----------------------------------------------
void CCollisionManager::CheckHitPlayerToMap(CPlayerManager& _playerManager, CMapBase* _map)
{
	for (int i = 0; i < _playerManager.GetPlayerNum(); i++)
	{
		//プレイヤーのクラスを取得
		CPlayer* player = _playerManager.GetPlayer(i);

		//プレイヤーが死んでいたらスキップ
		if (player->GetActive() == false)continue;

		for (int stage_i = 0; stage_i < 2; stage_i++)
		{
			if (_map->GetStageId() - stage_i < 0)continue;
			//ステージが出現していない場合は処理をしない
			if (_map->GetStageActive(_map->GetStageId() - stage_i) == false)continue;

			CheckHitCollToObject(*(_map->GetStage(_map->GetStageId() - stage_i)),*player);
		}

		//ステージのオブジェクトとの当たり判定
		for (int stageObject_i = 0; stageObject_i < _map->GetStageObjectNum(); stageObject_i++)
		{
			CObject* stageObject = _map->GetStageObject(stageObject_i);
			if (stageObject->GetActive() == false)continue;

			CheckHitCollToObject(*stageObject,*player);
		}
	}
}

//----------------------------------------------
//		アイテムとマップの当たり判定
//----------------------------------------------
void CCollisionManager::CheckHitItemToMap(CItemManager& _itemManager, CMapBase* _map)
{
	std::list<std::unique_ptr<CItemBase>>& item = _itemManager.GetItemList();

	for (auto item_i = item.begin(); item_i != item.end(); ++item_i)
	{
		if ((*item_i)->GetActive() == false)continue;

		for (int stage_i = 0; stage_i < 2; stage_i++)
		{
			if (_map->GetStageId() - stage_i < 0)continue;
			//ステージが出現していなかったら処理をしない
			if (_map->GetStageActive(_map->GetStageId() - stage_i) == false)continue;

			CheckHitCollToObject(*(_map->GetStage(_map->GetStageId() - stage_i)), *(*item_i));
		}

		//ステージのオブジェクトとの当たり判定
		for (int stageObject_i = 0; stageObject_i < _map->GetStageObjectNum(); stageObject_i++)
		{
			CObject* stageObject = _map->GetStageObject(stageObject_i);
			if (stageObject->GetActive() == false)continue;

			CheckHitCollToObject(*stageObject,*(*item_i));
		}
	}
}

//----------------------------------------------
//		アイテムとプレイヤーの当たり判定
//----------------------------------------------
void CCollisionManager::CheckHitPlayerToItem(CPlayerManager& _playerManager, CItemManager& _itemManager)
{
	for (int player_i = 0; player_i < _playerManager.GetPlayerNum(); player_i++)
	{
		CPlayer* player = _playerManager.GetPlayer(player_i);

		list<unique_ptr<CItemBase>>& item = _itemManager.GetItemList();

		for (auto item_i = item.begin(); item_i != item.end(); ++item_i)
		{
			//当たり判定
			CheckHitObjectToObject(*player,*(*item_i));
		}
	}
}

//----------------------------------------------
//		 アイテムとアイテムの当たり判定
//----------------------------------------------
void CCollisionManager::CheckHitItemToItem(CItemManager& _itemManager)
{
	list<unique_ptr<CItemBase>>& item = _itemManager.GetItemList();

	for (auto itemA_i = item.begin(); itemA_i != item.end(); ++itemA_i)
	{
		for (auto itemB_i = item.begin(); itemB_i != item.end(); ++itemB_i)
		{
			//同じアイテムは処理をしない
			if ((*itemA_i) == (*itemB_i))continue;

			CheckHitObjectToObject(*(*itemA_i), *(*itemB_i));
		}
	}
}

//----------------------------------------------
//			プレイヤーと弾の当たり判定
//----------------------------------------------
void CCollisionManager::CheckHitPlayerToShot(CPlayerManager& _playerManager, CShotManager& _shotManager)
{
	for (int player_i = 0; player_i < _playerManager.GetPlayerNum(); player_i++)
	{
		CPlayer* player = _playerManager.GetPlayer(player_i);

		for (int shot_i = 0; shot_i < _shotManager.GetNum(); shot_i++)
		{
			CShotBase* shot = _shotManager.GetShot(shot_i);

			CheckHitObjectToObject(*player, *shot);
		}
	}
}

//----------------------------------------------
//		CPUの視界とプレイヤーの当たり判定
//----------------------------------------------
void CCollisionManager::CheckHitCpuPlayerFOVToPlayer(CPlayerManager& _playerManager)
{
	for (int cpuPlayerFOV_i = 0; cpuPlayerFOV_i < _playerManager.GetCpuPlayerFOVNum(); cpuPlayerFOV_i++)
	{
		CCpuPlayerFOV* cpuPlayerFOV = _playerManager.GetCpuPlayerFOV(cpuPlayerFOV_i);

		for (int player_i = 0; player_i < _playerManager.GetPlayerNum(); player_i++)
		{
			CPlayer* player = _playerManager.GetPlayer(player_i);

			CheckHitObjectToObject(*cpuPlayerFOV, *player);
		}
	}
}

//----------------------------------------------
//			CPUの視界とアイテムの当たり判定
//----------------------------------------------
void CCollisionManager::CheckHitCpuPlayerFOVToItem(CPlayerManager& _playerManager, CItemManager& _itemManager)
{
	list<unique_ptr<CItemBase>>& item = _itemManager.GetItemList();

	for (int cpuPlayerFOV_i = 0; cpuPlayerFOV_i < _playerManager.GetCpuPlayerFOVNum(); cpuPlayerFOV_i++)
	{
		CCpuPlayerFOV* cpuPlayerFOV = _playerManager.GetCpuPlayerFOV(cpuPlayerFOV_i);

		for (auto item_i = item.begin(); item_i != item.end(); ++item_i)
		{
			CheckHitObjectToObject(*cpuPlayerFOV, *(*item_i));
		}
	}
}

