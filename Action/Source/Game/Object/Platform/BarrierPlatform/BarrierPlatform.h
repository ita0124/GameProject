#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//通常足場
class BarrierPlatform :public PlatformBase {
public:
	//コンストラクタ
	BarrierPlatform();
	//デストラクタ
	~BarrierPlatform();
	//初期化処理
	void Init();
	//データ読み込み処理
	void Load();
	//毎フレーム呼び出す処理
	void Step();
	//描画処理
	void Draw();
};


