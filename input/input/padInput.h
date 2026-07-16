#pragma once

enum tagPadButton
{
	BUTTON_DOWN,		//下ボタン
	BUTTON_LEFT,		//左ボタン
	BUTTON_RIGHT,		//右ボタン
	BUTTON_UP,			//上ボタン
	BUTTON_A,			//Aボタン
	BUTTON_B,			//Bボタン
	BUTTON_X,			//Xボタン
	BUTTON_Y,			//Yボタン
	BUTTON_LB,			//LBボタン
	BUTTON_RB,			//RBボタン
	BUTTON_BACK,		//BACKボタン
	BUTTON_START,		//STARTボタン
	BUTTON_LSTICK,		//Lスティック押し込み
	BUTTON_RSTICK,		//Rスティック押し込み

	BUTTON_NUM,			//ボタンの数
};

class CPadInput
{
private:
	static unsigned int m_nowButton;		//現在のボタン情報
	static unsigned int m_beforeButton;		//1フレーム前のボタン
	static int			m_LX;				//左レバーを左右の入力情報
	static int			m_LY;				//左レバーを上下の入力情報
	static int			m_RX;				//右レバーを左右の入力情報
	static int			m_RY;				//右レバーを上下の入力情報
public:
	//コンストラクタ
	CPadInput();

	//コントローラー入力初期化
	static void Init();
	//コントローラー入力情報更新
	static void Update();
	//ボタン入力判定（通常判定）
	static bool IsRep(tagPadButton _button);
	//ボタン入力判定（トリガー判定）
	static bool IsTrg(tagPadButton _button);

	//レバーの値は1～0～(-1)の間で出力する
	//左レバーの左右の入力情報を取得
	static float GetLX() { return static_cast<float>(m_LX) / 1000.0f; }
	//左レバーの上下の入力情報を取得
	static float GetLY() { return static_cast<float>(m_LY) / 1000.0f; }
	//右レバーの左右の入力情報を取得
	static float GetRX() { return static_cast<float>(m_RX) / 1000.0f; }
	//右レバーの上下の入力情報を取得
	static float GetRY() { return static_cast<float>(m_RY) / 1000.0f; }

};

