#include "JumpPlatform.h"

namespace {
	const VECTOR	INIT_POS = { 50.0f,0.0f,0.0f };
	const char		FILE_PATH[] = ("Data/Model/Stage/JumpPlatform/JumpPlatform.mv1");	//モデルファイルパス
}

//コンストラクタ
JumpPlatform::JumpPlatform() {
	Init();
}
//デストラクタ
JumpPlatform::~JumpPlatform() {
	Exit();
}
//初期化処理
void JumpPlatform::Init() {
	ObjectBase::Init();
	m_Pos = INIT_POS;
}
//データ読み込み処理
void JumpPlatform::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void JumpPlatform::Step() {
}
