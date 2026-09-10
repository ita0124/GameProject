#pragma once
#include "Game/Base/Object/DropItemBase/DropItemBase.h"
#include "Lib/LoadMaterial/LoadMaterial.h"

//ドロップ体力回復
class DropHitPoints :public DropItemBase {
private:
	//消滅
	void Death();
public:
	//コンストラクタ
	DropHitPoints();
	//デストラクタ
	~DropHitPoints();
	//初期化処理
	void Init();
	//当たり判定後の処理(当たっている場合)
	void HitCalc(ObjectBase* _Owner);
	//毎フレーム呼び出す処理
	void Step();
};
