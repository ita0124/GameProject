#include "HitCheck.h"
#include "Game/Object/Actor/Character/Player/Player.h"
#include "Game/Object/Actor/Character/Enemy/Boss/Boss.h"
#include "Game/Base/Object/ActorBase/Character/EnemyBase/EnemyBase.h"
#include "Game/Object/Sword/Sword.h"

//オブジェクト同士の押し合い当たり判定
void HitCheck::ObjectToObjectPush(ObjectBase& _ObjectA, ObjectBase& _ObjectB) {
	//どちらかのオブジェクトの生存フラグがオフになっていれば以降の処理をしない
	if (!_ObjectA.GetIsActive() || !_ObjectB.GetIsActive())return;

	//Aの座標取得
	VECTOR	ObjectAPos = _ObjectA.GetCenter(ObjectBase::TagShape::BALL);
	//Bの座標取得
	VECTOR	ObjectBPos = _ObjectB.GetCenter(ObjectBase::TagShape::BALL);
	//Aの半径取得
	float	ObjectARad = _ObjectA.GetRad();
	//Bの半径取得
	float	ObjectBRad = _ObjectB.GetRad();
	//当たり判定
	bool IsHit = Collision::CheckHitSphereToSphere(ObjectAPos, ObjectARad, ObjectBPos, ObjectBRad);
	//当たっていれば
	if (IsHit) {
		//Y軸は考慮しない
		ObjectAPos.y = 0.0f;
		//Y軸は考慮しない
		ObjectBPos.y = 0.0f;
#ifdef _DEBUG
		DrawSphere3D(ObjectAPos, ObjectARad, DIV, RED, RED, FALSE);
		DrawSphere3D(ObjectBPos, ObjectBRad, DIV, RED, RED, FALSE);
#endif // DEBUG
		//Aの押し出しフラグがオンなら
		if (_ObjectA.GetIsPush()) {
			//BからAへの方向ベクトルを生成
			VECTOR Dir = VSub(ObjectAPos, ObjectBPos);
			//長さ取得
			float Len = VSize(Dir);
			//正規化
			Dir = VNorm(Dir);
			//押し出す長さを取得
			Len = (ObjectARad + ObjectBRad) - Len;
			//方向ベクトルに押し出す長さを掛ける
			Dir = VScale(Dir, Len);
			//現在の座標に加算
			_ObjectA.AddPos(Dir);
		}
		if (_ObjectB.GetIsPush()) {
			//AからBへの方向ベクトルを生成
			VECTOR Dir = VSub(ObjectBPos, ObjectAPos);
			//長さ取得
			float Len = VSize(Dir);
			//正規化
			Dir = VNorm(Dir);
			//押し出す長さを取得
			Len = (ObjectARad + ObjectBRad) - Len;
			//方向ベクトルに押し出す長さを掛ける
			Dir = VScale(Dir, Len);
			//現在の座標に加算
			_ObjectB.AddPos(Dir);
		}
	}
}
//オブジェクト同士の攻撃当たり判定
void HitCheck::ObjectToObjectAttack(ObjectBase& _Object, ObjectBase& _AttackObject) {
	//どちらかのオブジェクトの生存フラグがオフになっていれば以降の処理をしない
	if (!_Object.GetIsActive() || !_AttackObject.GetIsActive())return;
	//オブジェクトの当たり判定フラグがオフになっていれば以降の処理をしない
	if (!_Object.GetIsCollision())return;
	/*===========================================================================*/
	//オブジェクトがプレイヤーかつ攻撃オブジェクトが敵だった場合
	if (_Object.GetKinds() == ObjectBase::TagKinds::PLAYER && _AttackObject.GetKinds() == ObjectBase::TagKinds::ENEMY) {
		//エネミーベースクラスデータを保存する変数
		EnemyBase* PointerEnemy = nullptr;
		//エネミーベースクラスをダウンキャスト
		PointerEnemy = dynamic_cast<EnemyBase*>(&_AttackObject);
		//ボスのフレーム分
		for (int FrameNum = 0;FrameNum < PointerEnemy->GetFrameNumber();FrameNum++) {
			//フレームの攻撃判定がオフになっていたら次のフレームを見る
			if (!PointerEnemy->GetFrameDataIsAttackFlg(FrameNum))continue;
			//プレイヤーの座標を取得
			VECTOR	PlayerPos1 = _Object.GetCenter();
			//プレイヤーの頭の座標を取得
			VECTOR	PlayerPos2 = _Object.GetFramePos(_Object.GetHndl(), Player::FrameNumber::HEAD);
			//プレイヤーの当たり判定半径を取得
			float	PlayerRad = _Object.GetRad();
			//ボスの座標を取得
			VECTOR	BossPos = PointerEnemy->GetFrameDataPos(FrameNum);
			//ボスの当たり判定半径を取得
			float	BossRad = PointerEnemy->GetFrameDataRad(FrameNum);
#ifdef _DEBUG
			DrawSphere3D(PlayerPos1, PlayerRad, DIV, RED, RED, FALSE);
			DrawSphere3D(PlayerPos2, PlayerRad, DIV, RED, RED, FALSE);
			DrawSphere3D(BossPos, BossRad, DIV, RED, RED, FALSE);
#endif // DEBUG
			//当たり判定
			bool IsHit1 = Collision::CheckHitSphereToSphere(PlayerPos1, PlayerRad, BossPos, BossRad);
			bool IsHit2 = Collision::CheckHitSphereToSphere(PlayerPos2, PlayerRad, BossPos, BossRad);
			//当たっていれば
			if (IsHit1 || IsHit2) {
				_Object.HitCalc(PointerEnemy);
				//プレイヤークラスデータを保存する変数
				Player* PointerPlayer = nullptr;
				//プレイヤークラスをダウンキャスト
				PointerPlayer = dynamic_cast<Player*>(&_Object);
				_AttackObject.HitCalc(PointerPlayer);
				break;
			}
		}
	}
	/*===========================================================================*/
	//オブジェクトが盾だった場合
	if (_Object.GetKinds() == ObjectBase::TagKinds::SHIELD) {
		//オーナーを取得
		ObjectBase* Owner = _Object.GetOwner();
		//プレイヤークラスデータを保存する変数
		Player* PointerPlayer = nullptr;
		//プレイヤークラスをダウンキャスト
		PointerPlayer = dynamic_cast<Player*>(Owner);
		//ガード成功フラグがオンになっていたら以降の処理は行わない
		if (PointerPlayer->GetIsParrySucess() || PointerPlayer->GetIsGuardSuccess())return;
		//攻撃当たり判定を生成してよいか
		if (PointerPlayer->GetIsGuardCollision() || PointerPlayer->GetIsParryCollision()) {
			//エネミーベースクラスデータを保存する変数
			EnemyBase* PointerEnemy = nullptr;
			//エネミーベースクラスをダウンキャスト
			PointerEnemy = dynamic_cast<EnemyBase*>(&_AttackObject);
			//ボスのフレーム分
			for (int FrameNum = 0;FrameNum < PointerEnemy->GetFrameNumber();FrameNum++) {
				//ガード成功フラグがオンになっていたら以降の処理は行わない
				if (PointerPlayer->GetIsParrySucess() || PointerPlayer->GetIsGuardSuccess())return;
				//フレームの攻撃判定がオフになっていたら次のフレームを見る
				if (!PointerEnemy->GetFrameDataIsAttackFlg(FrameNum))continue;
				//プレイヤーの座標を取得
				VECTOR	ShieldPos = _Object.GetPos();
				//プレイヤーの当たり判定半径を取得
				float	ShieldRad = _Object.GetRad();
				//ボスの座標を取得
				VECTOR	BossPos = PointerEnemy->GetFrameDataPos(FrameNum);
				//ボスの当たり判定半径を取得
				float	BossRad = PointerEnemy->GetFrameDataRad(FrameNum);
#ifdef _DEBUG
				//パリィ許容フラグがオンなら
				if (PointerPlayer->GetIsParryCollision()) {
					DrawSphere3D(ShieldPos, ShieldRad, DIV, GREEN, GREEN, FALSE);
					DrawSphere3D(BossPos, BossRad, DIV, GREEN, GREEN, FALSE);
				}
				else {
					DrawSphere3D(ShieldPos, ShieldRad, DIV, BLUUE, BLUUE, FALSE);
					DrawSphere3D(BossPos, BossRad, DIV, BLUUE, BLUUE, FALSE);
				}
#endif // DEBUG
				//当たり判定
				bool IsHit = Collision::CheckHitSphereToSphere(ShieldPos, ShieldRad, BossPos, BossRad);
				//当たっていれば
				if (IsHit) {
					//パリィの処理
					PointerPlayer->HitCalc(PointerEnemy);
					PointerEnemy->HitCalc(PointerPlayer);
				}
			}
		}
	}
	/*===========================================================================*/
	//攻撃オブジェクトが剣だった場合
	if (_AttackObject.GetKinds() == ObjectBase::TagKinds::SWORD) {
		//オーナーを取得
		ObjectBase* Owner = _AttackObject.GetOwner();
		//オーナーの生存フラグがオフになっていれば以降の処理をしない
		if (!Owner->GetIsActive())return;
		//プレイヤークラスデータを保存する変数
		Player* PointerPlayer = nullptr;
		//プレイヤークラスをダウンキャスト
		PointerPlayer = dynamic_cast<Player*>(Owner);
		//攻撃当たり判定を生成してよいか
		if (PointerPlayer->GetIsAttackCollision()) {
			//剣の指定したフレームの座標を取得
			VECTOR	SwordPos = _AttackObject.GetFramePos(_AttackObject.GetHndl(), Sword::FrameNumber::BLADE002);
			//剣の当たり判定半径を取得
			float	SwordRad = _AttackObject.GetRad();
			//オブジェクトの座標を取得
			VECTOR	ObjectPos = _Object.GetCenter();
			//オブジェクトの当たり判定半径を取得
			float	ObjectRad = _Object.GetRad();
#ifdef _DEBUG
			DrawSphere3D(SwordPos, SwordRad, DIV, YELLOW, YELLOW, FALSE);
			DrawSphere3D(ObjectPos, ObjectRad, DIV, YELLOW, YELLOW, FALSE);
#endif // DEBUG
			//当たり判定
			bool IsHit = Collision::CheckHitSphereToSphere(SwordPos, SwordRad, ObjectPos, ObjectRad);
			//当たっていれば
			if (IsHit) {
				_Object.HitCalc(PointerPlayer);
			}
		}
	}
	/*===========================================================================*/
}
//オブジェクトの位置関係判定
void HitCheck::ObjectToObjectRelativePos(ObjectBase& _Object, ObjectBase& _RelativePosObject) {
	//どちらかのオブジェクトの生存フラグがオフになっていれば以降の処理をしない
	if (!_Object.GetIsActive() || !_RelativePosObject.GetIsActive())return;

	//オブジェクトがプレイヤーかつ位置関係を判定するオブジェクトがボスなら
	if (_Object.GetKinds() == ObjectBase::TagKinds::PLAYER && _RelativePosObject.GetKinds() == ObjectBase::TagKinds::ENEMY) {
		//ボスクラスデータを保存する変数
		Boss* PointerBoss = nullptr;
		//ボスクラスをダウンキャスト
		PointerBoss = dynamic_cast<Boss*>(&_RelativePosObject);
		for (int FrameNum = Boss::FrameNumber::FRONT;FrameNum < Boss::FrameNumber::FRAME_NUM;FrameNum++) {
			//プレイヤーの座標を取得
			VECTOR	PlayerPos1 = _Object.GetCenter();
			//プレイヤーの頭の座標を取得
			VECTOR	PlayerPos2 = _Object.GetFramePos(_Object.GetHndl(), Player::FrameNumber::HEAD);
			//プレイヤーの当たり判定半径を取得
			float	PlayerRad = _Object.GetRad();
			//ボスの座標を取得
			VECTOR	BossPos = PointerBoss->GetFrameDataPos(FrameNum);
			//ボスの当たり判定半径を取得
			float	BossRad = PointerBoss->GetFrameDataRad(FrameNum);
			//当たり判定
			bool IsHit1 = Collision::CheckHitSphereToSphere(PlayerPos1, PlayerRad, BossPos, BossRad);
			bool IsHit2 = Collision::CheckHitSphereToSphere(PlayerPos1, PlayerRad, BossPos, BossRad);
			//当たっていれば
			if (IsHit1 || IsHit2) {
				PointerBoss->HitFrame(FrameNum);
			}
		}
	}

}
//オブジェクトとフィールドの当たり判定
void HitCheck::ObjectToField(ObjectBase& _Object, ObjectBase& _Field) {
	//オブジェクトの生存フラグがオフになっていれば以降の処理をしない
	if (!_Object.GetIsActive())return;
	//オブジェクトの座標取得
	VECTOR	ObjectPos = _Object.GetCenter(ObjectBase::TagShape::BOX);
	//オブジェクトの１フレーム前の座標取得
	VECTOR PrevPos = _Object.GetPrevCenter(ObjectBase::TagShape::BOX);
	//オブジェクトのサイズを取得
	VECTOR	ObjectSize = _Object.GetSize();
	//足場クラスの座標取得
	VECTOR PlatformPos = _Field.GetCenter(ObjectBase::TagShape::FIELD);
	//足場クラスのサイズを取得
	VECTOR PlatformSize = _Field.GetSize();

	float Pos1X = PlatformPos.x + PlatformSize.x * 0.5f;
	float Pos1Y = PlatformPos.y + PlatformSize.y * 0.5f;
	float Pos1Z = PlatformPos.z + PlatformSize.z * 0.5f;

	float Pos2X = PlatformPos.x - PlatformSize.x * 0.5f;
	float Pos2Y = PlatformPos.y - PlatformSize.y * 0.5f;
	float Pos2Z = PlatformPos.z - PlatformSize.z * 0.5f;

	VECTOR PlatformPos1 = VGet(Pos1X, Pos1Y, Pos1Z);
	VECTOR PlatformPos2 = VGet(Pos2X, Pos2Y, Pos2Z);
	//当たり判定
	bool IsHit = Collision::CheckHitBoxToBox(ObjectPos, ObjectSize, PlatformPos, PlatformSize);
	//当たっていれば
	if (IsHit) {
		//面座標計算
		//オブジェクト
		//上方向
		float ObjectUP = ObjectPos.y + ObjectSize.y * 0.5f;
		//下方向
		float ObjectDown = ObjectPos.y - ObjectSize.y * 0.5f;
		//左方向
		float ObjectLeft = ObjectPos.x - ObjectSize.x * 0.5f;
		//右方向
		float ObjectRight = ObjectPos.x + ObjectSize.x * 0.5f;
		//前方向
		float ObjectFlont = ObjectPos.z - ObjectSize.z * 0.5f;
		//奥方向
		float ObjectBack = ObjectPos.z + ObjectSize.z * 0.5f;
		//１フレーム前
		//上方向
		float PrevObjectUp = PrevPos.y + ObjectSize.y * 0.5f;
		//下方向
		float PrevObjectDown = PrevPos.y - ObjectSize.y * 0.5f;
		//足場
		//上方向
		float PlatformUp = PlatformPos.y + PlatformSize.y * 0.5f;
		//下方向
		float PlatformDown = PlatformPos.y - PlatformSize.y * 0.5f;
		//左方向
		float PlatformLeft = PlatformPos.x - PlatformSize.x * 0.5f;
		//右方向
		float PlatformRight = PlatformPos.x + PlatformSize.x * 0.5f;
		//前方向
		float PlatformFlont = PlatformPos.z - PlatformSize.z * 0.5f;
		//奥方向
		float PlatformBack = PlatformPos.z + PlatformSize.z * 0.5f;

		//押し戻し方向設定
		VECTOR PushVec = VZERO;
		//着地しているか
		bool IsLanding = false;
		//着地
		if (PrevObjectDown >= PlatformUp || ObjectDown >= PlatformUp) {
			//押し戻し量計算
			//上方向
			float PushUp = PlatformUp - ObjectDown;
			//押し戻し方向再設定
			PushVec = VGet(0.0f, PushUp, 0.0f);
			//重力をリセット
			_Object.GravityReset();
			//着地フラグをオン
			IsLanding = true;
		}
		//天井ヒット
		else if (PrevObjectUp <= PlatformDown) {
			//押し戻し量計算
			//下方向
			float PushDown = PlatformDown - ObjectUP;
			//押し戻し方向再設定
			PushVec = VGet(0.0f, PushDown, 0.0f);
		}
		else {
			//押し戻し量計算
			//左方向
			float PushLeft = PlatformRight - ObjectLeft;
			//右方向
			float PushRight = PlatformLeft - ObjectRight;
			//前方向
			float PushFront = PlatformBack - ObjectFlont;
			//奥方向
			float PushBack = PlatformFlont - ObjectBack;

			//最も押し戻し量の小さい方向を探す
			//角方向の値を絶対値に変換
			//左方向
			float PushLeftAbs = fabsf(PushLeft);
			//右方向
			float PushRightAbs = fabsf(PushRight);
			//前方向
			float PushFrontAbs = fabsf(PushFront);
			//奥方向
			float PushBackAbs = fabsf(PushBack);

			//一旦上方向が最も小さいと仮定する
			float MinPush = PushLeftAbs;
			//押し戻し方向再設定
			PushVec = VGet(PushLeft, 0.0f, 0.0f);
			//右方向と比較
			//小さければ
			if (PushRightAbs < MinPush) {
				//最小を更新
				MinPush = PushRightAbs;
				//押し戻し方向再設定
				PushVec = VGet(PushRight, 0.0f, 0.0f);
			}
			//前方向と比較
			//小さければ
			if (PushFrontAbs < MinPush) {
				//最小を更新
				MinPush = PushFrontAbs;
				//押し戻し方向再設定
				PushVec = VGet(0.0f, 0.0f, PushFront);
			}
			//奥方向と比較
			//小さければ
			if (PushBackAbs < MinPush) {
				//最小を更新
				MinPush = PushBackAbs;
				//押し戻し方向再設定
				PushVec = VGet(0.0f, 0.0f, PushBack);
			}
		}
		//押し戻し計算
		_Object.AddPos(PushVec);
		//オブジェクトの座標を更新
		ObjectPos = _Object.GetCenter(ObjectBase::TagShape::BOX);
	}
	else {
		//重力処理を行う
		_Object.SetIsGravity(true);
	}
}
//オブジェクトと足場の当たり判定
void HitCheck::ObjectToPlatform(ObjectBase& _Object, PlatformManager& _PlatformManager) {
	//オブジェクトの生存フラグがオフになっていれば以降の処理をしない
	if (!_Object.GetIsActive())return;
	//オブジェクトの座標取得
	VECTOR	ObjectPos = _Object.GetCenter(ObjectBase::TagShape::BOX);
	//オブジェクトの１フレーム前の座標取得
	VECTOR PrevPos = _Object.GetPrevCenter(ObjectBase::TagShape::BOX);
	//オブジェクトのサイズを取得
	VECTOR	ObjectSize = _Object.GetSize();
	//重力処理を行うか
	bool IsGravity = true;
#ifdef _DEBUG
	VECTOR ObjectPos1 = VGet(ObjectPos.x + ObjectSize.x / 2, ObjectPos.y + ObjectSize.y / 2, ObjectPos.z + ObjectSize.z / 2);
	VECTOR ObjectPos2 = VGet(ObjectPos.x - ObjectSize.x / 2, ObjectPos.y - ObjectSize.y / 2, ObjectPos.z - ObjectSize.z / 2);
	DrawCube3D(ObjectPos1, ObjectPos2, RED, RED, FALSE);
#endif // DEBUG
	for (int Index = 0; Index < PLATFORM_MAX; Index++) {
		//足場マネージャークラスから一つ取得
		PlatformBase& OnePlatform = _PlatformManager.GetPlatform(Index);
		//取得した足場クラスの生存フラグがオフになっていれば次のforへ
		if (!OnePlatform.GetIsActive())continue;
		//足場クラスがオブジェクトを押し戻さない設定になっているならば次のforへ
		if (!OnePlatform.GetObjectPush())continue;
		//オブジェクトがプレイヤーかつ足場クラスがプレイヤーを押し戻さない設定になっているならば次のforへ
		if (_Object.GetKinds() == ObjectBase::TagKinds::PLAYER && !OnePlatform.GetPlayerPush())continue;
		//オブジェクトが敵かつ足場クラスが敵を押し戻さない設定になっているならば次のforへ
		if (_Object.GetKinds() == ObjectBase::TagKinds::ENEMY && !OnePlatform.GetEnemyPush())continue;
		//足場クラスの座標取得
		VECTOR PlatformPos = OnePlatform.GetCenter(ObjectBase::TagShape::FIELD);
		//足場クラスのサイズを取得
		VECTOR PlatformSize = OnePlatform.GetSize();

		float Pos1X = PlatformPos.x + PlatformSize.x * 0.5f;
		float Pos1Y = PlatformPos.y + PlatformSize.y * 0.5f;
		float Pos1Z = PlatformPos.z + PlatformSize.z * 0.5f;

		float Pos2X = PlatformPos.x - PlatformSize.x * 0.5f;
		float Pos2Y = PlatformPos.y - PlatformSize.y * 0.5f;
		float Pos2Z = PlatformPos.z - PlatformSize.z * 0.5f;

		VECTOR PlatformPos1 = VGet(Pos1X, Pos1Y, Pos1Z);
		VECTOR PlatformPos2 = VGet(Pos2X, Pos2Y, Pos2Z);
#ifdef _DEBUG
		DrawCube3D(PlatformPos1, PlatformPos2, RED, RED, FALSE);
#endif // DEBUG
		//当たり判定
		bool IsHit = Collision::CheckHitBoxToBox(ObjectPos, ObjectSize, PlatformPos, PlatformSize);
		//当たっていれば
		if (IsHit) {
			//面座標計算
			//オブジェクト
			//上方向
			float ObjectUP = ObjectPos.y + ObjectSize.y * 0.5f;
			//下方向
			float ObjectDown = ObjectPos.y - ObjectSize.y * 0.5f;
			//左方向
			float ObjectLeft = ObjectPos.x - ObjectSize.x * 0.5f;
			//右方向
			float ObjectRight = ObjectPos.x + ObjectSize.x * 0.5f;
			//前方向
			float ObjectFlont = ObjectPos.z - ObjectSize.z * 0.5f;
			//奥方向
			float ObjectBack = ObjectPos.z + ObjectSize.z * 0.5f;
			//１フレーム前
			//上方向
			float PrevObjectUp = PrevPos.y + ObjectSize.y * 0.5f;
			//下方向
			float PrevObjectDown = PrevPos.y - ObjectSize.y * 0.5f;
			//足場
			//上方向
			float PlatformUp = PlatformPos.y + PlatformSize.y * 0.5f;
			//下方向
			float PlatformDown = PlatformPos.y - PlatformSize.y * 0.5f;
			//左方向
			float PlatformLeft = PlatformPos.x - PlatformSize.x * 0.5f;
			//右方向
			float PlatformRight = PlatformPos.x + PlatformSize.x * 0.5f;
			//前方向
			float PlatformFlont = PlatformPos.z - PlatformSize.z * 0.5f;
			//奥方向
			float PlatformBack = PlatformPos.z + PlatformSize.z * 0.5f;

			//押し戻し方向設定
			VECTOR PushVec = VZERO;
			//着地しているか
			bool IsLanding = false;
			//もし取得した足場クラスがアイテムだったなら
			if (OnePlatform.GetIsItem()) {
				//当たり判定後の処理(当たっている場合)
				_PlatformManager.HitCalc(Index, &_Object);
			}
			//着地
			else if (PrevObjectDown >= PlatformUp || ObjectDown >= PlatformUp) {
				//押し戻し量計算
				//上方向
				float PushUp = PlatformUp - ObjectDown;
				//押し戻し方向再設定
				PushVec = VGet(0.0f, PushUp, 0.0f);
				//重力処理を行わない
				IsGravity = false;
				//重力をリセット
				_Object.GravityReset();
				//着地フラグをオン
				IsLanding = true;
				//当たり判定後の処理(当たっている場合)
				_PlatformManager.HitCalc(Index, &_Object);
			}
			//天井ヒット
			else if (PrevObjectUp <= PlatformDown) {
				//押し戻し量計算
				//下方向
				float PushDown = PlatformDown - ObjectUP;
				//押し戻し方向再設定
				PushVec = VGet(0.0f, PushDown, 0.0f);
			}
			else {
				//押し戻し量計算
				//左方向
				float PushLeft = PlatformRight - ObjectLeft;
				//右方向
				float PushRight = PlatformLeft - ObjectRight;
				//前方向
				float PushFront = PlatformBack - ObjectFlont;
				//奥方向
				float PushBack = PlatformFlont - ObjectBack;

				//最も押し戻し量の小さい方向を探す
				//角方向の値を絶対値に変換
				//左方向
				float PushLeftAbs = fabsf(PushLeft);
				//右方向
				float PushRightAbs = fabsf(PushRight);
				//前方向
				float PushFrontAbs = fabsf(PushFront);
				//奥方向
				float PushBackAbs = fabsf(PushBack);

				//一旦上方向が最も小さいと仮定する
				float MinPush = PushLeftAbs;
				//押し戻し方向再設定
				PushVec = VGet(PushLeft, 0.0f, 0.0f);
				//右方向と比較
				//小さければ
				if (PushRightAbs < MinPush) {
					//最小を更新
					MinPush = PushRightAbs;
					//押し戻し方向再設定
					PushVec = VGet(PushRight, 0.0f, 0.0f);
				}
				//前方向と比較
				//小さければ
				if (PushFrontAbs < MinPush) {
					//最小を更新
					MinPush = PushFrontAbs;
					//押し戻し方向再設定
					PushVec = VGet(0.0f, 0.0f, PushFront);
				}
				//奥方向と比較
				//小さければ
				if (PushBackAbs < MinPush) {
					//最小を更新
					MinPush = PushBackAbs;
					//押し戻し方向再設定
					PushVec = VGet(0.0f, 0.0f, PushBack);
				}
			}
			//押し戻し計算
			_Object.AddPos(PushVec);
			//オブジェクトの座標を更新
			ObjectPos = _Object.GetCenter(ObjectBase::TagShape::BOX);
		}
		else {
			//当たり判定後の処理(当たっていない場合)
			_PlatformManager.NotHitCalc(Index, &_Object);
		}
	}
	//着地していなければ
	if (IsGravity) {
		//重力処理を行う
		_Object.SetIsGravity(true);
	}
}
//オブジェクトとギミックの当たり判定
void HitCheck::ObjectToGimmick(ObjectBase& _Object, GimmickManager& _GimmickManager) {
	//オブジェクトの生存フラグがオフになっていれば以降の処理をしない
	if (!_Object.GetIsActive())return;
	//オブジェクトの座標取得
	VECTOR	ObjectPos = _Object.GetCenter(ObjectBase::TagShape::BOX);
	//オブジェクトの１フレーム前の座標取得
	VECTOR PrevPos = _Object.GetPrevCenter(ObjectBase::TagShape::BOX);
	//オブジェクトのサイズを取得
	VECTOR	ObjectSize = _Object.GetSize();
#ifdef _DEBUG
	VECTOR ObjectPos1 = VGet(ObjectPos.x + ObjectSize.x / 2, ObjectPos.y + ObjectSize.y / 2, ObjectPos.z + ObjectSize.z / 2);
	VECTOR ObjectPos2 = VGet(ObjectPos.x - ObjectSize.x / 2, ObjectPos.y - ObjectSize.y / 2, ObjectPos.z - ObjectSize.z / 2);
	DrawCube3D(ObjectPos1, ObjectPos2, RED, RED, FALSE);
#endif // DEBUG
	for (int Index = 0; Index < GIMMICK_MAX; Index++) {
		//足場マネージャークラスから一つ取得
		GimmickBase& OneGimmick = _GimmickManager.GetGimmick(Index);
		//取得した足場クラスの生存フラグがオフになっていれば次のforへ
		if (!OneGimmick.GetIsActive())continue;
		//足場クラスの座標取得
		VECTOR GimmickPos = OneGimmick.GetCenter(ObjectBase::TagShape::FIELD);
		//足場クラスのサイズを取得
		VECTOR GimmickSize = OneGimmick.GetSize();

		float Pos1X = GimmickPos.x + GimmickSize.x * 0.5f;
		float Pos1Y = GimmickPos.y + GimmickSize.y * 0.5f;
		float Pos1Z = GimmickPos.z + GimmickSize.z * 0.5f;

		float Pos2X = GimmickPos.x - GimmickSize.x * 0.5f;
		float Pos2Y = GimmickPos.y - GimmickSize.y * 0.5f;
		float Pos2Z = GimmickPos.z - GimmickSize.z * 0.5f;

		VECTOR GimmickPos1 = VGet(Pos1X, Pos1Y, Pos1Z);
		VECTOR GimmickPos2 = VGet(Pos2X, Pos2Y, Pos2Z);
#ifdef _DEBUG
		DrawCube3D(GimmickPos1, GimmickPos2, RED, RED, FALSE);
#endif // DEBUG
		//当たり判定
		bool IsHit = Collision::CheckHitBoxToBox(ObjectPos, ObjectSize, GimmickPos, GimmickSize);
		//当たっていれば
		if (IsHit) {
			//面座標計算
			//オブジェクト
			//下方向
			float ObjectDown = ObjectPos.y - ObjectSize.y * 0.5f;
			//１フレーム前
			//下方向
			float PrevObjectDown = PrevPos.y - ObjectSize.y * 0.5f;
			//足場
			//上方向
			float GimmickUp = GimmickPos.y + GimmickSize.y * 0.5f;
			//下方向
			float GimmickDown = GimmickPos.y - GimmickSize.y * 0.5f;
			//着地
			if (PrevObjectDown >= GimmickUp || ObjectDown >= GimmickUp) {
				//当たり判定後の処理(当たっている場合)
				_GimmickManager.HitCalc(Index, &_Object);
			}
		}
	}
}
//モブ敵とオブジェクトの押し合い当たり判定
void HitCheck::MobEnemyToObjectPush(MobEnemyManager& _MobEnemyManager, ObjectBase& _Object) {
	for (int Index = 0; Index < MOB_ENEMY_MAX; Index++) {
		//モブ敵マネージャークラスから一つ取得
		MobEnemyBase& OneMobEnemy = _MobEnemyManager.GetMobEnemy(Index);
		//取得したモブ敵クラスの生存フラグがオフになっていれば次のforへ
		if (!OneMobEnemy.GetIsActive())continue;
		//オブジェクト同士の押し合い当たり判定
		ObjectToObjectPush(OneMobEnemy, _Object);
	}
}
//モブ敵に対するオブジェクトの攻撃当たり判定
void HitCheck::MobEnemyToObjectAttack(MobEnemyManager& _MobEnemyManager, ObjectBase& _AttackObject) {
	for (int Index = 0; Index < MOB_ENEMY_MAX; Index++) {
		//モブ敵マネージャークラスから一つ取得
		MobEnemyBase& OneMobEnemy = _MobEnemyManager.GetMobEnemy(Index);
		//取得したモブ敵クラスの生存フラグがオフになっていれば次のforへ
		if (!OneMobEnemy.GetIsActive())continue;
		//オブジェクトの攻撃当たり判定
		ObjectToObjectAttack(OneMobEnemy, _AttackObject);
	}
}
//オブジェクトとモブ敵の攻撃当たり判定
void HitCheck::ObjectToMobEnemyAttack(ObjectBase& _Object, MobEnemyManager& _MobEnemyManager) {
	for (int Index = 0; Index < MOB_ENEMY_MAX; Index++) {
		//モブ敵マネージャークラスから一つ取得
		MobEnemyBase& OneMobEnemy = _MobEnemyManager.GetMobEnemy(Index);
		//取得したモブ敵クラスの生存フラグがオフになっていれば次のforへ
		if (!OneMobEnemy.GetIsActive())continue;
		//オブジェクトの攻撃当たり判定
		ObjectToObjectAttack(_Object, OneMobEnemy);
	}
}
//モブ敵と足場の当たり判定
void HitCheck::MobEnemyToPlatform(MobEnemyManager& _MobEnemyManager, PlatformManager& _PlatformManager) {
	for (int Index = 0; Index < MOB_ENEMY_MAX; Index++) {
		//モブ敵マネージャークラスから一つ取得
		MobEnemyBase& OneMobEnemy = _MobEnemyManager.GetMobEnemy(Index);
		//取得したモブ敵クラスの生存フラグがオフになっていれば次のforへ
		if (!OneMobEnemy.GetIsActive())continue;
		//オブジェクトと足場の当たり判定
		ObjectToPlatform(OneMobEnemy, _PlatformManager);
	}
}
//Bossが使うObjectを管理するクラスとオブジェクトの当たり判定
void HitCheck::BossGimmickManagerToObjectPush(BossGimmickManager& _BossGimmickManager, ObjectBase& _ObjectBase) {
	//クリスタル
	for (int CrystalIndex = 0;CrystalIndex < CRYSTAL_MAX;CrystalIndex++) {
		//クリスタルを取得
		Crystal& OneCrystal = _BossGimmickManager.GetCrystal(CrystalIndex);
		//取得したクリスタルクラスの生存フラグがオフになっていれば次のforへ
		if (!OneCrystal.GetIsActive()) continue;
		//オブジェクト同士の押し合い当たり判定
		ObjectToObjectPush(OneCrystal, _ObjectBase);
	}
}
//Bossが使うObjectを管理するクラスとオブジェクトの当たり判定
void HitCheck::BossGimmickManagerToObjectAttack(BossGimmickManager& _BossGimmickManager, ObjectBase& _ObjectBase) {
	//クリスタル
	for (int CrystalIndex = 0;CrystalIndex < CRYSTAL_MAX;CrystalIndex++) {
		//クリスタルを取得
		Crystal& OneCrystal = _BossGimmickManager.GetCrystal(CrystalIndex);
		//取得したクリスタルクラスの生存フラグがオフになっていれば次のforへ
		if (!OneCrystal.GetIsActive()) continue;
		//オブジェクトの攻撃当たり判定]
		ObjectToObjectAttack(OneCrystal, _ObjectBase);
	}
}
