#pragma once
#include "Lib/DxLibFont/DxLibFont.h"
#include <DxLib.h>
#include <math.h>

/*===================================================================================================================*/
//スクリーン関連
constexpr float	SCREEN_MAX_X = 1600;							//スクリーンのX最大値
constexpr float	SCREEN_MAX_Y = 800;								//スクリーンのY最大値

constexpr float	SCREEN_HALF_X = SCREEN_MAX_X / 2;				//スクリーンのX半分
constexpr float	SCREEN_HALF_Y = SCREEN_MAX_Y / 2;				//スクリーンのY半分

constexpr float	SCRREN_LEFT = SCREEN_HALF_X / 2;				//スクリーンの左部半分
constexpr float	SCREEN_RIGHT = SCRREN_LEFT + SCREEN_HALF_X;		//スクリーンの右部半分

constexpr float	SCREEN_UPPER = SCREEN_HALF_Y / 2;				//スクリーンの上部半分
constexpr float	SCREEN_LOWER = SCREEN_UPPER + SCREEN_HALF_Y;	//スクリーンの下部半分
/*===================================================================================================================*/
//色
const int	WHITE = GetColor(255, 255, 255);				//白

const int	GRAY = GetColor(127, 127, 127);					//灰色

const int	BLACK = GetColor(0, 0, 0);						//黒

const int	RED = GetColor(255, 50, 50);					//赤

const int	YELLOW = GetColor(255, 255, 50);				//黄

const int	PURPLR = GetColor(255, 50, 255);				//紫

const int	GREEN = GetColor(50, 255, 50);					//緑

const int	BLUUE = GetColor(50, 50, 255);					//青

const int	SKYBLUE = GetColor(50, 255, 255);				//水色

const int	ORANGE = GetColor(255, 127, 50);				//橙色
/*===================================================================================================================*/
//初期化
constexpr VECTOR	VZERO = { 0.0f,0.0f,0.0f };					//VECTOR型を全て0に

constexpr VECTOR VONE = { 1.0f,1.0f,1.0f };						//VECTOR型を全て1に(スケールの初期化に使う)
/*===================================================================================================================*/
//その他
constexpr int	DIV = 16;

constexpr float	RADIAN_CALC = DX_PI_F / 180.0f;					//360角度をラジアンに変換するときに使う
