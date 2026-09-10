#include "GimmickBase.h"

//コンストラクタ
GimmickBase::GimmickBase() {
	Init();
}
//デストラクタ
GimmickBase::~GimmickBase() {
	Exit();
}
//初期化処理
void GimmickBase::Init() {
	ObjectBase::Init();

	memset(&m_RequestData, 0, sizeof(REQUEST_DATA));					//リクエスト時に必要なデータ構造体を初期化

	m_GimmickKinds = BARRIERACTIVATOR;									//ギミックオブジェクト種類

	m_GimmickType = -1;													//作用させるギミックタイプ
}
//リクエスト
void GimmickBase::Request(const REQUEST_DATA& _RequestData) {
	//座標情報設定
	m_Pos = _RequestData.Pos;
	//回転率情報設定
	m_Rot = _RequestData.Rot;
	//拡縮情報設定
	m_Scale = _RequestData.Scale;
	//サイズ情報設定
	m_Size = _RequestData.Size;
	//作用するギミックタイプ設定
	m_GimmickType = _RequestData.GimmickType;
}
