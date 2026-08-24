#include "RespawnPlatform.h"

namespace {
	constexpr float		MIN_LEN = 0.0f;
	constexpr VECTOR	INIT_POS = { 0.0f,0.0f,0.0f };
	constexpr char		FILE_PATH[] = ("Data/Model/Stage/RespawnPlatform/RespawnPlatform.mv1");	//モデルファイルパス
}

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
}
//データ読み込み処理
void RespawnPlatform::Load() {
	ObjectBase::Load(FILE_PATH);
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
