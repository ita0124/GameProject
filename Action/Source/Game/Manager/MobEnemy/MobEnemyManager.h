#pragma once
#include "Game/Object/Actor/Character/Enemy/Boar/Boar.h"
#include "Game/Object/Actor/Character/Enemy/Wolf/Wolf.h"

namespace {
	constexpr int MOB_ENEMY_MAX = 20;
	constexpr int MOB_ENEMY_ID = 1;
	constexpr int MOB_ENEMY_REQUEST_DATA_MAX = 12;
}

class MobEnemyManager {
private:
	MobEnemyBase* m_MobEnemy[MOB_ENEMY_MAX];

	int		m_MobEnemyID[MOB_ENEMY_MAX][MOB_ENEMY_ID];							//出現させるモブ敵を指定するデータをcsvから受け取り保存する
	float	m_RequestData[MOB_ENEMY_MAX][MOB_ENEMY_REQUEST_DATA_MAX];			//リクエスト時に必要なデータをcsvから受け取り保存する
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
	//当たり判定後の処理(当たっていない場合)
	void NotHitCalc(const int& _Num, ObjectBase* _Object);
	//指定した配列番号のメンバ変数を返す
	MobEnemyBase& GetMobEnemy(const int &_Num) { return *m_MobEnemy[_Num]; }
	//Set
	void SetPlayerPos(VECTOR _PlayerPos);
};