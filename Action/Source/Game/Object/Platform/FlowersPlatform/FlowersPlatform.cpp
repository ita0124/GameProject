#include "FlowersPlatform.h"

//コンストラクタ
FlowersPlatform::FlowersPlatform() {
	Init();
}
//デストラクタ
FlowersPlatform::~FlowersPlatform() {
	Exit();
}
//初期化処理
void FlowersPlatform::Init() {
	PlatformBase::Init();

	m_PlatformKinds = FLOWERS;	//足場オブジェクト種類を再設定
	m_ObjectPush = false;		//オブジェクトを押し戻さない
}
//毎フレーム呼び出す処理
void FlowersPlatform::Step() {
}
