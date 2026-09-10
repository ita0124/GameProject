#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//木
class TreePlatform :public PlatformBase {
public:
	//コンストラクタ
	TreePlatform();
	//デストラクタ
	~TreePlatform();
	//初期化処理
	void Init();
	//毎フレーム呼び出す処理
	void Step();
};


