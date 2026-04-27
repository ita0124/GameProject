#include "MovingPlatform.h"

namespace {
	const VECTOR	INIT_POS = { 25.0f,0.0f,0.0f };
	const char		FILE_PATH[] = ("Data/Model/Stage/MovingPlatform/MovingPlatform.mv1");	//モデルファイルパス
}

//コンストラクタ
MovingPlatform::MovingPlatform() {
	Init();
}
//デストラクタ
MovingPlatform::~MovingPlatform() {
	Exit();
}
//初期化処理
void MovingPlatform::Init() {
	ObjectBase::Init();
	m_Pos = INIT_POS;
}
//データ読み込み処理
void MovingPlatform::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void MovingPlatform::Step() {
	m_Pos = VAdd(m_Pos, VGet(0.1f, 0.0f, 0.1f));
}
