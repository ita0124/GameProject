#pragma once
#include "Game/Base/Scene/SceneBase.h"
#include "Game/Select/Select.h"
#include "Game/Object/Sky/Sky.h"

class SelectScene :public SceneBase {
private:
	Sky m_Sky;

	int m_SelectNum;
public:
	//コンストラクタ・デストラクタ
	SelectScene();
	~SelectScene();

	//シーン中繰り返し行う処理
	int Loop() override;

	//描画処理管理関数
	void Draw() override;

private:
	//初期化処理管理関数
	void Init() override;

	//データ破棄処理管理関数
	void Exit() override;

	//データ読み込み処理管理関数
	void Load() override;

	//毎フレーム呼び出す処理管理関数
	int Step() override;
};
