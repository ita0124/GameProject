#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//通常足場
class NormalPlatform :public PlatformBase {
public:
	//コンストラクタ
	NormalPlatform();
	//デストラクタ
	~NormalPlatform();
	//初期化処理
	void Init();
	//毎フレーム呼び出す処理
	void Step();
};


