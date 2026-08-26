#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//壁クラス
class WallPlatform :public PlatformBase {
public:
	//コンストラクタ
	WallPlatform();
	//デストラクタ
	~WallPlatform();
	//初期化処理
	void Init();
	//毎フレーム呼び出す処理
	void Step();
};


