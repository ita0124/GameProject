#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//バリアクラス
class BarrierPlatform :public PlatformBase {
public:
	//コンストラクタ
	BarrierPlatform();
	//デストラクタ
	~BarrierPlatform();
	//初期化処理
	void Init();
	//毎フレーム呼び出す処理
	void Step();
	//描画処理
	void Draw();
};


