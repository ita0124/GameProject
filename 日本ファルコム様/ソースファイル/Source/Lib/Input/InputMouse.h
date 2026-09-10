#pragma once
#pragma once
#include "Comon.h"

//マウスボード入力クラス
class InputMouse {
protected:
	static int m_NowMouse;	//今フレームのマウス情報
	static int m_OldMouse;	//前フレームのマウス情報

	static int m_PosX;		//X軸情報
	static int m_PosY;		//Y軸情報
public:
	//コンストラクタ
	InputMouse();

	//初期化
	static void Init();

	//マウス入力情報を更新
	static void Update();

	//マウスのボタンリピート入力情報取得
	//_Mouse		:チェックするマウス
	//@return	:マウスを入力したか
	static bool IsPushMouseRep(int _Mouse);

	//マウスのボタントリガー入力情報取得
	//_Mouse		:チェックするマウス
	//@return	:マウスを一度だけ入力したか
	static bool	IsPushMouseTrg(int _Mouse);

	//マウスのボタンを離したかの情報取得
	//_Mouse		:チェックするマウス
	//@return	:マウスを離したか
	static bool IsPushMouseLet(int _Mouse);

	//マウスのX軸移動
	//@return	:マウスをX軸方向にどれだけ動かしたか
	static int GetPosX();

	//マウスのY軸移動
	//@return	:マウスをY軸方向にどれだけ動かしたか
	static int GetPosY();
};

/*
MOUSE_INPUT_LEFT　　： マウス左ボタン
MOUSE_INPUT_RIGHT　 ： マウス右ボタン
MOUSE_INPUT_MIDDLE　： マウス中央ボタン
*/