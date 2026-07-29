#include "Collision.h"

//点と矩形の当たり判定
bool Collision::CheckHitDotToSquare(VECTOR _DotPos, VECTOR _SquarePos, float _Width, float _Height) {
	//矩形の上下左右それぞれの座標を計算する
	float Up = _SquarePos.y - _Height * 0.5f;
	float Down = _SquarePos.y + _Height * 0.5f;
	float Left = _SquarePos.x - _Width * 0.5f;
	float Right = _SquarePos.x + _Width * 0.5f;

	//4つの端をそれぞれチェックし、すべての条件を満たしていた場合true
	if (_DotPos.y >= Up && _DotPos.y <= Down
		&& _DotPos.x >= Left && _DotPos.x <= Right) {
		return true;
	}
	else {
		return false;
	}
}

//矩形同士の当たり判定
bool Collision::CheckHitSquareToSquare(VECTOR _SquarePos1, float _Width1, float _Height1, VECTOR _SquarePos2, float _Width2, float _Height2) {
	//矩形の上下左右それぞれの座標を計算する(1つ目)
	float Up1 = _SquarePos1.y - _Height1 * 0.5f;
	float Down1 = _SquarePos1.y + _Height1 * 0.5f;
	float Left1 = _SquarePos1.x - _Width1 * 0.5f;
	float Right1 = _SquarePos1.x + _Width1 * 0.5f;

	//矩形の上下左右それぞれの座標を計算する(2つ目)
	float Up2 = _SquarePos2.y - _Height2 * 0.5f;
	float Down2 = _SquarePos2.y + _Height2 * 0.5f;
	float Left2 = _SquarePos2.x - _Width2 * 0.5f;
	float Right2 = _SquarePos2.x + _Width2 * 0.5f;

	/*bool isA = Up1 <= Down2
		&& Down1 <= Up2
		&& Left1 <= Right2
		&& Right1 <= Left2 ? true : false;

	return isA;*/

	if (Up1 <= Down2 && Down1 >= Up2
		&& Left1 <= Right2 && Right1 >= Left2) {
		return true;
	}
	else {
		return false;
	}
}

//円同士の当たり判定
bool Collision::CheckHitCircleToCircle(VECTOR _CirclePos1, float _Radius1, VECTOR _CirclePos2, float _Radius2) {
	//横幅の2乗
	float LenX = _CirclePos1.x - _CirclePos2.x;
	LenX *= LenX;

	//縦幅の2乗
	float LenY = _CirclePos1.y - _CirclePos2.y;
	LenY *= LenY;

	//最短距離の2乗を計算
	float Len = LenX + LenY;

	//半径２個を足した値の2乗
	float TotalRadius = _Radius1 + _Radius2;
	TotalRadius *= TotalRadius;

	//最短距離の2乗 <= 半径を2個を足した値の2乗
	if (Len <= TotalRadius) {
		return true;
	}
	else {
		return false;
	}
}

//ボックス同士の当たり判定
bool Collision::CheckHitBoxToBox(VECTOR _BoxPos1, VECTOR _BoxSize1, VECTOR _BoxPos2, VECTOR _BoxSize2) {
	//矩形の上下左右前奥それぞれの座標を計算する(1つ目)
	float Up1 = _BoxPos1.y + _BoxSize1.y * 0.5f;
	float Down1 = _BoxPos1.y - _BoxSize1.y * 0.5f;
	float Left1 = _BoxPos1.x - _BoxSize1.x * 0.5f;
	float Right1 = _BoxPos1.x + _BoxSize1.x * 0.5f;
	float Flont1 = _BoxPos1.z - _BoxSize1.z * 0.5f;
	float Back1 = _BoxPos1.z + _BoxSize1.z * 0.5f;

	//矩形の上下左右前奥それぞれの座標を計算する(2つ目)
	float Up2 = _BoxPos2.y + _BoxSize2.y * 0.5f;
	float Down2 = _BoxPos2.y - _BoxSize2.y * 0.5f;
	float Left2 = _BoxPos2.x - _BoxSize2.x * 0.5f;
	float Right2 = _BoxPos2.x + _BoxSize2.x * 0.5f;
	float Flont2 = _BoxPos2.z - _BoxSize2.z * 0.5f;
	float Back2 = _BoxPos2.z + _BoxSize2.z * 0.5f;

	//6つの端をそれぞれチェックし、すべての条件を満たしていた場合true
	if (Up1 >= Down2 && Down1 <= Up2
		&& Left1 <= Right2 && Right1 >= Left2
		&& Flont1 <= Back2 && Back1 >= Flont2) {
		return true;
	}
	else {
		return false;
	}
}

//球同士の当たり判定
bool Collision::CheckHitSphereToSphere(VECTOR _SpherePos1, float _Radius1, VECTOR _SpherePos2, float _Radius2) {
	//横幅の2乗
	float LenX = _SpherePos1.x - _SpherePos2.x;
	LenX *= LenX;

	//縦幅の2乗
	float LenY = _SpherePos1.y - _SpherePos2.y;
	LenY *= LenY;

	//奥行の幅を2乗
	float LenZ = _SpherePos1.z - _SpherePos2.z;
	LenZ *= LenZ;

	//最短距離の2乗を計算
	float Len = LenX + LenY + LenZ;

	//半径２個を足した値の2乗
	float TotalRadius = _Radius1 + _Radius2;
	TotalRadius *= TotalRadius;

	//最短距離の2乗 <= 半径を2個を足した値の2乗
	if (Len <= TotalRadius) {
		return true;
	}
	else {
		return false;
	}
}
