#pragma once
#include <DxLib.h>

//当たり判定管理クラス
class Collision {
public:
	//点と矩形の当たり判定
	//_DotPos		:点の座標
	//_SquarePos	:矩形の中心座標
	//_Width		:矩形の横幅
	//_Height		:矩形の縦幅
	//@return		:true=当たった false=当たっていない
	static bool CheckHitDotToSquare(VECTOR _DotPos, VECTOR _SquarePos, float _Width, float _Height);

	//矩形同士の当たり判定
	//_SquarePos	:矩形の中心座標
	//_Width		:矩形の横幅
	//_Height		:矩形の縦幅
	static bool CheckHitSquareToSquare(VECTOR _SquarePos1, float _Width1, float _Height1, VECTOR _SquarePos2, float _Width2, float _Height2);

	//円同士の当たり判定
	//_CirclePos	:円の中心座標
	//_Radius1		:円の半径
	static bool CheckHitCircleToCircle(VECTOR _CirclePos1, float _Radius1, VECTOR _CirclePos2, float _Radius2);

	//ボックス同士の当たり判定
	//_BoxPos		:ボックスの中心座標
	//_BoxSize		:ボックスのサイズ
	static bool CheckHitBoxToBox(VECTOR _BoxPos1, VECTOR _BoxSize1, VECTOR _BoxPos2, VECTOR _BoxSize2);

	//球同士の当たり判定
	//_SpherePos1	:球の中心座標
	//_Radius1		:球の半径
	static bool CheckHitSphereToSphere(VECTOR _SpherePos1, float _Radius1, VECTOR _SpherePos2, float _Radius2);

};
