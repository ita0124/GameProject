#include "InputManager.h"
#include "InputKey.h"
#include "InputPad.h"
#include "InputMouse.h"

namespace {
	//マウス移動量からアナログ入力へ変換する倍率
	constexpr float MOUSE_ANALOG_SENSITIVITY = 0.01f;
}

using namespace std;

unsigned int InputManager::m_NowBuf;
unsigned int InputManager::m_OldBuf;

float InputManager::m_LeftStickX;			//左スティックX
float InputManager::m_LeftStickY;			//左スティックY
float InputManager::m_RightStickX;			//右スティックX
float InputManager::m_RightStickY;			//右スティックY

//初期化
void InputManager::Init() {
	m_NowBuf = 0;
	m_OldBuf = 0;

	m_LeftStickX = 0.0f;				//左スティックX
	m_LeftStickY = 0.0f;				//左スティックY
	m_RightStickX = 0.0f;				//右スティックX
	m_RightStickY = 0.0f;				//右スティックY

	InputKey::Init();
	InputMouse::Init();
}
//更新処理
void InputManager::Update() {
	InputKey::Update();
	InputPad::Update();
	InputMouse::Update();
	// 前回のデータ更新
	m_OldBuf = m_NowBuf;
	// 一度データを0にする
	m_NowBuf = 0;
	//ローリング
	if (InputKey::IsPushKeyRep(KEY_INPUT_R) || InputPad::IsPushPadRep(XINPUT_BUTTON_X)) {
		m_NowBuf |= INPUT_ROLLING;
	}
	//ジャンプ
	if (InputKey::IsPushKeyRep(KEY_INPUT_SPACE) || InputPad::IsPushPadRep(XINPUT_BUTTON_A)) {
		m_NowBuf |= INPUT_JUMP;
	}
	//ガード
	if (InputKey::IsPushKeyRep(KEY_INPUT_L) || InputPad::IsPushPadRep(XINPUT_BUTTON_RIGHT_SHOULDER) || InputMouse::IsPushMouseRep(MOUSE_INPUT_RIGHT)) {
		m_NowBuf |= INPUT_GUARD;
	}
	//スキル攻撃
	if (InputKey::IsPushKeyRep(KEY_INPUT_K) || InputPad::IsPushPadRep(XINPUT_BUTTON_Y) || InputMouse::IsPushMouseRep(MOUSE_INPUT_MIDDLE)) {
		m_NowBuf |= INPUT_SKILL_ATTACK;
	}
	//通常攻撃
	if (InputKey::IsPushKeyRep(KEY_INPUT_J) || InputPad::IsPushPadRep(XINPUT_BUTTON_B) || InputMouse::IsPushMouseRep(MOUSE_INPUT_LEFT)) {
		m_NowBuf |= INPUT_NORMAL_ATTACK;
	}
	//カメラチェンジ
	if (InputKey::IsPushKeyRep(KEY_INPUT_C) || InputPad::IsPushPadRep(XINPUT_BUTTON_RIGHT_THUMB)) {
		m_NowBuf |= INPUT_CAMERA_CHANGE;
	}
}
//リピート入力情報取得
bool InputManager::IsPushRep(unsigned char _Button, unsigned int _Num) {
	if (m_NowBuf & _Button) {
		return true;
	}
	else {
		return false;
	}
}

//トリガー入力情報取得
bool InputManager::IsPushTrg(unsigned char _Button, unsigned int _Num) {
	if ((m_NowBuf & _Button) && !(m_OldBuf & _Button)) {
		return true;
	}
	else {
		return false;
	}
}

//離したかの情報取得
bool InputManager::IsPushLet(unsigned char _Button, unsigned int _Num) {
	if (!(m_NowBuf & _Button) && (m_OldBuf & _Button)) {
		return true;
	}
	else {
		return false;
	}
}

//左アナログスティックのX軸の入力
float InputManager::GetLAnalogXInput(unsigned int _Num) {
	if (InputPad::GetLAnalogXInput(_Num) != 0.0f) {
		m_LeftStickX = InputPad::GetLAnalogXInput();
	}
	else if (InputKey::IsPushKeyRep(KEY_INPUT_A)) {
		m_LeftStickX = -1.0f;
	}
	else if (InputKey::IsPushKeyRep(KEY_INPUT_D)) {
		m_LeftStickX = 1.0f;
	}
	else {
		m_LeftStickX = 0.0f;
	}
	return m_LeftStickX;
}

//左アナログスティックのY軸の入力
float InputManager::GetLAnalogYInput(unsigned int _Num) {
	if (InputPad::GetLAnalogYInput(_Num) != 0.0f) {
		m_LeftStickY = InputPad::GetLAnalogYInput();
	}
	else if (InputKey::IsPushKeyRep(KEY_INPUT_S)) {
		m_LeftStickY = -1.0f;
	}
	else if (InputKey::IsPushKeyRep(KEY_INPUT_W)) {
		m_LeftStickY = 1.0f;
	}
	else {
		m_LeftStickY = 0.0f;
	}
	return m_LeftStickY;
}

//右アナログスティックのX軸の入力
float InputManager::GetRAnalogXInput(unsigned int _Num) {
	if (InputPad::GetRAnalogXInput(_Num) != 0.0f) {
		m_RightStickX = InputPad::GetRAnalogXInput();
	}
	else if (InputKey::IsPushKeyRep(KEY_INPUT_RIGHT)) {
		m_RightStickX = 1.0f;
	}
	else if (InputKey::IsPushKeyRep(KEY_INPUT_LEFT)) {
		m_RightStickX = -1.0f;
	}
	else if (InputMouse::GetPosX() != 0) {
		m_RightStickX = -((float)InputMouse::GetPosX() * MOUSE_ANALOG_SENSITIVITY);
	}
	else {
		m_RightStickX = 0.0f;
	}
	return m_RightStickX;
}

//右アナログスティックのY軸の入力
float InputManager::GetRAnalogYInput(unsigned int _Num) {
	if (InputPad::GetRAnalogYInput(_Num) != 0.0f) {
		m_RightStickY = InputPad::GetRAnalogYInput();
	}
	else if (InputKey::IsPushKeyRep(KEY_INPUT_DOWN)) {
		m_RightStickY = 1.0f;
	}
	else if (InputKey::IsPushKeyRep(KEY_INPUT_UP)) {
		m_RightStickY = -1.0f;
	}
	else if (InputMouse::GetPosY() != 0) {
		m_RightStickY = (float)InputMouse::GetPosY() * MOUSE_ANALOG_SENSITIVITY;
	}
	else {
		m_RightStickY = 0.0f;
	}
	return m_RightStickY;
}
