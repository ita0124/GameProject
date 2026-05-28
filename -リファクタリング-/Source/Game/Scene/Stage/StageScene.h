#pragma once
#include "Game/Base/Scene/SceneBase.h"
#include "Game/Object/Sky/Sky.h"
#include "Game/Object/Actor/Character/Player/Player.h"
#include "Game/Object/Sword/Sword.h"
#include "Game/Object/Shield/Shield.h"
#include "Game/UI/HitPoints/HitPoints.h"
#include "Game/UI/SkillPoints/SkillPoints.h"
#include "Game/UI/Stamina/Stamina.h"
#include "Game/Manager/Platform/PlatformManager.h"
#include "Game/Manager/Camera/CameraManager.h"
#include "Game/HitCheck/HitCheck.h"

class StageScene :public SceneBase {
private:
	Sky				m_Sky;				//天球クラス
	Player			m_Player;			//プレイヤークラス
	Sword			m_Sword;			//剣クラス
	Shield			m_Shield;			//盾クラス
	HitPoints		m_HitPoints;		//体力UIクラス
	SkillPoints		m_SkillPoints;		//スキルポイントUIクラス
	Stamina			m_Stamina;			//スタミナUIクラス
	PlatformManager	m_PlatformManager;	//プラットフォームマネージャークラス
	CameraManager	m_CameraManager;	//カメラマネージャークラス
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
};
