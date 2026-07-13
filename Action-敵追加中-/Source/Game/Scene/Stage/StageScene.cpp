#include "StageScene.h"

namespace {
	constexpr int	FADE_SPEED = 5;					//フェードスピード
	constexpr int	PLAYER_RESPAWN_FADE_SPEED = 5;	//リスポーン完了フェードスピード
	constexpr int	PLAYER_DEATH_FADE_SPEED = 5;	//プレイヤー落下フェードスピード
}

//コンストラクタ
StageScene::StageScene() {
	//タグをINITに設定
	m_ID = INIT;
	//ゲーム内の状態を設定
	m_GameID = GAME_STEP;
	//影生成用のハンドル
	m_ShadowHndl = MakeShadowMap(4096, 4096);
	//影用ライト
	SetShadowMapLightDirection(m_ShadowHndl, { 0.5f, -0.5f, 0.0f });
}
//デストラクタ
StageScene::~StageScene() {
	//データ破棄処理管理関数呼び出す
	Exit();
}
//シーン中繰り返し行う処理
int StageScene::Loop() {
	int Res = 0;

	switch (m_ID) {
		//初期化処理を行う
	case INIT:
		//初期化処理管理関数を呼び出す
		Init();
		//タグをLOADに設定
		m_ID = LOAD;
		break;

		//データ読み込み処理を行う
	case LOAD:
		//データ読み込み処理管理関数を呼び出す
		Load();
		//タグをSTARTWAITに設定
		m_ID = STARTWAIT;
		break;

		//フェードイン処理を確実に行う	BGM再生なども
	case  STARTWAIT:
		//フェードイン関数を呼び出す
		Fade::RequestIn(FADE_SPEED);
		//フェードインが終わったら
		if (Fade::IsEndIn()) {
			//タグをSTEPに設定
			m_ID = STEP;
			////BGMを呼び出す
			//SoundManager::Play(SoundManager::TagID::BGM_TITLE, DX_PLAYTYPE_LOOP);
		}
		break;

		//毎フレーム呼び出す処理を行う
	case STEP:
		//毎フレーム呼び出す処理を行う
		//0以外が帰ってきたら
		if (Step() != 0) {
			//タグをENDWAITに設定
			m_ID = ENDWAIT;
		}
		break;

		//フェードアウトを確実に行う
	case ENDWAIT:
		//フェードアウト関数を呼び出す
		Fade::RequestOut(FADE_SPEED);
		//フェードアウトが終わったら
		if (Fade::IsEndOut()) {
			//タグをENDに設定
			m_ID = END;
		}
		break;

		//データ破棄を行う
	case END:
		//音楽停止
		SoundManager::AllStop();
		//データ破棄処理管理関数を呼び出す
		Exit();
		//タグをINITに設定
		m_ID = INIT;

		Res = 1;
		break;
	}
	//0以外の数値が入ると次のシーンへ以降する
	return Res;
}
//描画処理管理関数
void StageScene::Draw() {
	switch (m_ID) {
	case STARTWAIT:
		m_Sky.Draw();								//天球クラス
		m_PlatformManager.Draw();					//プラットフォームマネージャークラス
		break;
	case STEP:
	case ENDWAIT:
		m_Sky.Draw();								//天球クラス
		m_PlatformManager.Draw();					//プラットフォームマネージャークラス

		//影生成セットアップ
		ShadowMap_DrawSetup(m_ShadowHndl);
		m_Player.Draw();							//プレイヤークラス
		m_Sword.Draw();								//剣クラス
		m_Shield.Draw();							//盾クラス
		m_Boar.Draw();								//イノシシクラス
		m_Wolf.Draw();								//オオカミクラス
		//影設定終了
		ShadowMap_DrawEnd();
		//影生成
		SetUseShadowMap(0, m_ShadowHndl);
		m_PlatformManager.Draw();					//プラットフォームマネージャークラス
		m_Player.Draw();							//プレイヤークラス
		m_Sword.Draw();								//剣クラス
		m_Shield.Draw();							//盾クラス
		m_Boar.Draw();								//イノシシクラス
		m_Wolf.Draw();								//オオカミクラス
		//影生成終了
		SetUseShadowMap(0, -1);

		m_HitPoints.Draw();							//体力UIクラス
		m_SkillPoints.Draw();						//スキルポイントUIクラス
		m_Stamina.Draw();							//スタミナUIクラス

		m_CameraManager.Draw();
		break;
	}
}
//初期化処理管理関数
void StageScene::Init() {
	m_Sky.Init();									//天球クラス
	m_Player.Init();								//プレイヤークラス
	//オーナーを設定
	m_Sword.Init(&m_Player);						//剣クラス
	//オーナーを設定
	m_Shield.Init(&m_Player);						//盾クラス
	m_Boar.Init();									//イノシシクラス
	m_Wolf.Init();									//オオカミクラス
	m_HitPoints.Init();								//体力UIクラス
	m_SkillPoints.Init();							//スキルポイントUIクラス
	m_Stamina.Init();								//スタミナUIクラス
	m_PlatformManager.Init();						//プラットフォームマネージャークラス
	m_CameraManager.Init();							//カメラマネージャークラス
}
//データ破棄処理管理関数
void StageScene::Exit() {
	m_Sky.Exit();									//天球クラス
	m_Player.Exit();								//プレイヤークラス
	m_Sword.Exit();									//剣クラス
	m_Shield.Exit();								//盾クラス
	m_Boar.Exit();									//イノシシクラス
	m_Wolf.Exit();									//オオカミクラス
	m_HitPoints.Exit();								//体力UIクラス
	m_SkillPoints.Exit();							//スキルポイントUIクラス
	m_Stamina.Exit();								//スタミナUIクラス
	m_PlatformManager.Exit();						//プラットフォームマネージャークラス

	DeleteShadowMap(m_ShadowHndl);
}
//データ読み込み処理管理関数
void StageScene::Load() {
	m_Sky.Load();									//天球クラス
	m_Player.Load();								//プレイヤークラス
	m_Sword.Load();									//剣クラス
	m_Shield.Load();								//盾クラス
	m_Boar.Load();									//イノシシクラス
	m_Wolf.Load();									//オオカミクラス
	m_HitPoints.Load();								//体力UIクラス
	m_SkillPoints.Load();							//スキルポイントUIクラス
	m_Stamina.Load();								//スタミナUIクラス
	m_PlatformManager.Load();						//プラットフォームマネージャークラス
}
//毎フレーム呼び出す処理管理関数
int StageScene::Step() {
	int Res = 0;

	m_Sky.Step();									//天球クラス
	m_PlatformManager.Step();

	switch (m_GameID) {
	case GAME_RESET:
		//プレイヤーのリスポーン処理
		m_Player.Respawn();
		//カメラマネージャークラス
		m_CameraManager.Init();
		//開始待機へ
		m_GameID = GAME_START_WAIT;
		break;
	case GAME_START_WAIT:
		//フェードイン関数を呼び出す
		Fade::RequestIn(PLAYER_RESPAWN_FADE_SPEED, BLACK);
		//フェードインが終わったら
		if (Fade::IsEndIn()) {
			//プレイヤーが動けるように
			m_GameID = GAME_STEP;
			//リスポーン完了
			m_Player.SetIsRespawn(false);
		}
		break;
	case GAME_STEP:
		//リスポーンが必要なら
		if (m_Player.GetIsRespawn()) {
			//リセット待機へ
			m_GameID = GAME_RESET_WAIT;
		}
		break;
	case GAME_RESET_WAIT:
		//フェードアウト関数を呼び出す
		Fade::RequestOut(PLAYER_DEATH_FADE_SPEED, BLACK);
		//フェードアウトが終わったら
		if (Fade::IsEndOut()) {
			m_GameID = GAME_RESET;
		}
		break;
	}
	PlayerStep();

	float ShadowPos = 500.0f;
	/*VECTOR MinShadow = VGet(m_Player.GetPos().x - ShadowPos, m_Player.GetPos().y - ShadowPos, m_Player.GetPos().z - ShadowPos);
	VECTOR MaxShadow = VGet(m_Player.GetPos().x + ShadowPos, m_Player.GetPos().y + ShadowPos, m_Player.GetPos().z + ShadowPos);*/

	VECTOR MinShadow = VGet(-ShadowPos,-100.0f,-ShadowPos);
	VECTOR MaxShadow = VGet(ShadowPos,1500.0f, ShadowPos);
	SetShadowMapDrawArea(m_ShadowHndl, MinShadow,MaxShadow);

	m_Boar.SetPlayerPos(m_Player.GetPos());
	m_Boar.Step();									//イノシシクラス
	m_Wolf.SetPlayerPos(m_Player.GetPos());
	m_Wolf.Step();									//オオカミクラス

	CameraStep();

	HitCheck::ObjectToPlatform(m_Player, m_PlatformManager);

	if (InputKey::IsPushKeyTrg(KEY_INPUT_N)) {
		Res = 1;
	}

	//終了位置設定　後にゴールオブジェクトを追加する
	VECTOR Pos = m_Player.GetPos();
	if (Pos.x <= 30.0f && Pos.x >= -30.0f && Pos.y >= 1140 && Pos.z <= 670.0f && Pos.z >= 610.0f) {
		Res = 1;
	}

	StatusStep();

	Update();

	return Res;
}
//モデル更新処理
void StageScene::Update() {
	m_Sky.Update();										//天球クラス
	m_PlatformManager.Update();
	m_Player.Update();
	m_Sword.Update();
	m_Shield.Update();
	m_Boar.Update();									//イノシシクラス
	m_Wolf.Update();									//オオカミクラス
	m_CameraManager.Update();							//カメラマネージャークラス
}
//プレイヤー関連Step
void StageScene::PlayerStep() {
	m_Player.SetCameraRot(m_CameraManager.GetCameraRot());
	m_Player.Step();
	m_Sword.Step();
	m_Shield.Step();
}
//ステータス関連Step
void StageScene::StatusStep() {
	int HitPoints = (int)m_Player.GetHitPoints();
	m_HitPoints.SetHitPoints(HitPoints);			//体力UIクラス
	int SkillPoints = (int)m_Player.GetSkillPoints();
	m_SkillPoints.SetSkillPoints(SkillPoints);		//スキルポイントUIクラス
	int Stamina = (int)m_Player.GetStamina();
	m_Stamina.SetStamina(Stamina);					//スタミナUIクラス
}
//カメラ関連Step
void StageScene::CameraStep() {
	if (m_CameraManager.GetID() == CameraManager::TagCamera::TARGET) {
		/*m_Target.Update(Vec);*/
	}

	VECTOR	CameraPos = m_CameraManager.GetCameraPos();
	CameraPos.y = 0.0f;
	VECTOR  CameraTargetPos = m_CameraManager.GetTargetPos();
	CameraTargetPos.y = 0.0f;
	VECTOR	TargetPos = m_Player.GetAttackTargetPos();
	TargetPos.y = 0.0f;

	VECTOR	SubCalc1 = VSub(CameraPos, CameraTargetPos);
	VECTOR	SubCalc2 = VSub(CameraPos, TargetPos);

	SubCalc1 = VNorm(SubCalc1);
	SubCalc2 = VNorm(SubCalc2);
	float Dot = VDot(SubCalc1, SubCalc2);

	if (Dot < 1.0f) {
		Dot = acosf(Dot);

		Dot = Dot * 180.0f / DX_PI_F;
	}

	if (Dot < 90.0f) {
		if (InputPad::IsPushPadTrg(XINPUT_BUTTON_RIGHT_THUMB) || InputPad::IsPushPadTrg(XINPUT_BUTTON_LEFT_THUMB) || InputKey::IsPushKeyTrg(KEY_INPUT_C)) {
			if (m_CameraManager.GetID() == CameraManager::TagCamera::TARGET) {
				m_CameraManager.ChangeCamera(CameraManager::TagCamera::PLAYER);
			}
			else if (m_CameraManager.GetID() == CameraManager::TagCamera::PLAYER) {
				m_CameraManager.ChangeCamera(CameraManager::TagCamera::TARGET);
			}
		}
	}

	m_CameraManager.Step(m_Player);
}
