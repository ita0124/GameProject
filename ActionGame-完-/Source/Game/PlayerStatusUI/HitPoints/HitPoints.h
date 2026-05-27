#pragma once
#include "Game/Base/UIBase/PlayerStatusUIBase/PlayerStatusUIBase.h"

//体力UIクラス
class HitPoints :public PlayerStatusUIBase {
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
	void Update(int _Num);

	//描画処理
	void Draw();
};
