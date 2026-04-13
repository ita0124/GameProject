#pragma once
#include "Game/Base/UIBase/UIBase.h"

//ターゲットクラス
class Target:public UIBase {
public:
	//コンストラクタ
	Target();

	//デストラクタ
	 ~Target();

	//初期化処理
	void Init();

	//データ破棄処理
	void Exit();

	//データ読み込み処理
	void Load();

	//描画処理
	void Draw();

	//座標更新
	void Update(VECTOR _Pos);
};
