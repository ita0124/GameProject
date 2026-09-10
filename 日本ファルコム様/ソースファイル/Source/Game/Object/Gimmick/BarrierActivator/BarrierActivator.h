#pragma once
#include "Game/Base/Object/GimmickBase/GimmickBase.h"

//通常足場
class BarrierActivator :public GimmickBase {
private:
	bool		m_IsHit;			//乗られたか
public:
	//コンストラクタ
	BarrierActivator();
	//デストラクタ
	~BarrierActivator();
	//初期化処理
	void Init();
	//データ読み込み処理
	void Load();
	//毎フレーム呼び出す処理
	void Step(MobEnemyManager _MobEnemyManager, PlatformManager _PlatformManager);
	//当たり判定後の処理(当たっている場合)
	void HitCalc(ObjectBase* _Object);
};


