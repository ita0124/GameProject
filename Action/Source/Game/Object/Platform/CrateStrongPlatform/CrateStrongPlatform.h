#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//通常足場
class CrateStrongPlatform :public PlatformBase {
public:
	//コンストラクタ
	CrateStrongPlatform();
	//デストラクタ
	~CrateStrongPlatform();
	//初期化処理
	void Init();
	//毎フレーム呼び出す処理
	void Step();
};


