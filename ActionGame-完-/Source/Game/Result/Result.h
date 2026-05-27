#pragma once
#include "Comon.h"
#include "ResultNum.h"

namespace {
	static constexpr int RESULTHANDL_MAX = 5;
}

class Result {       
private:
	int m_Hndl[RESULTHANDL_MAX];

	int m_Time;
	int m_BlendParam;
	int m_BlendParamTime;
	bool m_IsUpBlendParam;
public:
	const char* FilePath1[RESULTHANDL_MAX] = {
	"Data/Result/PushKeyorPad1.png",
	"Data/Result/PushKeyorPad2.png",
	"Data/Result/GAMEOVER.png",
	"Data/Result/GAMECLEAR.png",
	"Data/Result/BackGround.png",
	};

	//コンストラクタ
	Result();

	//デストラクタ
	~Result();

	//初期化処理
	void Init();

	// データ読み込み処理
	void Load();

	//データ破棄処理
	void Exit();

	//描画処理
	void Draw();

	//毎フレーム行う処理
	void Step();
};
