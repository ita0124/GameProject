#include "Game/Manager/StatusDrawManager/StatusDrawManager.h"

namespace {
	constexpr float		HEAD_POS_Y_BASE = 12.0f;

	constexpr VECTOR	HIT_POINTS_HEAD_POS = { 48.0f,HEAD_POS_Y_BASE * 4.0f,0.0f };		//先頭座標
	constexpr int		HIT_POINTS_MAX = 100;												//最大HP

	constexpr VECTOR	SKILL_POINTS_HEAD_POS = { 48.0f,HEAD_POS_Y_BASE * 9.0f,0.0f };		//先頭座標
	constexpr int		SKILL_POINTS_MAX = 100;												//最大スキルポイント

	constexpr VECTOR	STAMINA_HEAD_POS = { 48.0f,HEAD_POS_Y_BASE * 14.0f,0.0f };			//先頭座標
	constexpr int		STAMINA_MAX = 100;													//最大スタミナ


	constexpr  char		UI_FRAMEFILE_PATH[] = { "Data/UI/Frame.png" };						//画像パス

	constexpr VECTOR	FRAME_HERAD_POS = { 0.0f,HEAD_POS_Y_BASE * 0.0f,0.0f };				//フレーム先頭座標

	constexpr VECTOR	FRAME_SCALE = { 1.0f,1.0f,1.0f };									//フレーム画像スケール

	constexpr int		UI_FRAME_GRAPHICS_SIZEX = 24 * (int)FRAME_SCALE.x;						//画像サイズ(X軸)
	constexpr int		UI_FRAME_GRAPHICS_SIZEY = 24 * (int)FRAME_SCALE.y;						//画像サイズ(Y軸)

}

//コンストラクタ
StatusDrawManager::StatusDrawManager() {
	Init();
}
//デストラクタ
StatusDrawManager::~StatusDrawManager() {
	Exit();
}
//初期化処理
void StatusDrawManager::Init(CharacterBase* _Owner) {
	//オーナー変数がnullなら
	if (m_Owner == nullptr) {
		m_Owner = _Owner;											//オーナーを設定
	}
	m_HitPoints.Init(HIT_POINTS_HEAD_POS, HIT_POINTS_MAX);			//体力
	m_SkillPoints.Init(SKILL_POINTS_HEAD_POS, SKILL_POINTS_MAX);	//スキルポイント
	m_Stamina.Init(STAMINA_HEAD_POS, STAMINA_MAX);					//スタミナ

	m_Frame.Init();
	m_Frame.SetPos(FRAME_HERAD_POS);
	m_Frame.SetScale(FRAME_SCALE);
}
//データ破棄処理
void StatusDrawManager::Exit() {
	m_HitPoints.Exit();
	m_SkillPoints.Exit();
	m_Stamina.Exit();
	m_Frame.Exit();
}
//データ読み込み処理
void StatusDrawManager::Load() {
	m_HitPoints.Load();
	m_SkillPoints.Load();
	m_Stamina.Load();
	m_Frame.Load(UI_FRAMEFILE_PATH, 1, 1, 1, UI_FRAME_GRAPHICS_SIZEX, UI_FRAME_GRAPHICS_SIZEY);
}
//描画処理
void StatusDrawManager::Draw() {
	//まずはここでアルファ値をセット
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	//m_Frame.Draw();
	//鉾の画像に影響を出さないよう、初期化設定に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	m_HitPoints.Draw();
	m_SkillPoints.Draw();
	m_Stamina.Draw();
}
//ステータスを取得
void StatusDrawManager::SetStatus() {
	float	HitPoints = m_Owner->GetHitPoints();
	float	SkillPoints = (float)m_Owner->GetSkillPoints();
	float	Stamina = m_Owner->GetStamina();

	m_HitPoints.SetStatus(HitPoints);
	m_SkillPoints.SetStatus(SkillPoints);
	m_Stamina.SetStatus(Stamina);
}
//初期化処理
void StatusDrawManager::Init() {
	m_Owner = nullptr;
}
