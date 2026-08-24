#include "FlowersPlatform.h"

namespace {
	constexpr VECTOR	INIT_POS = { 0.0f,0.0f,0.0f };
	constexpr char		FILE_PATH[] = ("Data/Model/Stage/FlowersPlatform/FlowersPlatform.mv1");	//モデルファイルパス
}

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
//データ読み込み処理
void FlowersPlatform::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void FlowersPlatform::Step() {
}
