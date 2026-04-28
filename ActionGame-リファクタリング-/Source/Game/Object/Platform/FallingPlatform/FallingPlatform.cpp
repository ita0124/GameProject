#include "FallingPlatform.h"

namespace {
	const VECTOR	INIT_POS = { 75.0f,0.0f,0.0f };
	const char		FILE_PATH[] = ("Data/Model/Stage/FallingPlatform/FallingPlatform.mv1");	//モデルファイルパス
}

//コンストラクタ
FallingPlatform::FallingPlatform() {
	Init();
}
//デストラクタ
FallingPlatform::~FallingPlatform() {
	Exit();
}
//初期化処理
void FallingPlatform::Init() {
	ObjectBase::Init();
}
//データ読み込み処理
void FallingPlatform::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void FallingPlatform::Step() {

}
