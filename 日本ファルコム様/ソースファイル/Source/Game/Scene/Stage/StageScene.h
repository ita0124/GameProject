#pragma once
#include "Game/Base/Scene/SceneBase.h"
#include "Game/Object/Sky/Sky.h"
#include "Game/Object/Actor/Character/Player/Player.h"
#include "Game/Object/Sword/Sword.h"
#include "Game/Object/Shield/Shield.h"
#include "Game/Manager/Platform/PlatformManager.h"
#include "Game/Manager/MobEnemy/MobEnemyManager.h"
#include "Game/Manager/Gimmick/GimmickManager.h"
#include "Game/Manager/Camera/CameraManager.h"
#include "Game/Manager/StatusDrawManager/StatusDrawManager.h"
#include "Game/Manager/DropItemManager/DropItemManager.h"
#include "Game/HitCheck/HitCheck.h"
#include "Game/Result/ResultNum.h"
#include "Game/System/Coin/Coin.h"


class StageScene :public SceneBase {
private:
	Sky					m_Sky;					//天球クラス
	Player				m_Player;				//プレイヤークラス
	Sword				m_Sword;				//剣クラス
	Shield				m_Shield;				//盾クラス
	PlatformManager		m_PlatformManager;		//プラットフォームマネージャークラス
	MobEnemyManager		m_MobEnemyManager;		//モブ敵マネージャークラス
	GimmickManager		m_GimmickManager;		//ギミックマネージャークラス
	CameraManager		m_CameraManager;		//カメラマネージャークラス
	StatusDrawManager	m_StatusDrawManager;	//ステータス描画マネージャー
	DropItemManager		m_DropItemManager;		//ドロップアイテムマネージャー

	int m_ShadowHndl;					//影生成用のハンドル

	int m_Result;

	//ゲーム内の状態管理
	enum TagGameState {
		GAME_RESET,
		GAME_START_WAIT,
		GAME_STEP,
		GAME_RESET_WAIT,
	};
	//ゲーム内の状態管理
	TagGameState m_GameID;
public:
	//コンストラクタ・デストラクタ
	StageScene();
	~StageScene();

	//シーン中繰り返し行う処理
	int Loop() override;
	//描画処理管理関数
	void Draw() override;

private:
	//初期化処理管理関数
	void Init() override;
	//データ破棄処理管理関数
	void Exit() override;
	//データ読み込み処理管理関数
	void Load() override;
	//毎フレーム呼び出す処理管理関数
	int Step() override;
	//モデル更新処理
	void Update();
	//カメラ関連Step
	void CameraStep();
	//プレイヤー関連Step
	void PlayerStep();
	//ステータス関連Step
	void StatusStep();
};
