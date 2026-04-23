#include "StageBase.h"

//コンストラクタ
StageBase::StageBase() {
	Init();
}
//デストラクタ
StageBase::~StageBase() {
	Exit();
}
//リクエスト
bool StageBase::Request(const VECTOR& _Pos, const VECTOR& _Rot) {
	if (m_IsActive)return false;
	//生存フラグオン
	m_IsActive = true;
	//座標設定
	m_Pos = _Pos;
	//ラジアン角に修正
	VECTOR Rot = VGet(_Rot.x * DX_PI_F / 180.0f, _Rot.y * DX_PI_F / 180.0f, _Rot.z * DX_PI_F / 180.0f);
	m_Rot = Rot;
}
