#include "CharacterBase.h"

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
	m_Kinds = CHARACTER;		//種類設定

	m_Power = 0.0f;				//攻撃力
	m_HitPoints = 0.0f;			//体力
	m_Stamina = 0.0f;			//スタミナ
	m_SkillPoints = 0;			//スキルポイント

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
