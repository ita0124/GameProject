#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//落ちる足場
class FallingPlatform :public PlatformBase {
public:
	//足場の状態を管理するタグ
	enum TagState {
		NORMAL,				//通常
		FALL,				//落下
		DEATH,				//消滅

		STATE_NUM
	};
private:
	TagState	m_State;			//状態変数

	int			m_FallWait;			//落ち始めるまでの待機時間
	int			m_FallingTime;		//落ち続ける時間
	int			m_RespawnWait;		//再生成までの時間
	bool		m_IsHit;			//乗られたか
	VECTOR		m_FirstPos;			//初期座標
	ObjectBase* m_Object;
public:
	//コンストラクタ
	FallingPlatform();
	//デストラクタ
	~FallingPlatform();
	//初期化処理
	void Init();
	//データ読み込み処理
	void Load();
	//毎フレーム呼び出す処理
	void Step();
	//当たり判定後の処理(当たっている場合)
	void HitCalc(ObjectBase* _Object);
};


