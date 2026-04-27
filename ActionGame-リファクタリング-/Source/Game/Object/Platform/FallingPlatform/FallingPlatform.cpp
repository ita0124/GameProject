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
	m_Pos = INIT_POS;
	m_Time = 0;
	m_RespawnWait = 0;
}
//データ読み込み処理
void FallingPlatform::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void FallingPlatform::Step() {

	if (m_IsActive) {
		if (m_Time > 150) {
			m_IsActive = false;
			m_RespawnWait = 0;
		}
		else {
			m_Time++;
			m_Pos = VAdd(m_Pos, VGet(0.0f, -0.2f, 0.0f));
		}
	}
	else {
		if (m_RespawnWait > 120) {
			m_IsActive = true;
			m_Time = 0;
			m_Pos = INIT_POS;
		}
		else {
			m_RespawnWait++;
		}
	}
}
