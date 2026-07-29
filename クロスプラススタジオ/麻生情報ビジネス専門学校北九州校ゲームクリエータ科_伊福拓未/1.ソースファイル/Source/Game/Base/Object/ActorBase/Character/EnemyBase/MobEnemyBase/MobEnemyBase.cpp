#include "MobEnemyBase.h"

namespace {
	constexpr float	GRAVITY = -0.1f;			//重力
	constexpr float	GRAVITY_MAX = -5.0f;		//最大重力
}

//コンストラクタ
MobEnemyBase::MobEnemyBase() {
	Init();
}
//デストラクタ
MobEnemyBase::~MobEnemyBase() {
	Exit();
}
//初期化処理
void MobEnemyBase::Init() {
	ObjectBase::Init();

	memset(&m_NormalRequestData, 0, sizeof(REQUEST_DATA));					//リクエスト時に最低限必要なデータ構造体を初期化

	m_MobEnemyKinds = BOAR;														//足場オブジェクト種類を設定
}
//リクエスト
void MobEnemyBase::Request(const REQUEST_DATA& _NormalRequestData) {
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
//重力処理
void MobEnemyBase::GravityManager() {
	if (!m_IsGravity)return;
	//重力方向に加算
	m_Gravity += GRAVITY;
	//重力速度を制限
	if (m_Gravity <= GRAVITY_MAX) {
		m_Gravity = GRAVITY_MAX;
	}
	//座標に加算
	m_Pos.y += m_Gravity;
}
//ノックバックデータ数値代入
void MobEnemyBase::SetKnockBackData(float _Power, VECTOR _Pos) {
	m_KnockBackMaxDistance = _Power;
	//ノックバック開始時の敵座標
	m_KnockBackStartPos = _Pos;
	//
	m_KnockBackState = KNOCKBACK_START;
}
