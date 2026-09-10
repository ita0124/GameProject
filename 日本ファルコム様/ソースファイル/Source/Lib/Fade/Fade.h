#pragma once
#include "Comon.h"

class Fade {
private:
	enum TagFade {
		FADE_NON,		//フェードなし
		FADE_IN,		//フェードイン中
		FADE_OUT,		//フェードアウト中
		FADE_OUT_WAIT,	//フェードアウトの後

		NUM
	};

	static TagFade	m_State;
	static int		m_Count;
	static bool		m_Flg;
	static int		m_Speed;
	static int		m_Color;

public:
	Fade();

	static void Init();

	static void Step();

	static void Draw();

	static void RequestIn(int _Speed, int _Color = WHITE);

	static void RequestOut(int _Speed, int _Color = WHITE);

	static bool IsEndIn();

	static bool IsEndOut();
};
