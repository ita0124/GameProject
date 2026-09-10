#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//矢印
class ArrowPlatform :public PlatformBase {
public:
	//コンストラクタ
	ArrowPlatform();
	//デストラクタ
	~ArrowPlatform();
	//初期化処理
	void Init();
	//毎フレーム呼び出す処理
	void Step();
};


