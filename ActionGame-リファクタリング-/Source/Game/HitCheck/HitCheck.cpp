#include "HitCheck.h"

//オブジェクト同士の押し合い当たり判定
void HitCheck::ObjectToObjectPush(ObjectBase& _ObjectA, ObjectBase& _ObjectB) {
	//どちらかのオブジェクトの生存フラグがオフになっていれば以降の処理をしない
	if (!_ObjectA.GetIsActive() || !_ObjectB.GetIsActive())return;
	//どちらかの当たり判定実行フラグがオフになっていれば以降の処理をしない
	if (!_ObjectA.GetIsCollision() || !_ObjectB.GetIsCollision())return;

	//Aの座標取得
	VECTOR	ObjectAPos = _ObjectA.GetCenter();
	//Bの座標取得
	VECTOR	ObjectBPos = _ObjectB.GetCenter();
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
	Col = MV1CollCheck_Sphere(_CollObject.GetHndl(), -1, _Object.GetCenter(), _Object.GetRad());
	//ポリゴンと当たっていたか
	if (Col.HitNum != 0) {
		for (int Index = 0; Index < Col.HitNum; Index++) {
			//まず中心点から最近点を引き算
			VECTOR Vec = VSub(_Object.GetCenter(), Col.Dim[Index].HitPosition);
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
				float fLenY = _Object.GetCenter().y - Col.Dim[Index].HitPosition.y;
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
	VECTOR	ObjectPos = _Object.GetPos();
	//オブジェクトのサイズを取得
	VECTOR	ObjectSize = _Object.GetSize();

	//同フレーム内に一度行った計算を再度行わないようにするためのフラグ
	bool IsObjectUPToPlatformDown = false;
	bool IsObjectDownToPlatformUP = false;
	bool IsObjectLeftToPlatformRight = false;
	bool IsObjectRightToPlatformLeft = false;
	bool IsObjectFlontToPlatformBack = false;
	bool IsObjectBackToPlatformFlont = false;

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
//		if (IsHit) {
//#ifdef _DEBUG
//			VECTOR PlatformPos1 = VGet(PlatformPos.x + OnePlatform.GetSize().x / 2, PlatformPos.y + OnePlatform.GetSize().y / 2, PlatformPos.z + OnePlatform.GetSize().z / 2);
//			VECTOR PlatformPos2 = VGet(PlatformPos.x - OnePlatform.GetSize().x / 2, PlatformPos.y - OnePlatform.GetSize().y / 2, PlatformPos.z - OnePlatform.GetSize().z / 2);
//			DrawCube3D(PlatformPos1, PlatformPos2, RED, RED, FALSE);
//
//#endif // DEBUG
//			//重力をリセット
//			_Object.GravityReset();
//
//			//オブジェクト
//			//上方向
//			float ObjectUP = ObjectPos.y + ObjectSize.y * 0.5f;
//			//下方向
//			float ObjectDown = ObjectPos.y - ObjectSize.y * 0.5f;
//			//左側
//			float ObjectLeft = ObjectPos.x - ObjectSize.x * 0.5f;
//			//右側
//			float ObjectRight = ObjectPos.x + ObjectSize.x * 0.5f;
//			//手前
//			float ObjectFlont = ObjectPos.z - ObjectSize.z * 0.5f;
//			//奥
//			float ObjectBack = ObjectPos.z + ObjectSize.z * 0.5f;
//			//足場
//			//上方向
//			float PlatformUP = PlatformPos.y + PlatformSize.y * 0.5f;
//			//下方向
//			float PlatformDown = PlatformPos.y - PlatformSize.y * 0.5f;
//			//左側
//			float PlatformLeft = PlatformPos.x - PlatformSize.x * 0.5f;
//			//右側
//			float PlatformRight = PlatformPos.x + PlatformSize.x * 0.5f;
//			//手前
//			float PlatformFlont = PlatformPos.z - PlatformSize.z * 0.5f;
//			//奥
//			float PlatformBack = PlatformPos.z + PlatformSize.z * 0.5f;
//
//			//float型の絶対値に変換
//			// //オブジェクト
//			//上方向
//			float AbsObjectUP = fabsf(ObjectUP);
//			//下方向
//			float AbsObjectDown = fabsf(ObjectDown);
//			//左側
//			float AbsObjectLeft = fabsf(ObjectLeft);
//			//右側
//			float AbsObjectRight = fabsf(ObjectRight);
//			//手前
//			float AbsObjectFlont = fabsf(ObjectFlont);
//			//奥
//			float AbsObjectBack = fabsf(ObjectBack);
//			//足場
//			//上方向
//			float AbsPlatformUP = fabsf(PlatformUP);
//			//下方向
//			float AbsPlatformDown = fabsf(PlatformDown);
//			//左側
//			float AbsPlatformLeft = fabsf(PlatformLeft);
//			//右側
//			float AbsPlatformRight = fabsf(PlatformRight);
//			//手前
//			float AbsPlatformFlont = fabsf(PlatformFlont);
//			//奥
//			float AbsPlatformBack = fabsf(PlatformBack);
//			//計算
//			//オブジェクトの上方向と足場の下方向
//			float ObjectUpAddPlatformDown = AbsObjectUP + AbsPlatformDown;
//			//オブジェクトの下方向と足場の上方向
//			float ObjectDownAddPlatformUP = AbsObjectDown + AbsPlatformUP;
//			//オブジェクトの左方向と足場の右方向
//			float ObjectLeftAddPlatformRight = AbsObjectLeft + AbsPlatformRight;
//			//オブジェクトの右方向と足場の左方向
//			float ObjectRightAddPlatformLeft = AbsObjectRight + AbsPlatformLeft;
//			//オブジェクトの手前方向と足場の奥方向
//			float ObjectFlontAddPlatformBack = AbsObjectFlont + AbsPlatformBack;
//			//オブジェクトの奥方向と足場の手前方向
//			float ObjectBackAddPlatformFlont = AbsObjectBack + AbsPlatformFlont;
//			//計算
//			//オブジェクトの上方向と足場の下方向
//			float ObjectUpAddPlatformDown = ObjectUP - PlatformDown;
//			//オブジェクトの下方向と足場の上方向
//			float ObjectDownAddPlatformUP = ObjectDown - PlatformUP;
//			//オブジェクトの左方向と足場の右方向
//			float ObjectLeftAddPlatformRight = ObjectLeft - PlatformRight;
//			//オブジェクトの右方向と足場の左方向
//			float ObjectRightAddPlatformLeft = ObjectRight - PlatformLeft;
//			//オブジェクトの手前方向と足場の奥方向
//			float ObjectFlontAddPlatformBack = ObjectFlont - PlatformBack;
//			//オブジェクトの奥方向と足場の手前方向
//			float ObjectBackAddPlatformFlont = ObjectBack - PlatformFlont;
//			//オブジェクトの上方向と足場の下方向
//			float ObjectUpAddPlatformDown = PlatformDown - ObjectUP;
//			//オブジェクトの下方向と足場の上方向
//			float ObjectDownAddPlatformUP = PlatformUP - ObjectDown;
//			//オブジェクトの左方向と足場の右方向
//			float ObjectLeftAddPlatformRight = PlatformRight - ObjectLeft;
//			//オブジェクトの右方向と足場の左方向
//			float ObjectRightAddPlatformLeft = PlatformLeft - ObjectRight;
//			//オブジェクトの手前方向と足場の奥方向
//			float ObjectFlontAddPlatformBack = PlatformBack - ObjectFlont;
//			//オブジェクトの奥方向と足場の手前方向
//			float ObjectBackAddPlatformFlont = PlatformFlont - ObjectBack;
//
//			//6つの中で最も小さい値の方向に押し返す
//			//オブジェクトの上方向と足場の下方向が最も小さい
//			if (ObjectUpAddPlatformDown <= ObjectDownAddPlatformUP &&
//				ObjectUpAddPlatformDown <= ObjectLeftAddPlatformRight &&
//				ObjectUpAddPlatformDown <= ObjectRightAddPlatformLeft &&
//				ObjectUpAddPlatformDown <= ObjectFlontAddPlatformBack &&
//				ObjectUpAddPlatformDown <= ObjectBackAddPlatformFlont) {
//				//同フレーム内に一度計算が行われていれば
//				if (IsObjectUPToPlatformDown)continue;
//				//押し戻す方向を生成
//				float Dir = PlatformDown - ObjectUP;
//				//VECTOR型に格納
//				VECTOR CalcVec = VGet(0.0f, Dir, 0.0f);
//				//オブジェクトの座標を計算した分だけ移動させる
//				_Object.AddPos(CalcVec);
//				//計算終了に変更
//				IsObjectUPToPlatformDown = true;
//			}
//			//オブジェクトの下方向と足場の上方向が最も小さい
//			if (ObjectDownAddPlatformUP <= ObjectUpAddPlatformDown &&
//				ObjectDownAddPlatformUP <= ObjectLeftAddPlatformRight &&
//				ObjectDownAddPlatformUP <= ObjectRightAddPlatformLeft &&
//				ObjectDownAddPlatformUP <= ObjectFlontAddPlatformBack &&
//				ObjectDownAddPlatformUP <= ObjectBackAddPlatformFlont) {
//				//同フレーム内に一度計算が行われていれば
//				if (IsObjectDownToPlatformUP)continue;
//				//押し戻す方向を生成
//				float Dir = PlatformUP - ObjectDown;
//				//VECTOR型に格納
//				VECTOR CalcVec = VGet(0.0f, Dir, 0.0f);
//				//オブジェクトの座標を計算した分だけ移動させる
//				_Object.AddPos(CalcVec);
//				//計算終了に変更
//				IsObjectDownToPlatformUP = true;
//			}
//			//オブジェクトの左方向と足場の右方向が最も小さい
//			if (ObjectLeftAddPlatformRight <= ObjectUpAddPlatformDown &&
//				ObjectLeftAddPlatformRight <= ObjectDownAddPlatformUP &&
//				ObjectLeftAddPlatformRight <= ObjectRightAddPlatformLeft &&
//				ObjectLeftAddPlatformRight <= ObjectFlontAddPlatformBack &&
//				ObjectLeftAddPlatformRight <= ObjectBackAddPlatformFlont) {
//				//同フレーム内に一度計算が行われていれば
//				if (IsObjectLeftToPlatformRight)continue;
//				//押し戻す方向を生成
//				float Dir = PlatformRight - ObjectLeft;
//				//VECTOR型に格納
//				VECTOR CalcVec = VGet(Dir, 0.0f, 0.0f);
//				//オブジェクトの座標を計算した分だけ移動させる
//				_Object.AddPos(CalcVec);
//				//計算終了に変更
//				IsObjectLeftToPlatformRight = true;
//			}
//			//オブジェクトの右方向と足場の左方向が最も小さい
//			if (ObjectRightAddPlatformLeft <= ObjectUpAddPlatformDown &&
//				ObjectRightAddPlatformLeft <= ObjectDownAddPlatformUP &&
//				ObjectRightAddPlatformLeft <= ObjectLeftAddPlatformRight &&
//				ObjectRightAddPlatformLeft <= ObjectFlontAddPlatformBack &&
//				ObjectRightAddPlatformLeft <= ObjectBackAddPlatformFlont) {
//				//同フレーム内に一度計算が行われていれば
//				if (IsObjectRightToPlatformLeft)continue;
//				//押し戻す方向を生成
//				float Dir = PlatformLeft - ObjectRight;
//				//VECTOR型に格納
//				VECTOR CalcVec = VGet(Dir, 0.0f, 0.0f);
//				//オブジェクトの座標を計算した分だけ移動させる
//				_Object.AddPos(CalcVec);
//				//計算終了に変更
//				IsObjectRightToPlatformLeft = true;
//			}
//			//オブジェクトの手前方向と足場の奥方向が最も小さい
//			if (ObjectFlontAddPlatformBack <= ObjectUpAddPlatformDown &&
//				ObjectFlontAddPlatformBack <= ObjectDownAddPlatformUP &&
//				ObjectFlontAddPlatformBack <= ObjectLeftAddPlatformRight &&
//				ObjectFlontAddPlatformBack <= ObjectRightAddPlatformLeft &&
//				ObjectFlontAddPlatformBack <= ObjectBackAddPlatformFlont) {
//				//同フレーム内に一度計算が行われていれば
//				if (IsObjectFlontToPlatformBack)continue;
//				//押し戻す方向を生成
//				float Dir = PlatformBack - ObjectFlont;
//				//VECTOR型に格納
//				VECTOR CalcVec = VGet(0.0f, 0.0f, Dir);
//				//オブジェクトの座標を計算した分だけ移動させる
//				_Object.AddPos(CalcVec);
//				//計算終了に変更
//				IsObjectFlontToPlatformBack = true;
//			}
//			//オブジェクトの奥方向と足場の手前方向
//			if (ObjectBackAddPlatformFlont <= ObjectUpAddPlatformDown &&
//				ObjectBackAddPlatformFlont <= ObjectDownAddPlatformUP &&
//				ObjectBackAddPlatformFlont <= ObjectLeftAddPlatformRight &&
//				ObjectBackAddPlatformFlont <= ObjectRightAddPlatformLeft &&
//				ObjectBackAddPlatformFlont <= ObjectFlontAddPlatformBack) {
//				//同フレーム内に一度計算が行われていれば
//				if (IsObjectBackToPlatformFlont)continue;
//				//押し戻す方向を生成
//				float Dir = PlatformFlont - ObjectBack;
//				//VECTOR型に格納
//				VECTOR CalcVec = VGet(0.0f, 0.0f, Dir);
//				//オブジェクトの座標を計算した分だけ移動させる
//				_Object.AddPos(CalcVec);
//				//計算終了に変更
//				IsObjectBackToPlatformFlont = true;
//			}
//			//当たり判定後の処理(当たっている場合)
//			_PlatformManager.HitCalc(Index);
//		}
//		else {
//			//当たり判定後の処理(当たっていない場合)
//			_PlatformManager.NotHitCalc(Index);
//		}
//		//どの押し戻し処理も実行されていなければ
//		if (!IsObjectUPToPlatformDown
//			&& !IsObjectDownToPlatformUP
//			&& !IsObjectLeftToPlatformRight
//			&& !IsObjectRightToPlatformLeft
//			&& !IsObjectFlontToPlatformBack
//			&& !IsObjectBackToPlatformFlont) {
//			//何も触れていなければ重力処理をオンにする
//			_Object.SetIsGravity(true);
//		}
	}
}
