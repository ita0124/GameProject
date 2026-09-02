#pragma once
#include "Game/Base/Object/ObjectBase.h"

//ステージベースクラス
class DropItemBase :public ObjectBase {
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
	float		m_OrbitRadius;		//旋回半径
	float		m_JumpPower;		//ジャンプ力計算

	//待機
	void Idel();
	//旋回
	void Orbit();
	//消滅
	void Death();
public:
	//コンストラクタ
	DropItemBase();
	//デストラクタ
	~DropItemBase();
	//初期化処理
	void Init();
	// データ読み込み処理
	void Load(const int _Hndl);
	//毎フレーム呼び出す処理
	void Step();
	//当たり判定後の処理(当たっている場合)
	virtual void HitCalc(ObjectBase* _Owner);
	//重力処理
	void GravityManager();
	//リクエスト
	bool Request(const VECTOR& _Pos);
};


