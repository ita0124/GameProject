#include "ArrowPlatform.h"

//コンストラクタ
ArrowPlatform::ArrowPlatform() {
	Init();
}
//デストラクタ
ArrowPlatform::~ArrowPlatform() {
	Exit();
}
//初期化処理
void ArrowPlatform::Init() {
	PlatformBase::Init();

	m_PlatformKinds = ARROW;	//足場オブジェクト種類を再設定
}
//毎フレーム呼び出す処理
void ArrowPlatform::Step() {
}
