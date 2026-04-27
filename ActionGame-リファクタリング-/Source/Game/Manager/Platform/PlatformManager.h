#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

namespace {
	const int PLATFORM_MAX = 10;
}

class PlatformManager {
private:
	enum PLATFORM_ID {
		NORMAL,
		MOVING,
		FALLINP,
		BATTLE,
		JUMP,

		ID_NUM
	};

	/*vector<PlatformBase>* m_PlatformBase;*/
	PlatformBase* m_PlatformBase[PLATFORM_MAX];

	int Stage[13][PLATFORM_MAX];
public:
	//コンストラクタ
	PlatformManager();
	//デストラクタ
	~PlatformManager();
	//初期化処理
	void Init();
	//データ破棄処理
	void Exit();
	// データ読み込み処理
	void Load();
	//モデル更新処理
	void Update();
	//描画処理
	void Draw();
	//毎フレーム呼び出す処理
	void Step();
	//リクエスト
	void Request();
};
