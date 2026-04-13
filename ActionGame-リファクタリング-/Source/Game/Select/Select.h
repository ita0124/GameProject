#pragma once
#include "Comon.h"

namespace {
	static const int SELECT_MAX = 0;

	static const int SELECTHANDL_MAX = 3;
}

class Select {
private:
	static int m_Hndl[SELECTHANDL_MAX];
public:
	//初期化処理
	static void Init();
	//データ読み込み処理
	static void Load();
	//データ破棄処理
	static void Exit();
	//描画処理
	static void Draw();
	//毎フレーム呼び出す処理
	static void Step();
};
