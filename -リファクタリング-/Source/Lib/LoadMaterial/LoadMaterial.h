#pragma once

#include <DxLib.h>

class LoadMaterial {
public:
	static int MATERIAL_BLACK;
	static int MATERIAL_BLUE;
	static int MATERIAL_RED;
	static int MATERIAL_GRADATION_BLUE;
	static int MATERIAL_GRADATION_RED;
	static int MATERIAL_GRADATION_YELLOW;
	static int MATERIAL_DAMAGE;
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
