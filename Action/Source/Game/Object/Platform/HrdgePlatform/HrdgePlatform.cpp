#include "HrdgePlatform.h"

namespace {
	constexpr VECTOR	INIT_POS = { 0.0f,0.0f,0.0f };
	constexpr char		FILE_PATH[] = ("Data/Model/Stage/HrdgePlatform/HrdgePlatform.mv1");	//モデルファイルパス
}

//コンストラクタ
HrdgePlatform::HrdgePlatform() {
	Init();
}
//デストラクタ
HrdgePlatform::~HrdgePlatform() {
	Exit();
}
//初期化処理
void HrdgePlatform::Init() {
	PlatformBase::Init();

	m_PlatformKinds = HRDGE;	//足場オブジェクト種類を再設定
}
//データ読み込み処理
void HrdgePlatform::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void HrdgePlatform::Step() {
}
