#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//通常足場
class MovingPlatform :public PlatformBase {
public:
	//コンストラクタ
	MovingPlatform();
	//デストラクタ
	~MovingPlatform();
	//初期化処理
	void Init();
	//データ読み込み処理
	void Load();
	//毎フレーム呼び出す処理
	void Step();
};


