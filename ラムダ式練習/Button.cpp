#pragma once
#include "Button.h"

Button::Button(){}
Button::~Button() {}

// 初期設定
void Button::Init(VECTOR _Pos, std::string _Name, unsigned int _Color, int _id) {
	m_Pos	= _Pos;
	m_Name	= _Name;
	m_Color	= _Color;
	m_id = _id;
}

// 描画
void Button::Draw() {
	DrawBox((int)m_Pos.x, (int)m_Pos.y, (int)m_Pos.x + BOX_WIDTH, (int)m_Pos.y + BOX_HEIGHT, m_Color, false);
	DrawFormatString((int)m_Pos.x + NAME_MARGIN, (int)m_Pos.y + NAME_MARGIN, m_Color, m_Name.c_str());
}

void Button::IsHit() {
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	if ((int)m_Pos.x < mouseX && (int)m_Pos.x + BOX_WIDTH > mouseX && (int)m_Pos.y < mouseY && (int)m_Pos.y + BOX_HEIGHT > mouseY) {
		// コールバック関数の実行
		m_onHitCallBackA();
		m_onHitCallBackB();
		m_onHitCallBackC(m_id, m_Name);
	}else {
		m_onHitCallBackC(-1, "");
	}
}

void Button::SetOnHitA(std::function<void()> _CallBack) {
	m_onHitCallBackA = _CallBack;
}

void Button::SetOnHitB(std::function<void()> _CallBack) {
	m_onHitCallBackB = _CallBack;
}