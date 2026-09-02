#pragma once
#include "Game/Base/Object/ObjectBase.h"

//ステージベースクラス
class ItemBase :public ObjectBase {
public:
	enum TagState {
		IDEL,		//待機
		ORBIT,		//旋回
		DEATH,		//消滅

		STATE_NUM
	};
protected:
	TagState	m_State;			//状態変数
	bool		m_IsHit;

	float		m_OrbitAngle;		//現在の旋回角度
	float		m_OrbitTotalAngle;	//累積旋回角度
	float		m_OrbitCount;		//現在の周回数
	float		m_OrbitRadius;		//旋回半径

	//待機
	void Idel();
	//旋回
	void Orbit();
	//消滅
	void Death();
public:
	//コンストラクタ
	ItemBase();
	//デストラクタ
	~ItemBase();
	//初期化処理
	void Init();
	//毎フレーム呼び出す処理
	void Step();
	//当たり判定後の処理(当たっている場合)
	virtual void HitCalc(ObjectBase* _Owner);
};


