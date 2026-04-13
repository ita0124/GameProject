#include "PlayerStatusUIBase.h"

//コンストラクタ
PlayerStatusUIBase::PlayerStatusUIBase() {
	Init();
}

//デストラクタ
PlayerStatusUIBase::~PlayerStatusUIBase() {
	Exit();
}

//初期化処理
void PlayerStatusUIBase::Init() {
	m_Num = -1;
	UIBase::Init();
}

//データ破棄処理
void PlayerStatusUIBase::Exit() {
	UIBase::Exit();
}

//データ読み込み処理
void PlayerStatusUIBase::Load(const char* FilePath) {
	UIBase::Load(FilePath);
}

//数値更新
void PlayerStatusUIBase::Update(int _Num) {
	m_Num = _Num;
}
