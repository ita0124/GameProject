#pragma once
#include "Game/Base/Object2D/Object2DBase.h"

class HitPoints :public Object2DBase {
public:
	//コンストラクタ
	HitPoints();
	//デストラクタ
	~HitPoints();
	//初期化処理
	void Init();
	//データ破棄処理
	void Exit();
	//データ読み込み処理
	void Load();
	//数値更新
	void Step();
	//描画処理
	void Draw();
};

