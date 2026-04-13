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
	m_Kinds = Character;		//種類設定

	m_Power = FZERO;			//攻撃力
	m_HitPoints = FZERO;		//体力
	m_Stamina = FZERO;			//スタミナ
	m_SkillPoints = 0;			//スキルポイント

	m_IsStaminaRecover = true;	//スタミナを回復させてよいか
}

