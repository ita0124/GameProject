#pragma once
#include "Comon.h"

namespace {
	static constexpr int TAITLEHANDL_MAX = 3;
}

class Title {
private:
	int m_Hndl[TAITLEHANDL_MAX];

	int m_Time;

	int m_BlendParam;

	int m_BlendParamTime;

	bool m_IsUpBlendParam;
public:
	const char* FilePath1[TAITLEHANDL_MAX] = {
	"Data/Title/PushKeyorPad1.png",
	"Data/Title/PushKeyorPad2.png",
	"Data/Title/BATTLEANIMAL.png",
	};

	//コンストラクタ
	Title();

	//デストラクタ
	~Title();

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
