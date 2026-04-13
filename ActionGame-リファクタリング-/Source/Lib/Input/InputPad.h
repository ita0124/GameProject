#pragma once
#include "Comon.h"

static const int USE_PAD_NUM = 1;				//使用するコントローラーの数

static const short THUMB_MAX = 32767;		//アナログスティックの最大強度

static const float TRIGGER_MAX = 255.0f;	//トリガーの最大強度

static const float ANALOG_MIN = 0.05f;

/*
XInputボタン入力定義
XINPUT_BUTTON_DPAD_UP			// デジタル方向ボタン上
XINPUT_BUTTON_DPAD_DOWN			// デジタル方向ボタン下
XINPUT_BUTTON_DPAD_LEFT			// デジタル方向ボタン左
XINPUT_BUTTON_DPAD_RIGHT		// デジタル方向ボタン右
XINPUT_BUTTON_START				// STARTボタン
XINPUT_BUTTON_BACK				// BACKボタン
XINPUT_BUTTON_LEFT_THUMB		// 左スティック押し込み
XINPUT_BUTTON_RIGHT_THUMB		// 右スティック押し込み
XINPUT_BUTTON_LEFT_SHOULDER		// LBボタン
XINPUT_BUTTON_RIGHT_SHOULDER	// RBボタン
XINPUT_BUTTON_A					// Aボタン
XINPUT_BUTTON_B					// Bボタン
XINPUT_BUTTON_X					// Xボタン
XINPUT_BUTTON_Y					// Yボタン
*/

//コントローラー入力クラス
class InputPad {
private:
	static XINPUT_STATE m_NowInput[USE_PAD_NUM];	//現フレームの入力情報

	static XINPUT_STATE m_OldInput[USE_PAD_NUM];	//１フレーム前の入力情報

public:
	//パッド入力情報を更新
	static void Update();

	//キーリピート入力情報取得
	//_Button	:チェックするボタン
	//_Num		:コントローラーのナンバー
	//@return	:ボタン押したか
	static bool IsPushPadRep(unsigned char _Button, unsigned int _Num = 0);

	//キートリガー入力情報取得
	//_Button	:チェックするボタン
	//_Num		:コントローラーのナンバー
	//@return	:ボタンを一度だけ押したか
	static bool	IsPushPadTrg(unsigned char _Button, unsigned int _Num = 0);

	//キーを離したかの情報取得
	//_Button	:チェックするボタン
	//_Num		:コントローラーのナンバー
	//@return	:ボタンを離したか
	static bool IsPushPadLet(unsigned char _Button, unsigned int _Num = 0);

	//左アナログスティックのX軸の入力
	//_Num		:コントローラーのナンバー
	//@return	:左アナログスティックをX軸方向にどれだけ倒したか
	static float GetLAnalogXInput(unsigned int _Num = 0);

	//左アナログスティックのY軸の入力
	//_Num		:コントローラーのナンバー
	//@return	:左アナログスティックをY軸方向にどれだけ倒したか
	static float GetLAnalogYInput(unsigned int _Num = 0);

	//右アナログスティックのX軸の入力
	//_Num		:コントローラーのナンバー
	//@return	:右アナログスティックをX軸方向にどれだけ倒したか
	static float GetRAnalogXInput(unsigned int _Num = 0);

	//右アナログスティックのY軸の入力
	//_Num		:コントローラーのナンバー
	//@return	:右アナログスティックをY軸方向にどれだけ倒したか
	static float GetRAnalogYInput(unsigned int _Num = 0);

	//左トリガーの入力(リピート)
	//_Num		:コントローラーのナンバー
	//@return	:左トリガーボタンを押したか
	static bool GetLTriggerInputRep(unsigned int _Num = 0);	

	//左トリガーの入力(トリガー)
	//_Num		:コントローラーのナンバー
	//@return	:左トリガーボタンを押したか
	static bool GetLTriggerInputTrg(unsigned int _Num = 0);

	//右トリガーの入力(リピート)
	//_Num		:コントローラーのナンバー
	//@return	:右トリガーボタンを押したか
	static bool GetRTriggerInputRep(unsigned int _Num = 0);

	//右トリガーの入力(トリガー)
	//_Num		:コントローラーのナンバー
	//@return	:左トリガーボタンを押したか
	static bool GetRTriggerInputTrg(unsigned int _Num = 0);
};
