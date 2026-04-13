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

	static TagFade m_State;
	static int m_Count;
	static bool m_Flg;

public:
	Fade();

	static void Init();

	static void Step();

	static void Draw();

	static void RequestIn();

	static void RequestOut();

	static bool IsEndIn();

	static bool IsEndOut();
};
