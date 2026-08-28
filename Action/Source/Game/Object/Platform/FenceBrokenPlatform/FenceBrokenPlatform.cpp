#include "FenceBrokenPlatform.h"

//コンストラクタ
FenceBrokenPlatform::FenceBrokenPlatform() {
	Init();
}
//デストラクタ
FenceBrokenPlatform::~FenceBrokenPlatform() {
	Exit();
}
//初期化処理
void FenceBrokenPlatform::Init() {
	PlatformBase::Init();

	m_PlatformKinds = FENCE_BROKEN;	//足場オブジェクト種類を再設定
}
//毎フレーム呼び出す処理
void FenceBrokenPlatform::Step() {
}
