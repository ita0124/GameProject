#include "GrassPlatform.h"

//コンストラクタ
GrassPlatform::GrassPlatform() {
	Init();
}
//デストラクタ
GrassPlatform::~GrassPlatform() {
	Exit();
}
//初期化処理
void GrassPlatform::Init() {
	PlatformBase::Init();

	m_PlatformKinds = GRASS;	//足場オブジェクト種類を再設定
	m_ObjectPush = false;		//オブジェクトを押し戻さない
}
//毎フレーム呼び出す処理
void GrassPlatform::Step() {
}
