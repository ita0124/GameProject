#include "TreePlatform.h"

namespace {
	constexpr VECTOR	INIT_POS = { 0.0f,0.0f,0.0f };
	constexpr char		FILE_PATH[] = ("Data/Model/Stage/TreePlatform/TreePlatform.mv1");	//モデルファイルパス
}

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
//データ読み込み処理
void TreePlatform::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void TreePlatform::Step() {
}
