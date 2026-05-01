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
		for (int Index = 0;Index < Col.HitNum;Index++) {
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

	for (int Index = 0;Index < PLATFORM_MAX;Index++) {
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
			VECTOR PlatformPos1 = VGet(PlatformPos.x + OnePlatform.GetSize().x / 2, PlatformPos.y + OnePlatform.GetSize().y / 2, PlatformPos.z + OnePlatform.GetSize().z / 2);
			VECTOR PlatformPos2 = VGet(PlatformPos.x - OnePlatform.GetSize().x / 2, PlatformPos.y - OnePlatform.GetSize().y / 2, PlatformPos.z - OnePlatform.GetSize().z / 2);
			DrawCube3D(PlatformPos1, PlatformPos2, RED, RED, FALSE);

#endif // DEBUG
			//重力をリセット
			_Object.GravityReset();
			//まず中心点から最近点を引き算
			VECTOR Vec = VSub(ObjectPos, PlatformPos);
			//
			Vec.x = 0.0f;
			Vec.z = 0.0f;
			//取得したベクトルを三平方の定理で長さに変換
			float Len = VSize(Vec);
			//正規化
			Vec = VNorm(Vec);
			
			Len = _Object.GetRad() - Len;
		
			Vec = VScale(Vec, Len);
			//オブジェクトの座標を計算した分だけ移動させる
			_Object.SetPos(VAdd(_Object.GetPos(), Vec));
			return;
		}
		else {
			//何も触れていなければ重力処理をオンにする
			_Object.SetIsGravity(true);
		}
	}
}
