#include "NormalPlatform.h"

namespace {
	const VECTOR	INIT_POS = { 0.0f,0.0f,0.0f };
	const char		FILE_PATH[] = ("Data/Model/Stage/NormalPlatform/NormalPlatform.mv1");	//モデルファイルパス
}

//コンストラクタ
NormalPlatform::NormalPlatform() {
	Init();
}
//デストラクタ
NormalPlatform::~NormalPlatform() {
	Exit();
}
//初期化処理
void NormalPlatform::Init() {
	ObjectBase::Init();
}
//データ読み込み処理
void NormalPlatform::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void NormalPlatform::Step() {
}
