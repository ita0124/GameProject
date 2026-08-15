#include "StatusDrawBase.h"

namespace {
	constexpr int ICON_OFFSET_Y = 32;	//アイコンのY座標補正値
}

//コンストラクタ
StatusDrawBase::StatusDrawBase() {
	Init();
}
//デストラクタ
StatusDrawBase::~StatusDrawBase() {
	Exit();
}
//初期化処理
void StatusDrawBase::Init() {
	Object2DBase::Init();
	m_Status = 0.0f;									//取得したステータスを保存する
	m_MaxStatus = 0.0f;									//最大ステータス量を保存
	m_ImageSpacing = 0;									// 次の画像との間隔
}
//描画処理
void StatusDrawBase::Draw() {
	if (!m_IsActive || m_Hndl.size() == 0)return;
	for (int Index = 0; Index < m_MaxStatus; Index++) {
		int X = m_ImageSpacing * Index;
		DrawRotaGraph((int)m_Pos.x + X, (int)m_Pos.y, 1.0f, 0.0f, m_Hndl[MAX_STATUS][0], TRUE);
	}
	for (int Index = 0; Index < m_Status; Index++) {
		int X = m_ImageSpacing * Index;
		DrawRotaGraph((int)m_Pos.x + X, (int)m_Pos.y, 1.0f, 0.0f, m_Hndl[STATUS][0], TRUE);
	}
	DrawRotaGraph3((int)0, (int)m_Pos.y - ICON_OFFSET_Y, 0, 0, 1.0f, 1.0f, m_Rot, m_Hndl[ICON][0], TRUE, m_IsTurn);
}
