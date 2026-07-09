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

