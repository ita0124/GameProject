#include "RespawnPlatform.h"

//コンストラクタ
RespawnPlatform::RespawnPlatform() {
	Init();
}
//デストラクタ
RespawnPlatform::~RespawnPlatform() {
	Exit();
}
//初期化処理
void RespawnPlatform::Init() {
	PlatformBase::Init();

	m_Object = nullptr;
	m_EnemyPush = false;		//敵を押し戻さない
}
//毎フレーム呼び出す処理
void RespawnPlatform::Step() {
}
//当たり判定後の処理(当たっている場合)
void RespawnPlatform::HitCalc(ObjectBase* _Object) {
	if (_Object->GetKinds() == ObjectBase::TagKinds::PLAYER) {
		m_Object = _Object;
		if (m_Object != nullptr) {
			m_Object->SetRespawnPos(m_Pos);	
			m_Object = nullptr;
		}
		//マテリアルを変更
		MV1SetTextureGraphHandle(m_Hndl, 0, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_AFTER_RESPAWN), FALSE);
	}
}
