#include "FPS.h"

constexpr int FRAME_RATE = 60;
constexpr int FRAME_RATE_MILLI_SECOND = 1000 / FRAME_RATE;

float DebugFPS::m_FrameRate;
int DebugFPS::m_NowTime;
int DebugFPS::m_OldTime;
int DebugFPS::m_OldDrawTime;
int DebugFPS::m_Count;
int DebugFPS::m_FontHndl;

//コンストラクタ
DebugFPS::DebugFPS() {
	Init();
}

//初期化
void DebugFPS::Init() {
	m_FrameRate = 0.0f;
	m_NowTime = 0;
	m_OldTime = GetNowCount();
	m_OldDrawTime = GetNowCount();;
	m_Count = 0;
	m_FontHndl = CreateFontToHandle(NULL, 20, -1, DX_FONTTYPE_NORMAL);
}

//FPSを表示
void DebugFPS::Print() {
	Calcu();

	int Color = GetColor(255, 0, 0);
	/*文字の表示*/
#ifdef _DEBUG
	DrawFormatStringToHandle(0, 0, Color, m_FontHndl, "FPS:%.2f", m_FrameRate);
#endif // DEBUG
}

//FPS計算
void DebugFPS::Calcu() {
	//まずはカウントを増やす
	m_Count++;
	//最新の時間を取得
	m_NowTime = GetNowCount();
	//前回時間を取得してどのくらいの時間が経過したか
	int DifTime = m_NowTime - m_OldDrawTime;

	if (DifTime >= 1000) {
		//正常に動いていた場合 m_Countは60程度になっている
		float FrameCount = (float)(m_Count * 1000);
		//表示するFPSを更新
		m_FrameRate = FrameCount / DifTime;
		//カウントを初期化
		m_Count = 0;
		//前回時間を更新
		m_OldDrawTime = m_NowTime;
	}
}

//次のフレームに進めて良いか
bool DebugFPS::Next() {
	//最新の時間を取得
	m_NowTime = GetNowCount();
	//前回時間を取得してどのくらいの時間が経過したか
	int DifTime = m_NowTime - m_OldTime;

	if (DifTime >= FRAME_RATE_MILLI_SECOND) {
		return true;
	}
	else return false;
}

//FPS更新
void DebugFPS::Step() {
	//ここまで来たなら時間を更新する
	m_OldTime = m_NowTime;
}

