#pragma once
#include "Game/Object/Actor/Character/Enemy/Boss/Boss.h"
#include "Game/Object/Actor/Crystal/Crystal.h"

namespace {
	constexpr int CRYSTAL_MAX = 4;
}

//Bossが使うObjectを管理するクラス
class BossGimmickManager {
private:
	Boss*	m_Owner;						//オーナーボス
	Crystal	m_Crystal[CRYSTAL_MAX];			//クリスタル

public:
	//コンストラクタ
	BossGimmickManager();
	//デストラクタ
	~BossGimmickManager();
	//初期化処理
	void Init(Boss* _Owner);
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
	//リクエスト処理
	void Request();
	//指定した配列番号のメンバ変数を返す
	Crystal& GetCrystal(const int& _Num) { return m_Crystal[_Num]; }
private:
	//初期化処理
	void Init();
};

