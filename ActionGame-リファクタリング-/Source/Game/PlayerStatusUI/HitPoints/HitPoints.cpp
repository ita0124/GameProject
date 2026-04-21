#include "HitPoints.h"

namespace {
	const int HNDL_MAX = 2;

	const char* FILE_PATH[HNDL_MAX] = {
		"Data/UI/HitPoints.png",
		"Data/UI/Gray.png",
	};

	const VECTOR INIT_POS = { 50.0f,50.0f,0.0f };
	const int CALC_X = 5;
}

//コンストラクタ
HitPoints::HitPoints() {
	Init();
}

//デストラクタ
HitPoints::~HitPoints() {
	Exit();
}

//初期化処理
void HitPoints::Init() {
	PlayerStatusUIBase::Init();
	m_Pos = INIT_POS;
}

//データ破棄処理
void HitPoints::Exit() {
	PlayerStatusUIBase::Exit();
}

//データ読み込み処理
void HitPoints::Load() {
	for (int i = 0; i < HNDL_MAX; i++) {
		PlayerStatusUIBase::Load(FILE_PATH[i]);
	}
}

//モデル更新処理
void HitPoints::Update(int _Num) {
	PlayerStatusUIBase::Update(_Num);
}

//描画処理
void HitPoints::Draw() {
	for (int i = 0; i < 100; i++) {
		int X = CALC_X * i;
		DrawRotaGraph((int)m_Pos.x + X, (int)m_Pos.y, 1.0f, 0.0f, m_Hndl[1], TRUE);
	}

	for (int i = 0; i < m_Num; i++) {
		int X = CALC_X * i;
		DrawRotaGraph((int)m_Pos.x + X, (int)m_Pos.y, 1.0f, 0.0f, m_Hndl[0], TRUE);
		DrawFormatStringToHandle((int)m_Pos.x + 500, (int)m_Pos.y - 15, ORANGE, DxLibFont::FONTHNDL_N30, "%d", m_Num);
	}
	DrawFormatStringToHandle((int)10, (int)m_Pos.y - 15, GREEN, DxLibFont::FONTHNDL_N30, "HP");
	DrawFormatStringToHandle((int)m_Pos.x + 500, (int)m_Pos.y - 15, GREEN, DxLibFont::FONTHNDL_N30, "%d", m_Num);
}
