#include "Stamina.h"

namespace {
	const int HNDL_MAX = 2;

	const char* FILE_PATH[HNDL_MAX] = {
		"Data/UI/Stamina.png",
		"Data/UI/Gray.png",
	};

	const VECTOR INIT_POS = { 50.0f,100.0f,0.0f };
	const int CALC_X = 5;
}

//コンストラクタ
Stamina::Stamina() {
	Init();
}

//デストラクタ
Stamina::~Stamina() {
	Exit();
}

//初期化処理
void Stamina::Init() {
	PlayerStatusUIBase::Init();
	m_Pos = INIT_POS;
}

//データ破棄処理
void Stamina::Exit() {
	PlayerStatusUIBase::Exit();
}

//データ読み込み処理
void Stamina::Load() {
	for (int i = 0; i < HNDL_MAX; i++) {
		PlayerStatusUIBase::Load(FILE_PATH[i]);
	}
}

//モデル更新処理
void Stamina::Update(int _Num) {
	PlayerStatusUIBase::Update(_Num);
}

//描画処理
void Stamina::Draw() {
	for (int i = 0; i < 100; i++) {
		int X = CALC_X * i;
		DrawRotaGraph((int)m_Pos.x + X, (int)m_Pos.y, 1.0f, 0.0f, m_Hndl[1], TRUE);
	}

	for (int i = 0; i < m_Num; i++) {
		int X = CALC_X * i;
		DrawRotaGraph((int)m_Pos.x + X, (int)m_Pos.y, 1.0f, 0.0f, m_Hndl[0], TRUE);

		DrawFormatStringToHandle((int)8, (int)m_Pos.y - 15, ORANGE, DxLibFont::FONTHNDL_N30, "ST");
		DrawFormatStringToHandle((int)m_Pos.x + 500, (int)m_Pos.y - 15, ORANGE, DxLibFont::FONTHNDL_N30, "%d", m_Num);
	}
}
