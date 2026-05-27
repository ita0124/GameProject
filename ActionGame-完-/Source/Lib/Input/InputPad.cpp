#include "InputPad.h"

XINPUT_STATE InputPad::m_NowInput[USE_PAD_NUM];	//現フレームの入力情報

XINPUT_STATE InputPad::m_OldInput[USE_PAD_NUM];	//現フレームの入力情報

//パッド入力情報を更新
void InputPad::Update() {
	for (int i = 0; i < USE_PAD_NUM; i++) {
		//前フレームボタン情報に今フレームボタン情報を入れる
		m_OldInput[i] = m_NowInput[i];
	}

	for (int i = 0; i < USE_PAD_NUM; i++) {
		switch (i) {
		case 0:
			GetJoypadXInputState(DX_INPUT_PAD1, &m_NowInput[0]);
			break;
			//今回は使わない
			/*
		case 1:
			GetJoypadXInputState(DX_INPUT_PAD2, &m_NowInput[1]);
			break;
		case 2:
			GetJoypadXInputState(DX_INPUT_PAD3, &m_NowInput[2]);
			break;
		case 3:
			GetJoypadXInputState(DX_INPUT_PAD4, &m_NowInput[3]);
			break;
			*/
		}
	}
}

//キーリピート入力情報取得
bool InputPad::IsPushPadRep(unsigned char _Button, unsigned int _Num) {
	/*return m_NowInput[_Num].Buttons[_Button] == 1;*/

	if (m_NowInput[_Num].Buttons[_Button] == 1) {
		return true;
	}
	else {
		return false;
	}
}

//キートリガー入力情報取得
bool InputPad::IsPushPadTrg(unsigned char _Button, unsigned int _Num) {
	if (m_NowInput[_Num].Buttons[_Button] == 1 && m_OldInput[_Num].Buttons[_Button] != 1) {
		return true;
	}
	else {
		return false;
	}
}

//キーを離したかの情報取得
bool InputPad::IsPushPadLet(unsigned char _Button, unsigned int _Num) {
	if (m_NowInput[_Num].Buttons[_Button] != 1 && m_OldInput[_Num].Buttons[_Button] == 1) {
		return true;
	}
	else {
		return false;
	}
	/*return (m_NowInput[_Num].Buttons[_Button] != 1 && m_OldInput[_Num].Buttons[_Button] == 1);*/
}

//左アナログスティックのX軸の入力
float InputPad::GetLAnalogXInput(unsigned int _Num) {
	if (m_NowInput[_Num].ThumbLX == 0)return 0.0f;

	float Res = (float)m_NowInput[_Num].ThumbLX / (float)THUMB_MAX;

	if (Res < ANALOG_MIN && Res>0.0f) {
		Res = 0.0f;
	}
	else if (Res > -ANALOG_MIN && Res < 0.0f) {
		Res = 0.0f;
	}

	return Res;
}

//左アナログスティックのY軸の入力
float InputPad::GetLAnalogYInput(unsigned int _Num) {
	if (m_NowInput[_Num].ThumbLY == 0)return 0.0f;

	float Res = (float)m_NowInput[_Num].ThumbLY / (float)THUMB_MAX;

	if (Res < ANALOG_MIN && Res>0.0f) {
		Res = 0.0f;
	}
	else if (Res > -ANALOG_MIN && Res < 0.0f) {
		Res = 0.0f;
	}

	return Res;
}

//右アナログスティックのX軸の入力
float InputPad::GetRAnalogXInput(unsigned int _Num) {
	if (m_NowInput[_Num].ThumbRX == 0)return 0.0f;

	float Res = (float)m_NowInput[_Num].ThumbRX / (float)THUMB_MAX;

	if (Res < ANALOG_MIN && Res>0.0f) {
		Res = 0.0f;
	}
	else if (Res > -ANALOG_MIN && Res < 0.0f) {
		Res = 0.0f;
	}

	return Res;
}

//右アナログスティックのY軸の入力
float InputPad::GetRAnalogYInput(unsigned int _Num) {
	if (m_NowInput[_Num].ThumbRY == 0)return 0.0f;

	float Res = (float)m_NowInput[_Num].ThumbRY / (float)THUMB_MAX;

	if (Res < ANALOG_MIN && Res>0.0f) {
		Res = 0.0f;
	}
	else if (Res > -ANALOG_MIN && Res < 0.0f) {
		Res = 0.0f;
	}

	return Res;
}

//左トリガーの入力(リピート)
bool InputPad::GetLTriggerInputRep(unsigned int _Num) {
	if (((float)m_NowInput[_Num].LeftTrigger / (float)TRIGGER_MAX) > 0) {
		return true;
	}
	else {
		return false;
	}
}

//左トリガーの入力(トリガー)
bool InputPad::GetLTriggerInputTrg(unsigned int _Num) {
	if (((float)m_NowInput[_Num].LeftTrigger / (float)TRIGGER_MAX) > 0 && ((float)m_OldInput[_Num].LeftTrigger / (float)TRIGGER_MAX) == 0) {
		float f = ((float)m_NowInput[_Num].LeftTrigger / (float)TRIGGER_MAX);
		return true;
	}
	else {
		return false;
	}
}

//右トリガーの入力(リピート)
bool InputPad::GetRTriggerInputRep(unsigned int _Num) {
	if (((float)m_NowInput[_Num].RightTrigger / (float)TRIGGER_MAX) > 0) {
		return true;
	}
	else {
		return false;
	}
}

//右トリガーの入力(トリガー)
bool InputPad::GetRTriggerInputTrg(unsigned int _Num) {
	if (((float)m_NowInput[_Num].LeftTrigger / (float)TRIGGER_MAX) > 0 && ((float)m_OldInput[_Num].RightTrigger / (float)TRIGGER_MAX) == 0.0f) {
		return true;
	}
	else {
		return false;
	}
}
