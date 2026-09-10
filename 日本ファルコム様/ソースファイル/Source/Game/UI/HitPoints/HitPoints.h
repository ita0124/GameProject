#pragma once
#include "Game/Base/Object2D/StatusDraw/StatusDrawBase.h"

//体力UIクラス
class HitPoints :public StatusDrawBase {
public:
	//コンストラクタ
	HitPoints();
	//デストラクタ
	~HitPoints();
	//初期化処理
	void Init(VECTOR _Pos,float _MaxStatus);
	//データ読み込み処理
	void Load();
	//描画処理
	void Draw();
private:
	//初期化処理
	void Init();
};

