#include "BattlePlatform.h"

namespace {
	const VECTOR	INIT_POS = { 100.0f,0.0f,0.0f };
	const char		FILE_PATH[] = ("Data/Model/Stage/BattlePlatform/BattlePlatform.mv1");	//モデルファイルパス
}

//コンストラクタ
BattlePlatform::BattlePlatform() {
	Init();
}
//デストラクタ
BattlePlatform::~BattlePlatform() {
	Exit();
}
//初期化処理
void BattlePlatform::Init() {
	ObjectBase::Init();
}
//データ読み込み処理
void BattlePlatform::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void BattlePlatform::Step() {
}
