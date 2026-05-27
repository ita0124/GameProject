#include "UIFrame.h"

namespace {
	const char FILE_PATH[] = "Data/UI/UIFrame.png";

	const VECTOR INIT_POS = { 300.0f,80.0f,0.0f };

}

//コンストラクタ
UIFrame::UIFrame() {
	Init();
}

//デストラクタ
UIFrame::~UIFrame() {
	Exit();
}

//初期化処理
void UIFrame::Init() {
	UIBase::Init();
	m_Pos = INIT_POS;
}

//データ破棄処理
void UIFrame::Exit() {
	UIBase::Exit();
}

//データ読み込み処理
void UIFrame::Load() {
	UIBase::Load(FILE_PATH);
}

//描画処理
void UIFrame::Draw() {
	DrawRotaGraph((int)m_Pos.x, (int)m_Pos.y, 1.0f, 0.0f, m_Hndl[0], TRUE);
}