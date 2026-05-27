#include "MyMath.h"

VECTOR VecCreate(VECTOR _Pos1, VECTOR _Pos2)
{//ベクトル生成
	VECTOR Result;
	Result.x = _Pos2.x - _Pos1.x;
	Result.y = _Pos2.y - _Pos1.y;
	Result.z = _Pos2.z - _Pos1.z;
	return Result;
}

float VecLong(VECTOR _Pos1, VECTOR _Pos2)
{//ベクトルの長さを返す
	float Result;
	Result = sqrtf((_Pos1.x - _Pos2.x) * (_Pos1.x - _Pos2.x) + (_Pos1.y - _Pos2.y) * (_Pos1.y - _Pos2.y) + (_Pos1.z - _Pos2.z) * (_Pos1.z - _Pos2.z));
	return Result;
}

VECTOR VecAdd(VECTOR _Vec1, VECTOR _Vec2)
{//ベクトルの足し算
	VECTOR Result;
	Result.x = _Vec1.x + _Vec2.x;
	Result.y = _Vec1.y + _Vec2.y;
	Result.z = _Vec1.z + _Vec2.z;

	return Result;
}

VECTOR VecSubtract(VECTOR _Vec1, VECTOR _Vec2)
{//ベクトルの引き算
	VECTOR Result;
	Result.x = _Vec1.x - _Vec2.x;
	Result.y = _Vec1.y - _Vec2.y;
	Result.z = _Vec1.z - _Vec2.z;

	return Result;

}

VECTOR VecScale(VECTOR _Vec, float _Scale)
{//ベクトルのスカラー倍
	VECTOR Result;
	Result.x = _Vec.x * _Scale;
	Result.y = _Vec.y * _Scale;
	Result.z = _Vec.z * _Scale;

	return Result;
}

VECTOR VecMultiplication(VECTOR _Vec1, VECTOR _Vec2) {
	VECTOR Result;
	Result.x = _Vec1.x * _Vec2.x;
	Result.y = _Vec1.y * _Vec2.y;
	Result.z = _Vec1.z * _Vec2.z;

	return Result;
}

float VecDot(VECTOR _Vec1, VECTOR _Vec2)
{//ベクトルの内積
	float Result;
	Result = _Vec1.x * _Vec2.x + _Vec1.y * _Vec2.y + _Vec1.z * _Vec2.z;
	return Result;
}

VECTOR VecCross(VECTOR _Vec1, VECTOR _Vec2)
{//ベクトルの外積
	VECTOR Result;
	Result.x = _Vec1.y * _Vec2.z - _Vec1.z * _Vec2.y;
	Result.y = _Vec1.z * _Vec2.x - _Vec1.x * _Vec2.z;
	Result.z = _Vec1.x * _Vec2.y - _Vec1.y * _Vec2.x;
	return Result;
}

VECTOR VecNormalize(VECTOR _Vec)
{//ベクトルの正規化
	float size;
	VECTOR Result;
	size = _Vec.x * _Vec.x + _Vec.y * _Vec.y + _Vec.z * _Vec.z;
	size = sqrtf(size);
	Result.x = _Vec.x / size;
	Result.y = _Vec.y / size;
	Result.z = _Vec.z / size;

	return Result;
}

MATRIX GetIdentityMatrix()
{//単位行列を取得する
	MATRIX Result = { 0 };
	//行
	for (int Line = 0; Line < 4; Line++) {
		//列
		for (int Column = 0; Column < 4; Column++) {
			if (Line == Column)
			{
				Result.m[Line][Column] = 1;
			}
			else
			{
				Result.m[Line][Column] = 0;
			}
		}
	}
	return Result;
}

MATRIX GetTranslateMatrix(VECTOR _Translate)
{//平行移動行列を取得
	MATRIX Result = GetIdentityMatrix();
	Result.m[0][3] = _Translate.x;
	Result.m[1][3] = _Translate.y;
	Result.m[2][3] = _Translate.z;

	return Result;
}

MATRIX GetTranslateMatrix(float _TranslateX, float _TranslateY, float _TranslateZ)
{//平行移動行列を取得
	MATRIX Result = GetIdentityMatrix();
	Result.m[0][3] = _TranslateX;
	Result.m[1][3] = _TranslateY;
	Result.m[2][3] = _TranslateZ;

	return Result;
}

MATRIX GetScaleMatrix(VECTOR _Scale)
{//拡縮行列を取得する
	MATRIX Result = GetIdentityMatrix();
	Result.m[0][0] = _Scale.x;
	Result.m[1][1] = _Scale.y;
	Result.m[2][2] = _Scale.z;

	return Result;
}

