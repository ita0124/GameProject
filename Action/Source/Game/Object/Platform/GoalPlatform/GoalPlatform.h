#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//ゴール足場
class GoalPlatform :public PlatformBase {
private:
	ObjectBase *m_Object;
public:
	//コンストラクタ
	GoalPlatform();
	//デストラクタ
	~GoalPlatform();
	//初期化処理
	void Init();
	//毎フレーム呼び出す処理
	void Step();
	//当たり判定後の処理(当たっている場合)
	void HitCalc(ObjectBase* _Object);
};


