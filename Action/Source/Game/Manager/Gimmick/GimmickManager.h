#pragma once
#include "Game/Object/Gimmick/BarrierActivator/BarrierActivator.h"

namespace {
	constexpr int GIMMICK_MAX = 2;
	constexpr int GIMMICK_ID = 1;
	constexpr int GIMMICK_REQUEST_DATA_MAX = 13;
}

class GimmickManager {
public:
	enum TagMapKinds {
		MAP1,
		MAP2,

		MAP_NUM
	};

	GimmickBase* m_Gimmick[GIMMICK_MAX];
private:
	int		m_GimmickID[GIMMICK_MAX][GIMMICK_ID];							//出現させるモブ敵を指定するデータをcsvから受け取り保存する
	float	m_RequestData[GIMMICK_MAX][GIMMICK_REQUEST_DATA_MAX];			//リクエスト時に必要なデータをcsvから受け取り保存する
public:
	//初期化処理
	void Init(int _Map = 0);
	//データ破棄処理
	void Exit();
	//データ読み込み処理
	void Load();
	//描画処理
	void Draw();
	//毎フレーム呼び出す処理
	void Step(MobEnemyManager _MobEnemyManager, PlatformManager _PlatformManager);
	//モデル更新処理
	void Update();
	//当たり判定後の処理(当たっている場合)
	void HitCalc(const int& _Num, ObjectBase* _Object);
	//指定した配列番号のメンバ変数を返す
	GimmickBase& GetGimmick(const int& _Num) { return *m_Gimmick[_Num]; }
};