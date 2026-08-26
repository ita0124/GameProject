#include "FencePlatform.h"

//コンストラクタ
FencePlatform::FencePlatform() {
	Init();
}
//デストラクタ
FencePlatform::~FencePlatform() {
	Exit();
}
//初期化処理
void FencePlatform::Init() {
	PlatformBase::Init();

	m_PlatformKinds = FENCE;	//足場オブジェクト種類を再設定
}
//毎フレーム呼び出す処理
void FencePlatform::Step() {
}
