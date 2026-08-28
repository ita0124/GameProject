#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//塀足場
class FenceBrokenPlatform :public PlatformBase {
public:
	//コンストラクタ
	FenceBrokenPlatform();
	//デストラクタ
	~FenceBrokenPlatform();
	//初期化処理
	void Init();
	//毎フレーム呼び出す処理
	void Step();
};
