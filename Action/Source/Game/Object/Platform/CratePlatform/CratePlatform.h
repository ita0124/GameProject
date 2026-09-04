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
};


