#include "SkillPoints.h"

namespace {
	const int HNDL_MAX = 3;

	const char* FILE_PATH[HNDL_MAX] = {
		"Data/UI/SkillPoints.png",
		"Data/UI/Gray.png",
		"Data/UI/SkillPointsFrame.png"
	};

	const VECTOR INIT_POS = { 50.0f,150.0f,0.0f };
	const int CALC_X = 5;
}

//コンストラクタ
SkillPoints::SkillPoints() {
	Init();
}

//デストラクタ
SkillPoints::~SkillPoints() {
	Exit();
}

//初期化処理
void SkillPoints::Init() {
	PlayerStatusUIBase::Init();
	m_Pos = INIT_POS;
}

//データ破棄処理
void SkillPoints::Exit() {
	PlayerStatusUIBase::Exit();
}

//データ読み込み処理
void SkillPoints::Load() {
	for (int i = 0; i < HNDL_MAX; i++) {
		PlayerStatusUIBase::Load(FILE_PATH[i]);
	}
}

//モデル更新処理
void SkillPoints::Update(int _Num) {
	PlayerStatusUIBase::Update(_Num);
}

//描画処理
void SkillPoints::Draw() {
	for (int i = 0; i < 100; i++) {
		int X = CALC_X * i;
		DrawRotaGraph((int)m_Pos.x + X, (int)m_Pos.y, 1.0f, 0.0f, m_Hndl[1], TRUE);
	}

	for (int i = 0; i < m_Num; i++) {
		int X = CALC_X * i;
		DrawRotaGraph((int)m_Pos.x + X, (int)m_Pos.y, 1.0f, 0.0f, m_Hndl[0], TRUE);
	}

	for (int i = 0; i < 10; i++) {
		int X = 22 + 50 * i;
		DrawRotaGraph((int)m_Pos.x + X, (int)m_Pos.y - 1, 1.0f, 0.0f, m_Hndl[2], TRUE);
	}

	DrawFormatStringToHandle((int)8, (int)m_Pos.y - 15, SKYBLUE, DxLibFont::FONTHNDL_N30, "SG");
	DrawFormatStringToHandle((int)m_Pos.x + 500, (int)m_Pos.y - 15, SKYBLUE, DxLibFont::FONTHNDL_N30, "%d", m_Num / 10);
}
