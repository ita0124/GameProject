#include "BossArea.h"

namespace {
	constexpr VECTOR	SCALE = { 50.0f,1.0f,50.0f };											//スケール
	constexpr VECTOR	BOSSAREA_SIZE = { 20.0f * SCALE.x,20.0f * SCALE.y,20.0f * SCALE.z };	//当たり判定サイズ
	constexpr char		FILE_PATH[] = ("Data/Model/Stage/NormalPlatform/NormalPlatform.mv1");	//モデルファイルパス
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

	m_Scale = SCALE;			//スケール再設定
	m_Size = BOSSAREA_SIZE;		//サイズ再設定
}
//データ破棄処理
void BossArea::Exit() {
	ObjectBase::Exit();
}
//データ読み込み処理
void BossArea::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void BossArea::Step() {
}
