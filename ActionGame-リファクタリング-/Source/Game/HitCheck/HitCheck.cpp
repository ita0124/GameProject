#include "HitCheck.h"

//オブジェクト同士の押し合い当たり判定
void HitCheck::ObjectToObjectPush(ObjectBase& _ObjectA, ObjectBase& _ObjectB) {
	//どちらかのオブジェクトの生存フラグがオフになっていれば以降の処理をしない
	if (!_ObjectA.GetIsActive() || !_ObjectB.GetIsActive())return;
	//どちらかの当たり判定実行フラグがオフになっていれば以降の処理をしない
	if (!_ObjectA.GetIsCollision() || !_ObjectB.GetIsCollision())return;

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
			_ObjectA.AddPos(Dir);
		}
	}
}
//オブジェクト同士の攻撃当たり判定
void HitCheck::ObjectToObjectAttack(ObjectBase& _ObjectA, ObjectBase& _ObjectB) {

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
	//オブジェクトの前フレーム座標取得
	VECTOR PrevPos = _Object.GetPrevPos();
	//オブジェクトのサイズを取得
	VECTOR	ObjectSize = _Object.GetSize();
	//重力処理を行うか
	bool IsGravity = true;
	//動く床と当たったか
	bool IsMovingPlatform = false;

	DrawFormatStringToHandle(50, 100, RED, DxLibFont::FONTHNDL_N20, "プレイヤー前フレームY座標:%f", PrevPos.y);
	DrawFormatStringToHandle(50, 120, RED, DxLibFont::FONTHNDL_N20, "当たり判定計算前プレイヤーY座標:%f", ObjectPos.y);

	for (int Index = 0; Index < PLATFORM_MAX; Index++) {
		//足場マネージャークラスから一つ取得
		PlatformBase& OnePlatform = _PlatformManager.GetPlatform(Index);
		//取得した足場クラスの生存フラグがオフになっていれば次のforへ
		if (!OnePlatform.GetIsActive())continue;
		//足場クラスの座標取得
		VECTOR	PlatformPos = OnePlatform.GetCenter();
		//足場クラスのサイズを取得
		VECTOR	PlatformSize = OnePlatform.GetSize();

		//当たり判定
		bool IsHit = Collision::CheckHitBoxToBox(ObjectPos, ObjectSize, PlatformPos, PlatformSize);
		//当たっていれば
		if (IsHit) {
#ifdef _DEBUG
			VECTOR PlatformPos1 = VGet(PlatformPos.x + PlatformSize.x / 2, PlatformPos.y + PlatformSize.y / 2, PlatformPos.z + PlatformSize.z / 2);
			VECTOR PlatformPos2 = VGet(PlatformPos.x - PlatformSize.x / 2, PlatformPos.y - PlatformSize.y / 2, PlatformPos.z - PlatformSize.z / 2);
			DrawCube3D(PlatformPos1, PlatformPos2, RED, RED, FALSE);

			VECTOR ObjectPos1 = VGet(ObjectPos.x + ObjectSize.x / 2, ObjectPos.y + ObjectSize.y / 2, ObjectPos.z + ObjectSize.z / 2);
			VECTOR ObjectPos2 = VGet(ObjectPos.x - ObjectSize.x / 2, ObjectPos.y - ObjectSize.y / 2, ObjectPos.z - ObjectSize.z / 2);
			DrawCube3D(ObjectPos1, ObjectPos2, RED, RED, FALSE);

			DrawFormatStringToHandle(50, 200, RED, DxLibFont::FONTHNDL_N20, "足場ののY座標：%f", PlatformPos.y);
#endif // DEBUG
			//面座標計算
			//オブジェクト
			//上方向
			float ObjectUP = ObjectPos.y + ObjectSize.y * 0.5f;
			//下方向
			float ObjectDown = ObjectPos.y- ObjectSize.y * 0.5f;
			//左方向
			float ObjectLeft = ObjectPos.x - ObjectSize.x * 0.5f;
			//右方向
			float ObjectRight = ObjectPos.x + ObjectSize.x * 0.5f;
			//前方向
			float ObjectFlont = ObjectPos.z - ObjectSize.z * 0.5f;
			//奥方向
			float ObjectBack = ObjectPos.z + ObjectSize.z * 0.5f;
			//前フレーム
			//上方向
			float PrevObjectUp = PrevPos.y + ObjectSize.y * 0.5f;
			//下方向
			float PrevObjectDown = PrevPos.y;
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
			//着地
			if (PrevObjectDown >= PlatformUp) {
				//押し戻し量計算
				//上方向
				float PushUp = PlatformUp - ObjectDown;
				//押し戻し方向再設定
				PushVec = VGet(0.0f, PushUp, 0.0f);
				//重力処理を行わない
				IsGravity = false;
				//重力をリセット
				_Object.GravityReset();

				DrawFormatStringToHandle(50, 140, RED, DxLibFont::FONTHNDL_N20, "プレイヤー下面:%f", ObjectDown);
				DrawFormatStringToHandle(50, 240, RED, DxLibFont::FONTHNDL_N20, "上方向の押し戻し量%f", PushUp);
				DrawFormatStringToHandle(50, 260, RED, DxLibFont::FONTHNDL_N20, "足場上面：%f", PlatformUp);
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
			//もし当たった物体が移動する床だったなら
			if (OnePlatform.GetPlatformKinds() == PlatformBase::TagPlatformKinds::MOVING) {
				//動く足場データを保存する用の変数
				MovingPlatform* MovePlatform = nullptr;
				//動く足場クラスにダウンキャストする
				MovePlatform = dynamic_cast<MovingPlatform*>(&OnePlatform);
				//移動方向ベクトルを取得
				VECTOR MoveDir = MovePlatform->GetMoveDir();
				//移動速度を取得
				float MoveSpeed = MovePlatform->GetMovingPlatformRequestData().MoveSpeed;
				//移動方向ベクトルに移動速度を乗算
				MoveDir = VScale(MoveDir, MoveSpeed);
				//押し戻しに移動方向ベクトルを加算する
				PushVec = VAdd(PushVec, MoveDir);
				//押し戻し計算
				_Object.AddPos(PushVec);
				//オブジェクトの座標を更新
				ObjectPos = _Object.GetCenter(ObjectBase::TagShape::BOX);
				//当たり判定後の処理(当たっている場合)
				_PlatformManager.HitCalc(Index);
			}
			else {
				//押し戻し計算
				_Object.AddPos(PushVec);
				//オブジェクトの座標を更新
				ObjectPos = _Object.GetCenter(ObjectBase::TagShape::BOX);
				//当たり判定後の処理(当たっている場合)
				_PlatformManager.HitCalc(Index);
			}
		}
		else {
			//当たり判定後の処理(当たっていない場合)
			_PlatformManager.NotHitCalc(Index);
		}
	}
	//着地していなければ
	if (IsGravity) {
		//重力処理を行う
		_Object.SetIsGravity(true);
	}

	DrawFormatStringToHandle(50, 300, RED, DxLibFont::FONTHNDL_N20, "当たり判定計算後プレイヤーY座標:%f", ObjectPos.y);
}
