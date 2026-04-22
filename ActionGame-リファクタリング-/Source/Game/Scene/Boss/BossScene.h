#pragma once
#include "Game/Base/Scene/SceneBase.h"
#include "Game/Object/BossArea/BossArea.h"
#include "Game/Object/Sky/Sky.h"
#include "Game/Object/Actor/Character/Player/Player.h"
#include "Game/Object/Actor/Character/Enemy/Boss/Boss.h"
#include "Game/Manager/Camera/CameraManager.h"
#include "Game/Manager/Item/ItemManager.h"
#include "Game/PlayerStatusUI/HitPoints/HitPoints.h"
#include "Game/PlayerStatusUI/SkillPoints/SkillPoints.h"
#include "Game/PlayerStatusUI/Stamina/Stamina.h"
#include "Game/UI/Target/Target.h"
#include "Game/UI/UIFrame/UIFrame.h"
#include "Game/Result/ResultNum.h"
#include "Game/HitCheck/HitCheck.h"


namespace {
	const int HNDL_MAX = 4;	//非同期処理時に表示する画像のハンドル総数
}

class BossScene :public SceneBase {
private:
	BossArea		m_BossArea;			//ボス戦の足場クラス
	Sky				m_Sky;				//天球クラス
	Player			m_Player;			//プレイヤークラス
	Boss			m_Boss;
	CameraManager	m_CameraManager;	//カメラマネージャークラス
	ItemManager		m_ItemManger;		//アイテムマネーシャークラス
	HitPoints		m_HitPoints;		//体力UIクラス
	SkillPoints		m_SkillPoints;		//スキルポイントUIクラス
	Stamina			m_Stamina;			//スタミナUIクラス
	Target			m_Traget;			//ターゲットクラス
	UIFrame			m_UIFrame;			//UIフレームクラス

	typedef struct {
		int		LoadTime;				//非同期処理継続時間
		float	HndlNum;				//非同期処理時に表示する画像
		int		Hndl[HNDL_MAX];			//画像ハンドル
		bool	IsLoadASync;			//非同期処理中か
	}LOAD_DATA;

	LOAD_DATA		m_Load;
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
