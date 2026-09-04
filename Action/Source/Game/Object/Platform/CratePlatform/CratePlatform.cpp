#include "CratePlatform.h"

//コンストラクタ
CratePlatform::CratePlatform() {
	Init();
}
//デストラクタ
CratePlatform::~CratePlatform() {
	Exit();
}
//初期化処理
void CratePlatform::Init() {
	PlatformBase::Init();

	m_PlatformKinds = CRATE;	//足場オブジェクト種類を再設定
}
//毎フレーム呼び出す処理
void CratePlatform::Step() {
}
