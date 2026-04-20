#include "HitCheck.h"

//オブジェクト同士の押し合い当たり判定
void HitCheck::ObjectToObjectPush(ObjectBase& _ObjectA, ObjectBase& _ObjectB) {

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
			//プレイヤーの座標を計算した分だけ移動させる
			_Object.SetPos(VAdd(_Object.GetPos(), Vec));
			//法線を取得
			VECTOR Normal = Col.Dim[Index].Normal;
			//法線の角度を取得
			float Angle = atan2f(Normal.y, Normal.x);
			//角度が90度の場合足元にあるかを判断する
			if (Angle == 90.0f * (DX_PI_F / 180.0f)){
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
	//毎回データを削除
	MV1CollResultPolyDimTerminate(Col);
}
