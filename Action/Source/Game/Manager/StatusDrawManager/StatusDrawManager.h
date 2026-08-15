#pragma once
#include "Game/Base/Object/ActorBase/Character/CharacterBase.h"
#include "Game/UI/HitPoints/HitPoints.h"
#include "Game/UI/SkillPoints/SkillPoints.h"
#include "Game/UI/Stamina/Stamina.h"

namespace {
	constexpr int UI_FRAME_MAX = 3;
}

//ステータス描画マネージャー
class StatusDrawManager {
private:
	CharacterBase*	m_Owner;				//オーナーキャラクター
	HitPoints		m_HitPoints;			//体力
	SkillPoints		m_SkillPoints;			//スキルポイント
	Stamina			m_Stamina;				//スタミナ
	Object2DBase	m_Frame[UI_FRAME_MAX];	//UIフレーム

public:
	//コンストラクタ
	StatusDrawManager();
	//デストラクタ
	~StatusDrawManager();
	//初期化処理
	void Init(CharacterBase* _Owner);
	//データ破棄処理
	void Exit();
	//データ読み込み処理
	void Load();
	//描画処理
	void Draw();
	//ステータスを取得
	void SetStatus();
private:
	//初期化処理
	void Init();
};
