#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//通常足場
class CratePlatform :public PlatformBase {
public:
	//コンストラクタ
	CratePlatform();
	//デストラクタ
	~CratePlatform();
	//初期化処理
	void Init();
	//毎フレーム呼び出す処理
	void Step();
	//当たり判定後の処理(当たっている場合)
	void HitCalc(ObjectBase* _Object);
};


