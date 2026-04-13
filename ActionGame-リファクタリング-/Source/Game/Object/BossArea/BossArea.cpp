#include "BossArea.h"

namespace {
	const VECTOR	SCALE = { 10.0f,1.0f,10.0f };							//初期スケール
	const char		FILE_PATH[] = ("Data/Model/BossArea/BossArea.mv1");	//モデルファイルパス
}

//コンストラクタ
BossArea::BossArea() {
	Init();
}
//デストラクタ
BossArea::~BossArea() {
	Exit();
}
//初期化処理
void BossArea::Init() {
	ObjectBase::Init();

	m_Scale = SCALE;	//スケール再設定
}
//データ破棄処理
void BossArea::Exit() {
	//コリジョン情報を確実に消去する
	MV1TerminateCollInfo(m_Hndl);

	ObjectBase::Exit();
}
//データ読み込み処理
void BossArea::Load() {
	ObjectBase::Load(FILE_PATH);

	//コリジョン情報を構築
	MV1SetupCollInfo(m_Hndl);
}
//毎フレーム呼び出す処理
void BossArea::Step() {
}
