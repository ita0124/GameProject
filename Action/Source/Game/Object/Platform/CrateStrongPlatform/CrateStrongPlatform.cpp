#include "CrateStrongPlatform.h"

//コンストラクタ
CrateStrongPlatform::CrateStrongPlatform() {
	Init();
}
//デストラクタ
CrateStrongPlatform::~CrateStrongPlatform() {
	Exit();
}
//初期化処理
void CrateStrongPlatform::Init() {
	PlatformBase::Init();

	m_PlatformKinds = CRATE_STRONG;	//足場オブジェクト種類を再設定
}
//毎フレーム呼び出す処理
void CrateStrongPlatform::Step() {
}
