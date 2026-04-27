#include "StageBase.h"

//コンストラクタ
StageBase::StageBase() {
	Init();
}
//デストラクタ
StageBase::~StageBase() {
	Exit();
}
//初期化処理
void StageBase::Init() {
	ObjectBase::Init();

	memset(&m_NormalRequestData,0, sizeof(NORMAL_REQUEST_DATA));
}
//リクエスト
bool StageBase::Request(NORMAL_REQUEST_DATA _NormalRequestData) {
	if (m_IsActive)return false;
	//生存フラグオン
	m_IsActive = true;
	//座標設定
	m_Pos = _NormalRequestData.Pos;
	//ラジアン角に修正
	VECTOR Rot = VGet(_NormalRequestData.Rot.x * RADIAN_CALC, _NormalRequestData.Rot.y * RADIAN_CALC, _NormalRequestData.Rot.z * RADIAN_CALC);
	//回転率設定
	m_Rot = Rot;
	//拡縮設定
	m_Scale = _NormalRequestData.Scale;
	//サイズ設定
	m_Size = _NormalRequestData.Size;
	//最低限必要なデータはここまで



}
