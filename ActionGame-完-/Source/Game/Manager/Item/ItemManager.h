#pragma once
#include "Game/Item/HP/HPItem.h"
#include "Game/Item/Stamina/StaminaItem.h"
#include "Lib/Input/InputKey.h"
#include "Lib/Input/InputPad.h"

//アイテムマネーシャークラス
class ItemManager {
private:
	//シーンを管理するタグ
	enum TagItem {
		HP,
		STAMINA,

		SCENE_NUM
	};

	//
	TagItem m_ID;
	//
	ItemBase* m_Item[SCENE_NUM];

public:
	//コンストラクタ
	ItemManager();

	//デストラクタ
	~ItemManager();

	//初期化処理
	void Init();

	// データ読み込み処理
	void Load();

	//データ破棄処理
	void Exit();

	//実行中繰り返し行われる処理管理関数
	void Step(Player& _Player);

	//描画処理管理
	void Draw();
};
