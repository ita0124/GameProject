#pragma once
#include "Game/Base/Object/StageBase/StageBase.h"

//通常足場
class JumpPlatform :public StageBase {
public:
	//コンストラクタ
	JumpPlatform();
	//デストラクタ
	~JumpPlatform();
	//初期化処理
	void Init();
	//データ読み込み処理
	void Load();
	//毎フレーム呼び出す処理
	void Step();
};


