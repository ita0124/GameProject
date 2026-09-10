#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//花
class GrassPlatform :public PlatformBase {
public:
	//コンストラクタ
	GrassPlatform();
	//デストラクタ
	~GrassPlatform();
	//初期化処理
	void Init();
	//毎フレーム呼び出す処理
	void Step();
};


