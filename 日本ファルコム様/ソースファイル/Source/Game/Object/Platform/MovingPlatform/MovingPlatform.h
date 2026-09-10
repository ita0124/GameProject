#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"
#include <vector>
#include <iterator>

//動く足場
class MovingPlatform :public PlatformBase {
public:
	//足場の状態を管理するタグ
	enum TagState {
		MOVE,				//移動中
		STOP,				//停止

		STATE_NUM
	};
private:
	TagState	m_State;			//状態変数

	bool		m_IsEndPos;			//終端座標にたどり着いたか
	VECTOR		m_MoveDir;			//移動方向ベクトル
	float		m_PrevLen;			//前フレームの座標から終端座標までの距離を保存する
	int			m_StopTime;			//到着したら一度止まる継続時間
	bool		m_IsHit;
	std::vector<ObjectBase*> m_Object;
public:
	//コンストラクタ
	MovingPlatform();
	//デストラクタ
	~MovingPlatform();
	//初期化処理
	void Init();
	// データ読み込み処理
	void Load(const int _Hndl);
	//毎フレーム呼び出す処理
	void Step();
	//当たり判定後の処理(当たっている場合)
	void HitCalc(ObjectBase* _Object);
	//当たり判定後の処理(当たっていない場合)
	void NotHitCalc(ObjectBase* _Object);

	//Get
	VECTOR GetMoveDir() { return m_MoveDir; }
};


