#pragma once

#include "Game/Base/Item/ItemBase.h"

class HPItem :public ItemBase {
private:


public:
	//コンストラクタ
	HPItem();

	//デストラクタ
	~HPItem();

	//初期化処理
	void Init();
	
	// データ読み込み処理
	void Load();

	//アイテム使用処理
	void Use(Player& _Player);

	//描画処理
	void Draw();
};
