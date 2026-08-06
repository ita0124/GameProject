#include "CharacterBase.h"

namespace {
	//ノックバック量の減衰率
	constexpr float KNOCKBACK_DECAY_RATE = 0.1f;
}

//コンストラクタ
CharacterBase::CharacterBase() {
	Init();
}
//デストラクタ
CharacterBase::~CharacterBase() {
	Exit();
}
//初期化処理
void CharacterBase::Init() {
	ActorBase::Init();
	m_Kinds = CHARACTER;						//種類設定

	m_Power = 0.0f;								//攻撃力
	m_HitPoints = 0.0f;							//体力
	m_MaxHitPoints = 0.0f;						//最大体力
	m_Stamina = 0.0f;							//スタミナ
	m_MaxStamina = 0.0f;						//最大スタミナ
	m_SkillPoints = 0;							//スキルポイント

	m_KnockBackStartPos = VZERO;				//ノックバック開始時の敵座標
	m_KnockBackDistance = 0.0f;					//現在のノックバック量
	m_KnockBackMaxDistance = 0.0f;				//最大ノックバック量
	m_KnockBackSub = 0.0f;						//1フレーム毎のノックバック力減衰量
	m_KnockBackDuration = 0;					//ノックバック継続時間	
	m_KnockBackState = KNOCKBACK_NOT;			//ノックバック状態管理変数

	m_IsStaminaRecover = true;	//スタミナを回復させてよいか
}
//回転値更新
void CharacterBase::UpdateRotation(VECTOR _MoveVec, float _RotSpeed) {
	//移動方向から目標回転角を計算
	float TargetRot = atan2f(_MoveVec.x, _MoveVec.z);
	//現在の回転角との差を計算
	float RotDif = TargetRot - m_Rot.y;
	//角度差を-π～-πの範囲に補正
	//どっち回りをするべきか
	if (RotDif > (float)DX_PI)
	{
		RotDif -= (float)DX_TWO_PI;
	}
	else if (RotDif < -(float)DX_PI)
	{
		RotDif += (float)DX_TWO_PI;
	}
	//1フレームあたりの回転量
	float RotSpeed = _RotSpeed;
	//目標角度まで近ければそのまま合わせる
	if (fabsf(RotDif) <= RotSpeed)
	{
		m_Rot.y = TargetRot;
	}
	//一定速度で目標方向へ回転
	else
	{
		if (RotDif > 0.0f)
		{
			m_Rot.y += RotSpeed;
		}
		else
		{
			m_Rot.y -= RotSpeed;
		}
	}
}
//ノックバック
void CharacterBase::KnockBackManager() {
	switch (m_KnockBackState)
	{
	case KNOCKBACK_NOT:
		break;
	case KNOCKBACK_START:
		//状態をCHARGEへ
		m_KnockBackState = KNOCKBACK_BACK;
		//1フレーム当たりの減衰量
		m_KnockBackSub = m_KnockBackMaxDistance * KNOCKBACK_DECAY_RATE;
		//ノックバック継続時間をリセット
		m_KnockBackDuration = 0;
		//ノックバック量初期化
		m_KnockBackDistance = 0;
		break;
	case KNOCKBACK_BACK:
		m_KnockBackDuration++;

		//方向ベクトルを取得(正規化済み)
		VECTOR DirToMyPos = GetDirectionNotY(m_KnockBackStartPos, m_Pos, true);
		//移動量を計算
		VECTOR KnockBackSpeed = VScale(DirToMyPos, m_KnockBackDistance);
		//座標に加算
		m_Pos = VAdd(m_Pos, KnockBackSpeed);
		//ノックバック量を1フレーム毎の減衰量分減らす
		m_KnockBackDistance += m_KnockBackSub;

		if (m_KnockBackDistance >= m_KnockBackMaxDistance)
		{
			m_KnockBackState = KNOCKBACK_NOT;
		}
		break;
	}
}
