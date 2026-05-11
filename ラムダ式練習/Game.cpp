#pragma once
#include "Game.h"

void Game::Init() {
	m_Point = 0;
	m_Name = "";

	m_Button.Init(VGet(10.0f, 50.0f, 0.0f), "Name", GetColor(255, 0, 0), 1);

	// 何もキャプチャしないラムダ式
	m_Button.SetOnHitA([]() {
		DrawFormatString(0, 0, GetColor(255, 255, 255), "onHit");
		});

	// 自身のクラスをキャプチャしたラムダ式
	m_Button.SetOnHitB([this]() {
		m_Point += 1;
		if (m_Point > 100)
			m_Point = 0;
		});

	// 自身のクラスをキャプチャした引数と戻り値があるラムダ式
	m_Button.SetOnHitC([this](int _id, std::string _name) {
		DrawFormatString(250, 0, GetColor(255, 255, 255), "ボタンID = [%2d]", _id);
		m_Name = _name;
		return true;
		});
}

void Game::Step() {
	m_Button.IsHit();

	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0){
		// 何もキャプチャしないラムダ式
		m_Button.SetOnHitA([]() {
			DrawFormatString(0, 0, GetColor(255, 0, 0), "onHitSP");
			});

		// 自身のクラスをキャプチャしたラムダ式
		m_Button.SetOnHitB([this]() {
			m_Point -= 1;
			if (m_Point < -100)
				m_Point = 0;
			});
		// 自身のクラスをキャプチャした引数と戻り値があるラムダ式
		m_Button.SetOnHitC([this](int _id, std::string _name) {
			DrawFormatString(250, 0, GetColor(255, 0, 0), "ボタンID = [%2d]", _id);
			m_Name = _name;
			return true;
			});
	}
	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) {
		// 何もキャプチャしないラムダ式
		m_Button.SetOnHitA([]() {
			DrawFormatString(0, 0, GetColor(0, 255, 0), "onHitSP");
			});

		// 自身のクラスをキャプチャしたラムダ式
		m_Button.SetOnHitB([this]() {
			m_Point += 1;
			if (m_Point > 100)
				m_Point = 0;
			});
		// 自身のクラスをキャプチャした引数と戻り値があるラムダ式
		m_Button.SetOnHitC([this](int _id, std::string _name) {
			DrawFormatString(250, 0, GetColor(0, 255, 0), "ボタンID = [%2d]", _id);
			m_Name = _name;
			return true;
			});
	}
}

void Game::Draw() {
	m_Button.Draw();
	DrawFormatString(100, 0, GetColor(255, 255, 255), "m_Point = [%3d]", m_Point);
	DrawFormatString(0, 15, GetColor(255, 255, 255), "Game::m_Name = [%s]", m_Name.c_str());
}

void Game::Exit() {

}