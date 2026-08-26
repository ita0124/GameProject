#include "NormalPlatform.h"

//コンストラクタ
NormalPlatform::NormalPlatform() {
	Init();
}
//デストラクタ
NormalPlatform::~NormalPlatform() {
	Exit();
}
//初期化処理
void NormalPlatform::Init() {
	PlatformBase::Init();

	m_PlatformKinds = NORMAL;	//足場オブジェクト種類を再設定
}
//毎フレーム呼び出す処理
void NormalPlatform::Step() {
}
