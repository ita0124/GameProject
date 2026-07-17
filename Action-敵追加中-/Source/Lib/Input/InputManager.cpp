#include "InputManager.h"
#include "InputKey.h"
#include "InputPad.h"

using namespace std;

unsigned int InputManager::m_NowBuf;
unsigned int InputManager::m_OldBuf;

vector<int>InputManager::m_Key;
vector<int>InputManager::m_Pad;

float InputManager::m_LeftStickX;			//左スティックX
float InputManager::m_LeftStickY;			//左スティックY
float InputManager::m_RightStickX;			//右スティックX
float InputManager::m_RightStickY;			//右スティックY

constexpr int KEY_INPUT[InputManager::INPUT_MAX] = {
	KEY_INPUT_R,					//ローリング
	KEY_INPUT_Z,					//ジャンプ
	KEY_INPUT_F,					//ガード
	KEY_INPUT_Q,					//スキル攻撃
	KEY_INPUT_SPACE,				//通常攻撃
	KEY_INPUT_C,					//カメラチェンジ
};

constexpr int PAD_INPUT[InputManager::INPUT_MAX] = {
	XINPUT_BUTTON_X,				//ローリング
	XINPUT_BUTTON_A,				//ジャンプ
	XINPUT_BUTTON_RIGHT_SHOULDER,	//ガード
	XINPUT_BUTTON_Y,				//スキル攻撃
	XINPUT_BUTTON_B,				//通常攻撃
	XINPUT_BUTTON_RIGHT_THUMB,		//カメラチェンジ
};

//初期化
void InputManager::Init() {
	m_NowBuf = 0;
	m_OldBuf = 0;

	for (int Index = 0;Index < INPUT_MAX;Index++) {
		m_Key.push_back(KEY_INPUT[Index]);
		m_Pad.push_back(PAD_INPUT[Index]);
	}

	m_LeftStickX = 0.0f;				//左スティックX
	m_LeftStickY = 0.0f;				//左スティックY
	m_RightStickX = 0.0f;				//右スティックX
	m_RightStickY = 0.0f;				//右スティックY
}
//更新処理
void InputManager::Update() {
	InputKey::Update();
	InputPad::Update();
	// 前回のデータ更新
	m_OldBuf = m_NowBuf;
	// 一度データを0にする
	m_NowBuf = 0;
	//ローリング
	if (InputKey::IsPushKeyRep(KEY_INPUT_R) || InputPad::IsPushPadRep(XINPUT_BUTTON_X)) {
		m_NowBuf |= INPUT_ROLLING;
	}
	//ジャンプ
	if (InputKey::IsPushKeyRep(KEY_INPUT_Z) || InputPad::IsPushPadRep(XINPUT_BUTTON_A)) {
		m_NowBuf |= INPUT_JUMP;
	}
	//ガード
	if (InputKey::IsPushKeyRep(KEY_INPUT_F) || InputPad::IsPushPadRep(XINPUT_BUTTON_RIGHT_SHOULDER)) {
		m_NowBuf |= INPUT_GUARD;
	}
	//スキル攻撃
	if (InputKey::IsPushKeyRep(KEY_INPUT_Q) || InputPad::IsPushPadRep(XINPUT_BUTTON_Y)) {
		m_NowBuf |= INPUT_SKILL_ATTACK;
	}
	//通常攻撃
	if (InputKey::IsPushKeyRep(KEY_INPUT_SPACE) || InputPad::IsPushPadRep(XINPUT_BUTTON_B)) {
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
	/*if (InputPad::IsPushPadRep(_Button, _Num)) {
		return true;
	}
	else if (InputKey::IsPushKeyRep(_Button)) {
		return true;
	}
	else {
		return false;
	}*/
}

//トリガー入力情報取得
bool InputManager::IsPushTrg(unsigned char _Button, unsigned int _Num) {
	if ((m_NowBuf & _Button) && !(m_OldBuf & _Button)) {
		return true;
	}
	else {
		return false;
	}
	/*if (InputPad::IsPushPadTrg(_Button, _Num)) {
		return true;
	}
	else if (InputKey::IsPushKeyTrg(_Button)) {
		return true;
	}
	else {
		return false;
	}*/
}

//離したかの情報取得
bool InputManager::IsPushLet(unsigned char _Button, unsigned int _Num) {
	if (!(m_NowBuf & _Button) && (m_OldBuf & _Button)) {
		return true;
	}
	else {
		return false;
	}
	/*if (InputPad::IsPushPadLet(_Button, _Num)) {
		return true;
	}
	else if (InputKey::IsPushKeyLet(_Button)) {
		return true;
	}
	else {
		return false;
	}*/
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
	else {
		m_RightStickY = 0.0f;
	}
	return m_RightStickY;
}
