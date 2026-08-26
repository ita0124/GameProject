#include "BarrierPlatform.h"
#include "Lib/LoadMaterial/LoadMaterial.h"

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
	PlatformBase::Init();

	m_PlatformKinds = BARRIER;	//足場オブジェクト種類を再設定
	m_PlayerPush = false;		//プレイヤーを押し戻すか
}
//毎フレーム呼び出す処理
void BarrierPlatform::Step() {

}
//描画処理
void BarrierPlatform::Draw() {
	if (!m_PlayerPush)return;
	// 不透明度を50%にする
	MV1SetOpacityRate(m_Hndl, HALF);
	MV1DrawModel(m_Hndl);				//モデル描画
}