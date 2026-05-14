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

	memset(&m_NormalRequestData, 0, sizeof(NORMAL_REQUEST_DATA));					//リクエスト時に最低限必要なデータ構造体を初期化
	memset(&m_MovingPlatformRequestData, 0, sizeof(MOVINGPLATFORM_REQUEST_DATA));	//動く床をリクエストする時必要なデータ構造体を初期化

	m_PlatformKinds = NORMAL;														//足場オブジェクト種類を設定
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

	//Y軸をYサイズの半分下げる
	Res.y -= m_Size.y*0.5f;

	return Res;
}
//モデルの１フレーム前の中心を取る
VECTOR PlatformBase::GetPrevCenter() {
	VECTOR Res = m_PrevPos;

	//Y軸をYサイズの半分下げる
	Res.y -= m_Size.y * 0.5f;

	return Res;
}
