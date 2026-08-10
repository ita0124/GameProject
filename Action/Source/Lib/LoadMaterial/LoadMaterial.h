#pragma once

#include <DxLib.h>

class LoadMaterial {
public:
	static int MATERIAL_BLACK;
	static int MATERIAL_BLUE;
	static int MATERIAL_RED;
	static int MATERIAL_GRAY;

	//コンストラクタ・デストラクタ
	LoadMaterial();
	~LoadMaterial();

	//初期化
	static void Init();

	//データ読み込み
	static void Load();

	//データ破棄
	static void Exit();
};
