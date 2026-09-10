#include "TreePlatform.h"

//コンストラクタ
TreePlatform::TreePlatform() {
	Init();
}
//デストラクタ
TreePlatform::~TreePlatform() {
	Exit();
}
//初期化処理
void TreePlatform::Init() {
	PlatformBase::Init();

	m_PlatformKinds = TREE;	//足場オブジェクト種類を再設定
}
//毎フレーム呼び出す処理
void TreePlatform::Step() {
}
