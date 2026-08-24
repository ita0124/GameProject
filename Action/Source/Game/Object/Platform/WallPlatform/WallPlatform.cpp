#include "WallPlatform.h"

namespace {
	constexpr char		FILE_PATH[] = ("");	//モデルファイルパス
}

//コンストラクタ
WallPlatform::WallPlatform() {
	Init();
}
//デストラクタ
WallPlatform::~WallPlatform() {
	Exit();
}
//初期化処理
void WallPlatform::Init() {
	PlatformBase::Init();

	m_PlatformKinds = WALL;	//足場オブジェクト種類を再設定
	m_PlayerPush = false;	//プレイヤーを押し戻すか
}
//データ読み込み処理
void WallPlatform::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void WallPlatform::Step() {
}
