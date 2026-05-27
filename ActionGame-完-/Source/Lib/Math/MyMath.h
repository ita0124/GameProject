#ifndef _MYMATH_H_
#define _MYMATH_H_

#include "DxLib.h"
#include <math.h>

//ベクトルや行列の計算処理をまとめたクラス
VECTOR VecCreate(VECTOR _Pos1, VECTOR _Pos2);//ベクトル作成
float VecLong(VECTOR _Pos1, VECTOR _Pos2);//ベクトルの長さ
VECTOR VecAdd(VECTOR _Vec1, VECTOR vec2);//ベクトルの足し算
VECTOR VecSubtract(VECTOR _Vec1, VECTOR _Vec2);//ベクトルの引き算
VECTOR VecScale(VECTOR _Vec, float _Scale);//ベクトルのスカラー倍
VECTOR VecMultiplication(VECTOR _Vec1,VECTOR _Vec2);//ベクトル同士の掛け算
float VecDot(VECTOR _Vec1, VECTOR _Vec2);//ベクトルの内積
VECTOR VecCross(VECTOR _Vec1, VECTOR _Vec2);//ベクトルの外積
VECTOR VecNormalize(VECTOR _Vec);//ベクトルの正規化

MATRIX GetIdentityMatrix();//単位行列を取得する
MATRIX GetTranslateMatrix(VECTOR _Translate);//平行移動行列を取得
MATRIX GetTranslateMatrix(float _TranslateX,float _TranslateY, float _TranslateZ);//平行移動行列を取得
MATRIX GetScaleMatrix(VECTOR _Scale);//拡縮行列を取得する
MATRIX GetScaleMatrix(float _ScaleX, float _ScaleY, float _ScaleZ);//拡縮行列を取得する
MATRIX GetPitchMatrix(float _Pitch);//X軸回転行列を取得する
MATRIX GetYawMatrix(float _Yaw);//Y軸回転行列を取得する
MATRIX GetRoolMatrix(float _Rool);//Z軸回転行列を取得する
MATRIX MatAdd(MATRIX _Mat1, MATRIX _Mat2);//引数で渡された二つの行列を足し算する
MATRIX MatSub(MATRIX _Mat1, MATRIX _Mat2);//引数で渡された二つの行列を引き算する
MATRIX MatScale(MATRIX _Mat,float _Scale);//引数で渡された行列をスカラー倍する
MATRIX MatMult(MATRIX _Mat1, MATRIX _Mat2);//引数で渡された二つの行列の掛け算を行う
VECTOR MatTransform(MATRIX _Mat, VECTOR _Vec);//行列×ベクトルを行う(変換計算)
MATRIX MatTranspose(MATRIX _Mat);//転置されたい行列


#endif