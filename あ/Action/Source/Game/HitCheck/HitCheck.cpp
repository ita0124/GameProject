#include "HitCheck.h"
#include "Game/Object/Actor/Character/Player/Player.h"
#include "Game/Object/Actor/Character/Enemy/Boss/Boss.h"
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
	//Y軸を0.0fに固定
	ObjectAPos.y = 0.0f;
	ObjectBPos.y = 0.0f;
	//当たり判定
	bool IsHit = Collision::CheckHitSphereToSphere(ObjectAPos, ObjectARad, ObjectBPos, ObjectBRad);
	//当たっていれば
	if (IsHit) {
#ifdef _DEBUG
		DrawSphere3D(ObjectAPos, ObjectARad, DIV, RED, RED, FALSE);
		DrawSphere3D(ObjectBPos, ObjectBRad, DIV, RED, RED, FALSE);
#endif // DEBUG
		//Aの押し出しフラグがオンなら
		if (_ObjectA.GetIsPush()) {
			//BからAへの方向ベクトルを生成
			VECTOR Dir = VSub(ObjectAPos, ObjectBPos);
			//Y軸は考えない
			Dir.y = 0.0f;
			//長さ取得
			float Len = VSize(Dir);
			//正規化
			Dir = VNorm(Dir);
			//押し出す長さを取得
			Len = (ObjectARad + ObjectBRad) - Len;
			//方向ベクトルに押し出す長さを掛ける
			Dir = VScale(Dir, Len);
			//念のためY軸を0に
			Dir.y = 0.0f;
			//現在の座標に加算
			_ObjectA.AddPos(Dir);
		}
		if (_ObjectB.GetIsPush()) {
			//AからBへの方向ベクトルを生成
			VECTOR Dir = VSub(ObjectBPos, ObjectAPos);
			//Y軸は考えない
			Dir.y = 0.0f;
			//長さ取得
			float Len = VSize(Dir);
			//正規化
			Dir = VNorm(Dir);
			//押し出す長さを取得
			Len = (ObjectARad + ObjectBRad) - Len;
			//方向ベクトルに押し出す長さを掛ける
			Dir = VScale(Dir, Len);
			//念のためY軸を0に
			Dir.y = 0.0f;
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
	//オブジェクトがプレイヤーかつ攻撃オブジェクトがボスだった場合
	if (_Object.GetKinds() == ObjectBase::TagKinds::PLAYER && _AttackObject.GetKinds() == ObjectBase::TagKinds::BOSS) {
		//ボスクラスデータを保存する変数
		Boss* PointerBoss = nullptr;
		//ボスクラスをダウンキャスト
		PointerBoss = dynamic_cast<Boss*>(&_AttackObject);
		//ボスのフレーム分
		for (int FrameNum = 0;FrameNum < Boss::FrameNamber::FARAM_NUM;FrameNum++) {
			//フレームの攻撃判定がオフになっていたら次のフレームを見る
			if (!PointerBoss->GetFrameDataIsAttackFlg(FrameNum))continue;
			//プレイヤーの座標を取得
			VECTOR	PlayerPos1 = _Object.GetCenter();
			//プレイヤーの頭の座標を取得
			VECTOR	PlayerPos2 = _Object.GetFramePos(_Object.GetHndl(), Player::FrameNamber::HEAD);
			//プレイヤーの当たり判定半径を取得
			float	PlayerRad = _Object.GetRad();
			//ボスの座標を取得
			VECTOR	BossPos = PointerBoss->GetFrameDataPos(FrameNum);
			//ボスの当たり判定半径を取得
			float	BossRad = PointerBoss->GetFrameDataRad(FrameNum);
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
				_Object.HitCalc(PointerBoss);
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
		if (PointerPlayer->GetIsActionSuccess(Player::TagState::GUARD))return;
		//攻撃当たり判定を生成してよいか
		if (PointerPlayer->GetIsGuardCollision()) {
			//ボスクラスデータを保存する変数
			Boss* PointerBoss = nullptr;
			//ボスクラスをダウンキャスト
			PointerBoss = dynamic_cast<Boss*>(&_AttackObject);
			//ボスのフレーム分
			for (int FrameNum = 0;FrameNum <= Boss::FrameNamber::TOSEEND_RIGHT;FrameNum++) {
				//ガード成功フラグがオンになっていたら以降の処理は行わない
				if (PointerPlayer->GetIsActionSuccess(Player::TagState::GUARD))return;
				//パリィ成功フラグがオンになっていたら以降の処理は行わない
				if (PointerPlayer->GetIsActionSuccess(Player::TagState::PARRY))return;
				//フレームの攻撃判定がオフになっていたら次のフレームを見る
				if (!PointerBoss->GetFrameDataIsAttackFlg(FrameNum))continue;
				//プレイヤーの座標を取得
				VECTOR	ShieldPos = _Object.GetPos();
				//プレイヤーの当たり判定半径を取得
				float	ShieldRad = _Object.GetRad();
				//ボスの座標を取得
				VECTOR	BossPos = PointerBoss->GetFrameDataPos(FrameNum);
				//ボスの当たり判定半径を取得
				float	BossRad = PointerBoss->GetFrameDataRad(FrameNum);
#ifdef _DEBUG
				//パリィ許容フラグがオンなら
				if (PointerPlayer->GetIsParryWindo()) {
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
					//パリィ許容フラグがオンなら
					if (PointerPlayer->GetIsParryWindo()) {
						//パリィの処理
						PointerPlayer->HitCalc(PointerBoss);
						PointerBoss->HitCalc(PointerPlayer);
					}
					else {
						//ガードの処理1
						PointerPlayer->HitCalc(PointerBoss);
					}
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
			VECTOR	SwordPos = _AttackObject.GetFramePos(_AttackObject.GetHndl(), Sword::FrameNamber::BLADE002);
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
	if (_Object.GetKinds() == ObjectBase::TagKinds::PLAYER && _RelativePosObject.GetKinds() == ObjectBase::TagKinds::BOSS) {
		//ボスクラスデータを保存する変数
		Boss* PointerBoss = nullptr;
		//ボスクラスをダウンキャスト
		PointerBoss = dynamic_cast<Boss*>(&_RelativePosObject);
		for (int FrameNum = Boss::FrameNamber::FRONT;FrameNum < Boss::FrameNamber::FARAM_NUM;FrameNum++) {
			//プレイヤーの座標を取得
			VECTOR	PlayerPos1 = _Object.GetCenter();
			//プレイヤーの頭の座標を取得
			VECTOR	PlayerPos2 = _Object.GetFramePos(_Object.GetHndl(), Player::FrameNamber::HEAD);
			//プレイヤーの当たり判定半径を取得
			float	PlayerRad = _Object.GetRad();
			//ボスの座標を取得
			VECTOR	BossPos = PointerBoss->GetFrameDataPos(FrameNum);
			//ボスの当たり判定半径を取得
			float	BossRad = PointerBoss->GetFrameDataRad(FrameNum);
#ifdef _DEBUG
			DrawSphere3D(PlayerPos1, PlayerRad, DIV, RED, RED, FALSE);
			DrawSphere3D(PlayerPos2, PlayerRad, DIV, RED, RED, FALSE);
			DrawSphere3D(BossPos, BossRad, DIV, RED, RED, FALSE);
#endif // DEBUG
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
//Collとオブジェクトの当たり判定
void HitCheck::CollToObject(ObjectBase& _CollObject, ObjectBase& _Object) {
	//当たり判定が格納される構造体
	MV1_COLL_RESULT_POLY_DIM Col;
	//当たり判定を行い、その結果を構造体に格納
	Col = MV1CollCheck_Sphere(_CollObject.GetHndl(), -1, _Object.GetCenter(ObjectBase::TagShape::BALL), _Object.GetRad());
	//ポリゴンと当たっていたか
	if (Col.HitNum != 0) {
		for (int Index = 0; Index < Col.HitNum; Index++) {
			//まず中心点から最近点を引き算
			VECTOR Vec = VSub(_Object.GetCenter(ObjectBase::TagShape::BALL), Col.Dim[Index].HitPosition);
			//取得したベクトルを三平方の定理で長さに変換
			float Len = VSize(Vec);
			//法線をめり込んだ距離分掛け算する
			Len = _Object.GetRad() - Len;
			//法線をめり込んだ距離分掛け算する
			Vec = VScale(Col.Dim[Index].Normal, Len);
			//オブジェクトの座標を計算した分だけ移動させる
			_Object.SetPos(VAdd(_Object.GetPos(), Vec));
			//法線を取得
			VECTOR Normal = Col.Dim[Index].Normal;
			//法線の角度を取得
			float Angle = atan2f(Normal.y, Normal.x);
			//角度が90度の場合足元にあるかを判断する
			if (Angle == 90.0f * (DX_PI_F / 180.0f)) {
				float fLenY = _Object.GetCenter(ObjectBase::TagShape::BALL).y - Col.Dim[Index].HitPosition.y;
				//着地した場合重力をリセットする
				if (_Object.GetPos().y - Col.Dim[Index].HitPosition.y < 5.0f)
				{
					//重力をリセット
					_Object.GravityReset();
				}
			}
		}
	}
	else {
		//何も触れていなければ重力処理をオンにする
		_Object.SetIsGravity(true);
	}
	//毎回データを削除
	MV1CollResultPolyDimTerminate(Col);
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
		//足場クラスの座標取得
		VECTOR PlatformPos = OnePlatform.GetCenter();
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
			//着地
			if (PrevObjectDown >= PlatformUp || ObjectDown >= PlatformUp) {
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
