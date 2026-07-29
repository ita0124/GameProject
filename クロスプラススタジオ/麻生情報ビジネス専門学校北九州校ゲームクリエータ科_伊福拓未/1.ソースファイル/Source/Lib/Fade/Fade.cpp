#include "Fade.h"

Fade::TagFade	Fade::m_State;
int				Fade::m_Count;
bool			Fade::m_Flg;
int				Fade::m_Speed;
int				Fade::m_Color;

Fade::Fade() {
	Init();
}

void Fade::Init() {
	m_State = FADE_NON;
	m_Count = 0;
	m_Flg = false;
}

void Fade::Step() {
	switch (m_State) {
	case FADE_IN:
		//徐々に数字を減らし明るくする
		m_Count -= m_Speed;
		if (m_Count <= 0) {
			m_Count = 0;
			m_State = FADE_NON;
		}
		break;

	case FADE_OUT:
		//徐々に数字を増やし暗くする
		m_Count += m_Speed;
		if (m_Count >= 255) {
			m_Count = 255;
			m_State = FADE_OUT_WAIT;
		}
		break;
	}
}

void Fade::Draw() {
	switch (m_State) {
	case FADE_IN:
	case FADE_OUT:
	case FADE_OUT_WAIT:
		//まずはここでアルファ値をセット
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_Count);

		//フェード用の黒い四角
		DrawBox(0, 0, (int)SCREEN_MAX_X, (int)SCREEN_MAX_Y, m_Color, TRUE);

		//鉾の画像に影響を出さないよう、初期化設定に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		break;
	}
}

void Fade::RequestIn(int _Speed, int _Color) {
	if (m_Flg) return;

	m_State = FADE_IN;
	m_Count = 255;
	m_Flg = true;
	m_Speed = _Speed;
	m_Color = _Color;
}

void Fade::RequestOut(int _Speed, int _Color) {
	if (m_Flg)return;

	m_State = FADE_OUT;
	m_Count = 0;
	m_Flg = true;
	m_Speed = _Speed;
	m_Color = _Color;
}

bool Fade::IsEndIn() {
	if (m_State != FADE_IN) {
		m_Flg = false;
		return true;
	}
	else {
		return false;
	}
}

bool Fade::IsEndOut() {
	if (m_State != FADE_OUT) {
		m_Flg = false;
		return true;
	}
	else {
		return false;
	}
}
