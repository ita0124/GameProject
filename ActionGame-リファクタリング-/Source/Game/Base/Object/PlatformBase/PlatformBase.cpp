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

	memset(&m_NormalRequestData,0, sizeof(NORMAL_REQUEST_DATA));
}
//リクエスト
bool PlatformBase::Request(NORMAL_REQUEST_DATA _NormalRequestData) {
	if (m_IsActive)return false;
	//生存フラグオン
	m_IsActive = true;
	//最低限必要なデータ
	memcpy(&m_NormalRequestData, &_NormalRequestData, sizeof(NORMAL_REQUEST_DATA));
}
