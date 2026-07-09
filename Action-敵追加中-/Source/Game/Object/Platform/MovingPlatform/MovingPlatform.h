#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//動く足場
class MovingPlatform :public PlatformBase {
private:
	bool	m_IsEndPos;	//終端座標にたどり着いたか
	VECTOR	m_MoveDir;	//移動方向ベクトル
	float	m_PrevLen;	//前フレームの座標から終端座標までの距離を保存する
	bool	m_IsHit;
	ObjectBase *m_Object;
public:
	//コンストラクタ
	MovingPlatform();
	//デストラクタ
	~MovingPlatform();
	//初期化処理
	void Init();
	//データ読み込み処理
	void Load();
	//毎フレーム呼び出す処理
	void Step();
	//当たり判定後の処理(当たっている場合)
	void HitCalc(ObjectBase* _Object);
	//当たり判定後の処理(当たっていない場合)
	void NotHitCalc(ObjectBase* _Object);

	//Get
	VECTOR GetMoveDir() { return m_MoveDir; }
};


