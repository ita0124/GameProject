#include "UIBase.h"

//コンストラクタ
UIBase::UIBase() {
	Init();
}

//デストラクタ
UIBase::~UIBase() {
	Exit();
}

//初期化処理
void UIBase::Init() {
	m_Pos = VZERO;
	m_Hndl.clear();
}

//データ破棄処理
void UIBase::Exit() {
	for (int i = 0; i < m_Hndl.size(); i++) {
		if (m_Hndl[i] != -1) {
			DeleteGraph(m_Hndl[i]);
			m_Hndl[i] = -1;
		}
	}
}

//データ読み込み処理
void UIBase::Load(const char* FilePath) {
	int Hndl = LoadGraph(FilePath);
	m_Hndl.push_back(Hndl);
}
