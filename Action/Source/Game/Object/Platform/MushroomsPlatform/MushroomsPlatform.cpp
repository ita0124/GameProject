#include "MushroomsPlatform.h"

//コンストラクタ
MushroomsPlatform::MushroomsPlatform() {
	Init();
}
//デストラクタ
MushroomsPlatform::~MushroomsPlatform() {
	Exit();
}
//初期化処理
void MushroomsPlatform::Init() {
	PlatformBase::Init();

	m_PlatformKinds = MUSHROOMS;	//足場オブジェクト種類を再設定
	m_ObjectPush = false;			//オブジェクトを押し戻さない
}
//毎フレーム呼び出す処理
void MushroomsPlatform::Step() {
}
