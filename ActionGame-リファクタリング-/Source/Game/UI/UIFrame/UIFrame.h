#pragma once
#include "Game/Base/UIBase/UIBase.h"

//UIフレームクラス
class UIFrame :public UIBase {
public:
	//コンストラクタ・デストラクタ
	UIFrame();
	~UIFrame();

	//初期化処理
	void Init();

	//データ破棄処理
	void Exit();

	//データ読み込み処理
	void Load();

	//描画処理
	void Draw();
};
