#pragma once
#include "Game/Base/Scene/SceneBase.h"
#include "Game/Result/ResultNum.h"
#include "Game/Result/Result.h"

class ResultScene :public SceneBase {
private:
	Result	m_Result;
	int m_ResultNum;
public:
	//コンストラクタ・デストラクタ
	ResultScene();
	~ResultScene();

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
