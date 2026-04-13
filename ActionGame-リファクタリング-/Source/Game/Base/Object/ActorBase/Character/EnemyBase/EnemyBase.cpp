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
	m_Kinds = Enemy;		//種類設定

	m_PlayerPos = VZERO;	//プレイヤー座標

	m_NextActionTime = 0;	//次の行動までの待機時間
	m_DownTime = 0;			//ダウン状態継続時間
}
