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
	m_GimmickType = -1;																//作用するギミックタイプ
	m_ObjectPush = true;															//オブジェクトを押し戻すか
	m_PlayerPush = true;															//プレイヤーを押し戻すか
	m_EnemyPush = true;																//敵を押し戻すか
}
// データ読み込み処理
void PlatformBase::Load(const int _Hndl) {
	if (m_Hndl != -1)return;
	//モデルロード
	m_Hndl = MV1DuplicateModel(_Hndl);
}
//リクエスト
void PlatformBase::Request(const NORMAL_REQUEST_DATA& _NormalRequestData) {
	//座標情報設定
	m_Pos = _NormalRequestData.Pos;
	//回転率情報設定
	m_Rot = _NormalRequestData.Rot;
	//拡縮情報設定
	m_Scale = _NormalRequestData.Scale;
	//サイズ情報設定
	m_Size = _NormalRequestData.Size;
	//作用するギミックタイプ設定
	m_GimmickType = _NormalRequestData.GimmickType;
}
