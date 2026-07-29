#pragma once
#include <DxLib.h>
#include <vector>
#include <iterator>

class InputManager {
public:
	enum TagInput {
		INPUT_ROLLING =			0b01,		//ローリング
		INPUT_JUMP =			0b10,		//ジャンプ
		INPUT_GUARD =			0b100,		//ガード
		INPUT_SKILL_ATTACK =	0b1000,		//スキル攻撃
		INPUT_NORMAL_ATTACK =	0b10000,	//通常攻撃
		INPUT_CAMERA_CHANGE =	0b100000,	//カメラチェンジ

		INPUT_MAX
	};

private:
	static unsigned int m_NowBuf;
	static unsigned int m_OldBuf;

	static float m_LeftStickX;			//左スティックX
	static float m_LeftStickY;			//左スティックY
	static float m_RightStickX;			//右スティックX
	static float m_RightStickY;			//右スティックY
public:
	//初期化
	static void Init();
	//更新処理
	static void Update();

	//リピート入力情報取得
	//_Button	:チェックするボタン
	//_Num		:コントローラーのナンバー
	//@return	:ボタン押したか
	static bool IsPushRep(unsigned char _Button, unsigned int _Num = 0);

	//トリガー入力情報取得
	//_Button	:チェックするボタン
	//_Num		:コントローラーのナンバー
	//@return	:ボタンを一度だけ押したか
	static bool	IsPushTrg(unsigned char _Button, unsigned int _Num = 0);

	//離したかの情報取得
	//_Button	:チェックするボタン
	//_Num		:コントローラーのナンバー
	//@return	:ボタンを離したか
	static bool IsPushLet(unsigned char _Button, unsigned int _Num = 0);

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
};

