#pragma once
#include "Lib/DxLibFont/DxLibFont.h"
#include <DxLib.h>
#include <iostream>
#include <vector>
#include <list>
#include <math.h>

using namespace std;

/*===================================================================================================================*/
//スクリーン関連
static const float SCREEN_MAX_X = 1600;							//スクリーンのX最大値
static const float SCREEN_MAX_Y = 800;							//スクリーンのY最大値

static const float SCREEN_HALF_X = SCREEN_MAX_X / 2;			//スクリーンのX半分
static const float SCREEN_HALF_Y = SCREEN_MAX_Y / 2;			//スクリーンのY半分

static const float SCRREN_LEFT = SCREEN_HALF_X / 2;				//スクリーンの左部半分
static const float SCREEN_RIGHT = SCRREN_LEFT + SCREEN_HALF_X;	//スクリーンの右部半分

static const float SCREEN_UPPER = SCREEN_HALF_Y / 2;			//スクリーンの上部半分
static const float SCREEN_LOWER = SCREEN_UPPER + SCREEN_HALF_Y;	//スクリーンの下部半分
/*===================================================================================================================*/
//色
static const int WHITE = GetColor(255, 255, 255);				//白

static const int GRAY = GetColor(127, 127, 127);				//灰色

static const int BLACK = GetColor(0, 0, 0);						//黒

static const int RED = GetColor(255, 50, 50);					//赤

static const int YELLOW = GetColor(255, 255, 50);				//黄

static const int PURPLR = GetColor(255, 50, 255);				//紫

static const int GREEN = GetColor(50, 255, 50);					//緑

static const int BLUUE = GetColor(50, 50, 255);					//青

static const int SKYBLUE = GetColor(50, 255, 255);				//水色

static const int ORANGE = GetColor(255, 127, 50);				//橙色
/*===================================================================================================================*/
//初期化
static const VECTOR VZERO = { 0.0f,0.0f,0.0f };					//VECTOR型を全て0に

static const VECTOR VONE = { 1.0f,1.0f,1.0f };					//VECTOR型を全て1に(スケールの初期化に使う)

static const float FZERO = 0.0f;								//float型を0に

static const int HNDL = -1;										//ハンドルの初期化
/*===================================================================================================================*/
//その他
static const int DIV = 16;
