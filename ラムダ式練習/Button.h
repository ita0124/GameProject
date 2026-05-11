#pragma once
#include "DxLib.h"
#include <string>
#include <functional>

constexpr int BOX_WIDTH		= 200;
constexpr int BOX_HEIGHT	= 100;

constexpr int NAME_MARGIN = 5;

class Button {
public:
	Button();
	~Button();

	void	Init(VECTOR _Pos, std::string _Name, unsigned int _Color, int _id);
	void	Draw();
	void	IsHit();

	// コールバック関数のセット
	void	SetOnHitA(std::function<void()> _CallBack);
	void	SetOnHitB(std::function<void()> _CallBack);
	void	SetOnHitC(std::function<bool(int, std::string)> _CallBack) { m_onHitCallBackC = _CallBack; }

private:
	VECTOR			m_Pos;
	std::string		m_Name;
	unsigned int	m_Color;
	int				m_id;

	// コールバック関数用変数
	std::function<void()> m_onHitCallBackA;
	std::function<void()> m_onHitCallBackB;
	std::function<bool(int, std::string)> m_onHitCallBackC;
};