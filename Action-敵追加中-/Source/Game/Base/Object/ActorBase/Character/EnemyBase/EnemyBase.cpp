#include "EnemyBase.h"

//コンストラクタ
EnemyBase::EnemyBase() {
	Init();
}
//デストラクタ
EnemyBase::~EnemyBase() {
	Exit();
}
//初期化処理
void EnemyBase::Init() {
	CharacterBase::Init();
	m_Kinds = ENEMY;		//種類設定

	m_PlayerPos = VZERO;	//プレイヤー座標

	m_NextActionTime = 0;	//次の行動までの待機時間
	m_DownTime = 0;			//ダウン状態継続時間
	m_DamageTime = 0;		//ダメージ処理の継続時間
}
//ダメージ処理を管理
void EnemyBase::DamageManager() {
	if (m_DamageTime <= 0) {
		//当たり判定オン
		m_IsCollision = true;
		//ダメージ処理の継続時間をリセット
		m_DamageTime = 0;
	}
	else {
		m_DamageTime--;
	}
}
