#include "InputMouse.h"

int InputMouse::m_NowMouse;	//今フレームのマウス情報
int InputMouse::m_OldMouse;	//前フレームのマウス情報

int InputMouse::m_NowPosX;	//今フレームのX軸情報
int InputMouse::m_NowPosY;	//今フレームのY軸情報
int InputMouse::m_OldPosX;	//前フレームのX軸情報
int InputMouse::m_OldPosY;	//前フレームのY軸情報

//コンストラクタ
InputMouse::InputMouse() {
	Init();
}

//初期化
void InputMouse::Init() {
	// マウスを非表示状態にする
	SetMouseDispFlag(TRUE);
	// マウスの位置をセット
	SetMousePoint(SCREEN_HALF_X, SCREEN_HALF_Y);

	m_NowMouse = 0;	//今フレームのマウス情報
	m_OldMouse = 0;	//前フレームのマウス情報

	m_NowPosX = SCREEN_HALF_X;	//今フレームのX軸情報
	m_NowPosY = SCREEN_HALF_Y;	//今フレームのY軸情報
	m_OldPosX = SCREEN_HALF_X;	//前フレームのX軸情報
	m_OldPosY = SCREEN_HALF_Y;	//前フレームのY軸情報
}

//マウス入力情報を更新
void InputMouse::Update() {
	//前フレームボタン情報に今フレームボタン情報を入れる
	m_OldMouse = m_NowMouse;
	//前フレーム座標情報に今フレーム座標情報を入れる
	m_OldPosX = m_NowPosX;
	m_OldPosY = m_NowPosY;
	//現在のマウス入力情報を取得
	m_NowMouse = GetMouseInput();
	//マウスの位置を更新
	GetMousePoint(&m_NowPosX, &m_NowPosY);
}

//マウスのボタンリピート入力情報取得
bool InputMouse::IsPushMouseRep(int _Mouse) {
	if (m_NowMouse == _Mouse) {
		return true;
	}
	else {
		return false;
	}
}

//マウスのボタントリガー入力情報取得
bool InputMouse::IsPushMouseTrg(int _Mouse) {
	if (m_NowMouse == _Mouse && m_OldMouse != _Mouse) {
		return true;
	}
	else {
		return false;
	}
}

//マウスのボタンを離したかの情報取得
bool InputMouse::IsPushMouseLet(int _Mouse) {
	if (m_NowMouse != _Mouse && m_OldMouse == _Mouse) {
		return true;
	}
	else {
		return false;
	}
}

//マウスのX軸移動
int InputMouse::GetPosX() {
	int Pos = m_NowPosX - m_OldPosX;
	return Pos;
}

//マウスのY軸移動
int InputMouse::GetPosY() {
	int Pos = m_NowPosY - m_OldPosY;
	return Pos;
}
