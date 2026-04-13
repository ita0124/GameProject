#include "Target.h"

namespace {
	const char FILE_PATH[] = "Data/UI/Target.png";
}

//コンストラクタ
Target::Target() {
	Init();
}

//デストラクタ
Target::~Target() {
	Exit();
}

//初期化処理
void Target::Init() {
	UIBase::Init();
}

//データ破棄処理
void Target::Exit() {
	UIBase::Exit();
}

//データ読み込み処理
void Target::Load() {
	UIBase::Load(FILE_PATH);
}

//描画処理
void Target::Draw() {
	DrawRotaGraph((int)m_Pos.x, (int)m_Pos.y, 1.0f, 0.0f, m_Hndl[0], TRUE);
}

//座標更新
void Target::Update(VECTOR _Pos) {
	m_Pos = _Pos;
}