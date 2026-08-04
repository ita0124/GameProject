#include "BarrierPlatform.h"
#include "Lib/LoadMaterial/LoadMaterial.h"

namespace {
	constexpr VECTOR	INIT_POS = { 0.0f,0.0f,0.0f };
	constexpr char		FILE_PATH[] = ("Data/Model/Stage/BarrierWall/BarrierWall.mv1");	//モデルファイルパス
}

//コンストラクタ
BarrierPlatform::BarrierPlatform() {
	Init();
}
//デストラクタ
BarrierPlatform::~BarrierPlatform() {
	Exit();
}
//初期化処理
void BarrierPlatform::Init() {
	ObjectBase::Init();

	m_PlatformKinds = BARRIER;	//足場オブジェクト種類を再設定
	m_IsActive = false;			//最初は表示しない
}
//データ読み込み処理
void BarrierPlatform::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void BarrierPlatform::Step() {
	m_IsActive;
}
//描画処理
void BarrierPlatform::Draw() {
	if (!m_IsActive)return;
	// 不透明度を50%にする
	MV1SetOpacityRate(m_Hndl, HALF);
	MV1DrawModel(m_Hndl);				//モデル描画
}