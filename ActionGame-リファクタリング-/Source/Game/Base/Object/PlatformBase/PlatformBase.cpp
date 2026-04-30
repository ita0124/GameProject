#include "PlatformBase.h"

//コンストラクタ
PlatformBase::PlatformBase() {
	Init();
}
//デストラクタ
PlatformBase::~PlatformBase() {
	Exit();
}
//初期化処理
void PlatformBase::Init() {
	ObjectBase::Init();

	memset(&m_NormalRequestData, 0, sizeof(NORMAL_REQUEST_DATA));
}
//リクエスト
void PlatformBase::Request(const NORMAL_REQUEST_DATA& _NormalRequestData) {
	//生存フラグオン
	m_IsActive = true;
	//座標情報設定
	m_Pos = _NormalRequestData.Pos;
	//回転率情報設定
	m_Rot = _NormalRequestData.Rot;
	//拡縮情報設定
	m_Scale = _NormalRequestData.Scale;
	//サイズ情報設定
	m_Size = _NormalRequestData.Size;
}
//モデルの中心を取る
VECTOR PlatformBase::GetCenter() {
	VECTOR Res = m_Pos;

	//Y軸を半径分上げる
	Res.y -= m_Size.y/2;

	return Res;
}
