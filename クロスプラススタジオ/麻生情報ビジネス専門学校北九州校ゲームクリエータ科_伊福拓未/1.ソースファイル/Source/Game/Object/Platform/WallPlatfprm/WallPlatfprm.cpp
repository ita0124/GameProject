#include "WallPlatfprm.h"

namespace {
	constexpr VECTOR	INIT_POS = { 0.0f,0.0f,0.0f };								
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
	ObjectBase::Init();

	m_PlatformKinds = WALL;	//足場オブジェクト種類を再設定
}
//データ読み込み処理
void WallPlatform::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void WallPlatform::Step() {
}
