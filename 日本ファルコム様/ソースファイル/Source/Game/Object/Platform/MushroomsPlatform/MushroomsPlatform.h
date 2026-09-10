#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//キノコ
class MushroomsPlatform :public PlatformBase {
public:
	//コンストラクタ
	MushroomsPlatform();
	//デストラクタ
	~MushroomsPlatform();
	//初期化処理
	void Init();
	//毎フレーム呼び出す処理
	void Step();
};


