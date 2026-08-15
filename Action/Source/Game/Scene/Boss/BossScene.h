#pragma once
#include "Game/Base/Scene/SceneBase.h"
#include "Game/Object/BossArea/BossArea.h"
#include "Game/Object/Sky/Sky.h"
#include "Game/Object/Actor/Character/Player/Player.h"
#include "Game/Object/Actor/Character/Enemy/Boss/Boss.h"
#include "Game/Object/Sword/Sword.h"
#include "Game/Object/Shield/Shield.h"
#include "Game/Manager/Camera/CameraManager.h"
#include "Game/Manager/StatusDrawManager/StatusDrawManager.h"
#include "Game/Result/ResultNum.h"
#include "Game/HitCheck/HitCheck.h"

namespace {
	constexpr int HNDL_MAX = 4;	//非同期処理時に表示する画像のハンドル総数
}

class BossScene :public SceneBase {
private:
	BossArea			m_BossArea;				//ボス戦の足場クラス
	Sky					m_Sky;					//天球クラス
	Player				m_Player;				//プレイヤークラス
	Sword				m_Sword;				//剣クラス
	Shield				m_Shield;				//盾クラス
	Boss				m_Boss;					//ボスクラス
	CameraManager		m_CameraManager;		//カメラマネージャークラス
	StatusDrawManager	m_StatusDrawManager;	//ステータス描画マネージャー

	typedef struct {
		int		LoadTime;				//非同期処理継続時間
		float	HndlNum;				//非同期処理時に表示する画像
		int		Hndl[HNDL_MAX];			//画像ハンドル
		bool	IsLoadASync;			//非同期処理中か
	}LOAD_DATA;

	LOAD_DATA		m_Load;

	int m_ShadowHndl;					//影生成用のハンドル

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
	//コンストラクタ
	BossScene();
	//デストラクタ
	~BossScene();
	//シーン中繰り返し行う処理
	int	Loop() override;
	//描画処理管理関数
	void Draw() override;
private:
	//初期化処理管理関数
	void Init() override;
	//データ破棄処理管理関数
	void Exit() override;
	//データ読み込み処理管理関数
	void Load() override;
	//データ読み込み処理(非同期)
	bool LoadASync();
	//毎フレーム呼び出す処理管理関数
	int Step() override;
	//モデル更新処理
	void Update();
	//プレイヤー関連Step
	void PlayerStep();
	//敵関連Step
	void EnemyStep();
	//カメラ関連Step
	void CameraStep();
	//当たり判定関係
	void HitCheck();
};