MATRIX GetScaleMatrix(float _ScaleX, float _ScaleY, float _ScaleZ)
{//拡縮行列を取得する
	MATRIX Result = GetIdentityMatrix();
	Result.m[0][0] = _ScaleX;
	Result.m[1][1] = _ScaleY;
	Result.m[2][2] = _ScaleZ;

	return Result;
}

MATRIX GetPitchMatrix(float _Pitch)
{//X軸回転行列を取得する
	MATRIX Result = GetIdentityMatrix();
	Result.m[1][1] = cosf(_Pitch);
	Result.m[1][2] = -sinf(_Pitch);
	Result.m[2][1] = sinf(_Pitch);
	Result.m[2][2] = cosf(_Pitch);

	return Result;
}

MATRIX GetYawMatrix(float _Yaw)
{//Y軸回転行列を取得する
	MATRIX Result = GetIdentityMatrix();
	Result.m[0][0] = cosf(_Yaw);
	Result.m[0][2] = sinf(_Yaw);
	Result.m[2][0] = -sinf(_Yaw);
	Result.m[2][2] = cosf(_Yaw);

	return Result;
}

MATRIX GetRoolMatrix(float _Rool)
{//Z軸回転行列を取得する
	MATRIX Result = GetIdentityMatrix();
	Result.m[0][0] = cosf(_Rool);
	Result.m[0][1] = -sinf(_Rool);
	Result.m[1][0] = sinf(_Rool);
	Result.m[1][1] = cosf(_Rool);

	return Result;
}

MATRIX MatAdd(MATRIX _Mat1, MATRIX _Mat2)
{//引数で渡された二つの行列を足し算する
	MATRIX Result = { 0 };
	//行
	for (int Line = 0; Line < 4; Line++) {
		//列
		for (int Column = 0; Column < 4; Column++) {
			Result.m[Line][Column] = _Mat1.m[Line][Column] + _Mat2.m[Line][Column];
		}
	}

	return Result;
}

MATRIX MatSub(MATRIX _Mat1, MATRIX _Mat2)
{//引数で渡された二つの行列を引き算する
	MATRIX Result = { 0 };
	//行
	for (int Line = 0; Line < 4; Line++) {
		//列
		for (int Column = 0; Column < 4; Column++) {
			Result.m[Line][Column] = _Mat1.m[Line][Column] - _Mat2.m[Line][Column];
		}
	}

	return Result;
}

MATRIX MatScale(MATRIX _Mat, float _Scale)
{//引数で渡された行列をスカラー倍する
	MATRIX Result = { 0 };
	//行
	for (int Line = 0; Line < 4; Line++) {
		//列
		for (int Column = 0; Column < 4; Column++) {
			Result.m[Line][Column] = _Mat.m[Line][Column] * _Scale;
		}
	}

	return Result;
}

MATRIX MatMult(MATRIX _Mat1, MATRIX _Mat2)
{//引数で渡された二つの行列の掛け算を行う
	MATRIX Result = { 0 };
	//行
	for (int Line = 0; Line < 4; Line++) {
		//列
		for (int Column = 0; Column < 4; Column++) {
			for (int Multi = 0; Multi < 4; Multi++){
				Result.m[Line][Column] += (_Mat1.m[Line][Multi] * _Mat2.m[Multi][Column]);
			}
		}
	}

	return Result;
}

VECTOR MatTransform(MATRIX _Mat, VECTOR _Vec)
{//行列×ベクトルを行う(変換計算)
	float Work[4] = { 0 };
	float Result_Buf[4] = { 0 };
	VECTOR Result_Vec;
	Work[0] = _Vec.x;
	Work[1] = _Vec.y;
	Work[2] = _Vec.z;
	Work[3] = 1;
	//行
	for (int Line = 0; Line < 4; Line++) {
		for (int Multi = 0; Multi < 4; Multi++) {
			Result_Buf[Line] += (_Mat.m[Line][Multi] * Work[Multi]);
		}
	}

	Result_Vec.x = Result_Buf[0];
	Result_Vec.y = Result_Buf[1];
	Result_Vec.z = Result_Buf[2];

	return Result_Vec;
}

MATRIX MatTranspose(MATRIX _Mat)
{//転置されたい行列
	MATRIX Result = { 0 };
	//行
	for (int Line = 0; Line < 4; Line++) {
		//列
		for (int Column = 0; Column < 4; Column++) {
			Result.m[Column][Line] = _Mat.m[Line][Column];
		}
	}

	return Result;
}
