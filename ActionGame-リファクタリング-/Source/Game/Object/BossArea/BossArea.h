#pragma once
#include "Game/Base/Object/ObjectBase.h"

//ボス戦の足場クラス
class BossArea :public ObjectBase {
public:
	//コンストラクタ
	BossArea();
	//デストラクタ
	~BossArea();
	//初期化処理
	void Init();
	//データ破棄処理
	void Exit();
	//データ読み込み処理
	void Load();
	//毎フレーム呼び出す処理
	void Step();
};
