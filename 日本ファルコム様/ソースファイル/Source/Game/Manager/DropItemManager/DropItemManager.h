#pragma once
#include "Game/Object/DropItem/DropCoin/DropCoin.h"
#include "Game/Object/DropItem/DropHitPoints/DropHitPoints.h"
#include "Game/Object/DropItem/DropSkillPoints/DropSkillPoints.h"

namespace {
	constexpr int DROP_ITEM_MAX = 60;
}

//ドロップアイテムマネージャー
class DropItemManager {
private:
	DropItemBase* m_DropItem[DROP_ITEM_MAX];
public:
	//初期化処理
	void Init();
	//データ破棄処理
	void Exit();
	//データ読み込み処理
	void Load();
	//描画処理
	void Draw();
	//毎フレーム呼び出す処理
	void Step();
	//モデル更新処理
	void Update();
	//当たり判定後の処理(当たっている場合)
	void HitCalc(const int& _Num, ObjectBase* _Object);
	//指定した配列番号のメンバ変数を返す
	DropItemBase& GetDropItem(const int& _Num) { return *m_DropItem[_Num]; }
};