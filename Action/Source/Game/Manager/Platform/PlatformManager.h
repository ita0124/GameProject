#pragma once
#include "Game/Object/Platform/NormalPlatform/NormalPlatform.h"
#include "Game/Object/Platform/MovingPlatform/MovingPlatform.h"
#include "Game/Object/Platform/FallingPlatform/FallingPlatform.h"
#include "Game/Object/Platform/WallPlatfprm/WallPlatfprm.h"
#include "Game/Object/Platform/RespawnPlatform/RespawnPlatform.h"
#include "Game/Object/Platform/GoalPlatform/GoalPlatform.h"

namespace {
	constexpr int PLATFORM_MAX = 100;
	constexpr int PLATFORM_ID = 1;
	constexpr int PLATFORM_REQUEST_DATA_MAX = 16;
}

class PlatformManager {
public:
	enum TagMapKinds {
		MAP1,
		MAP2,

		MAP_NUM
	};
private:
	PlatformBase* m_Platform[PLATFORM_MAX];

	int		m_PlatformID[PLATFORM_MAX][PLATFORM_ID];						//出現させるプラットフォームを指定するデータをcsvから受け取り保存する
	float	m_RequestData[PLATFORM_MAX][PLATFORM_REQUEST_DATA_MAX];			//リクエスト時に必要なデータをcsvから受け取り保存する
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
	void Step();
	//モデル更新処理
	void Update();
	//当たり判定後の処理(当たっている場合)
	void HitCalc(const int& _Num, ObjectBase* _Object);
	//当たり判定後の処理(当たっていない場合)
	void NotHitCalc(const int& _Num, ObjectBase* _Object);
	//指定した配列番号のメンバ変数を返す
	PlatformBase& GetPlatform(const int& _Num) { return *m_Platform[_Num]; }
};