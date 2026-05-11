#pragma once
#include "DxLib.h"
#include <string>
#include "Button.h"

class Game {
public:
	Game(){}
	~Game(){}

	void Init();
	void Step();
	void Draw();
	void Exit();

private:
	Button		m_Button;
	int			m_Point;
	std::string m_Name;

	void Test() { return; }
};