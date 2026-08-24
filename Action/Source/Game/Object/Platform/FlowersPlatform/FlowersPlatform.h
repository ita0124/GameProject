#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//花
class FlowersPlatform :public PlatformBase {
public:
	//コンストラクタ
	FlowersPlatform();
	//デストラクタ
	~FlowersPlatform();
	//初期化処理
	void Init();
	//データ読み込み処理
	void Load();
	//毎フレーム呼び出す処理
	void Step();
};


