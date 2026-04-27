#pragma once
#include "Game/Object/Platform/NormalPlatform/NormalPlatform.h"
#include "Game/Object/Platform/MovingPlatform/MovingPlatform.h"
#include "Game/Object/Platform/FallingPlatform/FallingPlatform.h"
#include "Game/Object/Platform/BattlePlatform/BattlePlatform.h"
#include "Game/Object/Platform/JumpPlatform/JumpPlatform.h"

namespace {
	const int PLATFORM_MAX = 10;
	const int PLATFORM_ID = 1;
	const int REQUEST_DATA_MAX = 12;
}

class PlatformManager {
private:
	//プラットフォームの呼び出し
	enum TagPlatform {
		NORMAL,			//通常足場
		MOVING,			//動く足場
		FALLING,		//落ちる足場
		BATTLE,			//バトル足場
		JUMP,			//ジャンプ足場
	};

	PlatformBase* m_Platform[PLATFORM_MAX];
	
	int		m_PlatformID[PLATFORM_MAX][PLATFORM_ID];
	float	m_RequestData[PLATFORM_MAX][REQUEST_DATA_MAX];
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
	//リクエスト
	void Request();
};