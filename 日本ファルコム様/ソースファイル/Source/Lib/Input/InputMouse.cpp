#include "InputMouse.h"

int InputMouse::m_NowMouse;	//今フレームのマウス情報
int InputMouse::m_OldMouse;	//前フレームのマウス情報

int InputMouse::m_PosX;		//X軸情報
int InputMouse::m_PosY;		//Y軸情報
//コンストラクタ
InputMouse::InputMouse() {
	Init();
}

//初期化
void InputMouse::Init() {
	// マウスを非表示状態にする
	SetMouseDispFlag(FALSE);
	// マウスの位置をセット
	SetMousePoint((int)SCREEN_HALF_X, (int)SCREEN_HALF_Y);

	m_NowMouse = 0;					//今フレームのマウス情報
	m_OldMouse = 0;					//前フレームのマウス情報

	m_PosX = (int)SCREEN_HALF_X;	//X軸情報
	m_PosY = (int)SCREEN_HALF_Y;	//Y軸情報
}

//マウス入力情報を更新
void InputMouse::Update() {
	//前フレームボタン情報に今フレームボタン情報を入れる
	m_OldMouse = m_NowMouse;
	//現在のマウス入力情報を取得
	m_NowMouse = GetMouseInput();
	//マウスの位置を更新
	GetMousePoint(&m_PosX, &m_PosY);
	// マウスの位置をセット
	SetMousePoint((int)SCREEN_HALF_X, (int)SCREEN_HALF_Y);
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
	int Pos = (int)SCREEN_HALF_X - m_PosX;
	return Pos;
}

//マウスのY軸移動
int InputMouse::GetPosY() {
	int Pos = (int)SCREEN_HALF_Y - m_PosY;
	return Pos;
}
