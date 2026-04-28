#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//落ちる足場
class FallingPlatform :public PlatformBase {
private:

public:
	//コンストラクタ
	FallingPlatform();
	//デストラクタ
	~FallingPlatform();
	//初期化処理
	void Init();
	//データ読み込み処理
	void Load();
	//毎フレーム呼び出す処理
	void Step();
};


