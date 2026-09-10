#pragma once
#include <DxLib.h>
#include <vector>
#include <iterator>

class LoadMaterial {
public:
	enum TagMaterial {
		MATERIAL_BLACK,
		MATERIAL_BLUE,
		MATERIAL_RED,
		MATERIAL_BEFORE_RESPAWN,
		MATERIAL_AFTER_RESPAWN,
		MATERIAL_GRADATION_BLUE,
		MATERIAL_GRADATION_YELLOW,
		MATERIAL_GRADATION_RED,

		MATERIAL_MAX
	};

	static std::vector<int>m_Hndl;	//音楽ハンドル

	//コンストラクタ・デストラクタ
	LoadMaterial();
	~LoadMaterial();

	//初期化
	static void Init();

	//データ読み込み
	static void Load();

	//データ破棄
	static void Exit();

	//Get
	static int  GetHndl(int _Hndl) { return m_Hndl[_Hndl]; }
};
