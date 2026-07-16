#pragma once
#include "Game/Base/Object/ActorBase/Character/CharacterBase.h"
#include "Lib/LoadMaterial/LoadMaterial.h"

//エネミーベースクラス
class EnemyBase :public CharacterBase {
protected:
	VECTOR	m_PlayerPos;		//プレイヤー座標

	int		m_NextActionTime;	//次の行動までの待機時間
	int		m_DownTime;			//ダウン状態継続時間

public:
	//コンストラクタ
	EnemyBase();
	//デストラクタ
	~EnemyBase();
	//初期化処理
	virtual void Init();

	//Set
	//プレイヤー座標
	void SetPlayerPos(VECTOR _PlayerPos) { m_PlayerPos = _PlayerPos; }
};
