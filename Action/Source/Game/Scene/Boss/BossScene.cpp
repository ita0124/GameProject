#include "BossScene.h"

namespace {
	constexpr int	LOAD_TIME = 60;					//非同期処理の最低時間
	constexpr float	HNDLNUM_ADD = 0.1f;				//HndlNumに１フレームずつ加算する

	constexpr const char* FILE_PATH[HNDL_MAX] = {	//非同期中に表示する画像のファイルパス
	"Data/Load/Elephant1.png",						//象１頭
	"Data/Load/Elephant2.png",						//象２頭
	"Data/Load/Elephant3.png",						//象３頭
	"Data/Load/Elephant4.png",						//象４頭
	};

	constexpr int	FADE_SPEED = 5;					//フェードスピード
	constexpr int	PLAYER_RESPAWN_FADE_SPEED = 5;	//リスポーン完了フェードスピード
	constexpr int	PLAYER_DEATH_FADE_SPEED = 5;	//プレイヤー落下フェードスピード
	constexpr float CHANGE_DISTANCE = 100.0f;		//攻撃対象を切り替える距離差
	constexpr float SHADOW_RANGE = 500.0f;			//影の判定範囲
}

//コンストラクタ
BossScene::BossScene() {
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
BossScene::~BossScene() {
	//データ破棄処理管理関数呼び出す
	Exit();
}
//シーン中繰り返し行う処理
int	BossScene::Loop() {
	int Res = 0;

	switch (m_ID)
	{
		//初期化処理を行う
	case INIT:
		//初期化処理管理関数を呼び出す
		Init();
		//フェードイン関数を呼び出す
		Fade::RequestIn(FADE_SPEED);
		//フェードインが終わったら
		if (Fade::IsEndIn()) {
			//状態をLOADに
			m_ID = LOAD;
		}
		break;
		//データ読み込み処理を行う
	case LOAD:
		//データ読み込み処理(非同期)
		if (LoadASync()) {
			SetUseASyncLoadFlag(false);
			//状態をSTARTWAITに
			m_ID = STARTWAIT;
		}
		break;
		//フェードイン処理を確実に行う
	case STARTWAIT:
		//フェードイン関数を呼び出す
		Fade::RequestIn(FADE_SPEED);
		//フェードインが終わったら
		if (Fade::IsEndIn()) {
			//状態をSTEPに
			m_ID = STEP;
			//BGMを呼び出す
			SoundManager::Play(SoundManager::TagID::BGM_MAIN, DX_PLAYTYPE_LOOP);
		}
		break;
		//毎フレーム呼び出す処理を行う
	case STEP:
		//毎フレーム呼び出す処理管理関数
		//0以外が返ってきたら
		if (Step() != 0) {
			//状態をENDWAITに
			m_ID = ENDWAIT;
		}
		break;
		//フェードアウトを確実に行う
	case ENDWAIT:
		//フェードアウト関数を呼び出す
		Fade::RequestOut(FADE_SPEED);
		//フェードアウトが終わったら
		if (Fade::IsEndOut()) {
			//状態をENDに
			m_ID = END;
		}
		break;
		//データ破棄を行う
	case END:
		//音楽停止
		SoundManager::AllStop();
		//データ破棄処理管理関数を呼び出す
		Exit();
		//状態をINITに
		m_ID = INIT;

		Res = 1;
		break;
	}
	//0以外の数値が入ると次のシーンへ移行する
	return Res;
}
//描画処理管理関数
void BossScene::Draw() {
	switch (m_ID) {
	case LOAD:
		DrawRotaGraph((int)SCREEN_HALF_X, (int)SCREEN_HALF_Y, 10, 0, m_Load.Hndl[(int)m_Load.HndlNum % 4], TRUE);
		break;
	case STARTWAIT:
		m_Sky.Draw();
		m_BossArea.Draw();
		break;
	case STEP:
	case ENDWAIT:
		m_Sky.Draw();								//天球クラス
		m_BossArea.Draw();							//ボス戦の足場クラス
		//影生成セットアップ
		ShadowMap_DrawSetup(m_ShadowHndl);
		m_Player.Draw();							//プレイヤークラス
		m_Sword.Draw();								//剣クラス
		m_Shield.Draw();							//盾クラス
		m_Boss.Draw();								//ボスクラス
		m_BossGimmickManager.Draw();				//Bossが使うObjectを管理するクラス
		//影設定終了
		ShadowMap_DrawEnd();
		//影生成
		SetUseShadowMap(0, m_ShadowHndl);
		m_BossArea.Draw();							//ボス戦の足場クラス
		m_Player.Draw();							//プレイヤークラス
		m_Sword.Draw();								//剣クラス
		m_Shield.Draw();							//盾クラス
		m_Boss.Draw();								//ボスクラス
		m_BossGimmickManager.Draw();				//Bossが使うObjectを管理するクラス
		//影生成終了
		SetUseShadowMap(0, -1);

		m_StatusDrawManager.Draw();					//ステータス描画マネージャー
		break;
	}
}
//初期化処理管理関数
void BossScene::Init() {
	m_BossArea.Init();								//ボス戦の足場クラス
	m_Sky.Init();									//天球クラス
	m_Player.Init();								//プレイヤークラス
	m_Sword.Init(&m_Player);						//剣クラス
	m_Shield.Init(&m_Player);						//盾クラス

	m_Boss.Init();									//ボスクラス
	m_CameraManager.Init();							//カメラマネージャークラス
	m_BossGimmickManager.Init(&m_Boss);				//Bossが使うObjectを管理するクラス

	m_StatusDrawManager.Init(&m_Player);			//ステータス描画マネージャー

	m_Load.LoadTime = 0;							//非同期処理継続時間
	m_Load.HndlNum = 0.0f;							//非同期処理時に表示する画像
	m_Load.IsLoadASync = false;						//非同期処理中か
	for (int Index = 0; Index < HNDL_MAX; Index++) {
		m_Load.Hndl[Index] = -1;					//画像ハンドル
	}
}
//データ破棄処理管理関数
void BossScene::Exit() {
	m_BossArea.Exit();								//ボス戦の足場クラス
	m_Sky.Exit();									//天球クラス
	m_Player.Exit();								//プレイヤークラス
	m_Sword.Exit();									//剣クラス
	m_Shield.Exit();								//盾クラス
	m_Boss.Exit();									//ボスクラス
	m_StatusDrawManager.Exit();						//ステータス描画マネージャー
	m_BossGimmickManager.Exit();					//Bossが使うObjectを管理するクラス

	DeleteShadowMap(m_ShadowHndl);
}
//データ読み込み処理管理関数
void BossScene::Load() {
	//非同期読み込みを行う
	SetUseASyncLoadFlag(true);

	m_BossArea.Load();								//ボス戦の足場クラス
	m_Sky.Load();									//天球クラス
	m_Player.Load();								//プレイヤークラス
	m_Sword.Load();									//剣クラス
	m_Shield.Load();								//盾クラス
	m_Boss.Load();									//ボスクラス
	m_StatusDrawManager.Load();						//ステータス描画マネージャー
	m_BossGimmickManager.Load();					//Bossが使うObjectを管理するクラス

	//非同期読み込みを行わない
	SetUseASyncLoadFlag(false);
}
//データ読み込み処理(非同期)
bool BossScene::LoadASync() {
	if (m_Load.IsLoadASync) {
		m_Load.HndlNum += HNDLNUM_ADD;
		m_Load.LoadTime++;

		//今非同期処理をしている総数
		int ASyncLoadNum = GetASyncLoadNum();

		if (ASyncLoadNum == 0 && m_Load.LoadTime > LOAD_TIME) {
			//非同期処理終了
			m_Load.IsLoadASync = false;
			m_Load.HndlNum = 0.0f;
			m_Load.LoadTime = 0;

			return true;
		}
	}
	else {
		for (int Index = 0; Index < HNDL_MAX; Index++) {
			if (m_Load.Hndl[Index] != -1)continue;
			m_Load.Hndl[Index] = LoadGraph(FILE_PATH[Index]);		//画像ハンドル
		}
		//データ読み込み処理管理関数を呼び出す
		Load();
		//非同期処理中に変更
		m_Load.IsLoadASync = true;
	}
	return false;
}
//毎フレーム呼び出す処理管理関数
int BossScene::Step() {
	int Res = 0;

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
	m_Player.SetAttackTarget(&m_Boss);
	PlayerStep();

	float ShadowPos = SHADOW_RANGE;
	VECTOR MinShadow = VGet(m_Player.GetPos().x - ShadowPos, m_Player.GetPos().y - ShadowPos, m_Player.GetPos().z - ShadowPos);
	VECTOR MaxShadow = VGet(m_Player.GetPos().x + ShadowPos, m_Player.GetPos().y + ShadowPos, m_Player.GetPos().z + ShadowPos);

	SetShadowMapDrawArea(m_ShadowHndl, MinShadow, MaxShadow);

	EnemyStep();
	HitCheck();

	CameraStep();

	m_StatusDrawManager.SetStatus();					//ステータス描画マネージャー

	Update();

	if (!m_Player.GetIsActive()) {
		Res = 1;
		ResultNum::SetNum(Res);
	}
	if (!m_Boss.GetIsActive()) {
		Res = 2;
		ResultNum::SetNum(Res);
	}

	return Res;
}
//モデル更新処理
void BossScene::Update() {
	m_BossArea.Update();								//ボス戦の足場クラス
	m_Sky.Update();										//天球クラス
	m_Player.Update();									//プレイヤークラス
	m_Sword.Update();									//剣クラス
	m_Shield.Update();									//盾クラス
	m_Boss.Update();									//ボスクラス
	m_CameraManager.Update();							//カメラマネージャークラス
	m_BossGimmickManager.Update();						//Bossが使うObjectを管理するクラス
}
//プレイヤー関連Step
void BossScene::PlayerStep() {
	m_BossArea.Step();									//ボス戦の足場クラス
	m_Sky.Step(m_Player.GetPos());						//天球クラス

	m_Player.SetCameraRot(m_CameraManager.GetCameraRot());

	if (m_Boss.GetRockOn()) {
		//Bossを攻撃対象に設定
		m_Player.SetAttackTarget(&m_Boss);
		m_Player.SetAttackTargetPos(m_Boss.GetFramePos(m_Boss.GetHndl(), Boss::FrameNumber::CHEST));
	}
	else {
		//攻撃対象を初期化
		ObjectBase* AttackTarget = nullptr;
		//攻撃対象までの距離を初期化
		float CurrentTargetLength = FLT_MAX;
		for (int CrystalIndex = 0; CrystalIndex < CRYSTAL_MAX; CrystalIndex++) {
			//クリスタルを取得
			Crystal& OneCrystal = m_BossGimmickManager.GetCrystal(CrystalIndex);
			//取得したクリスタルクラスの生存フラグがオフになっていれば次のforへ
			if (!OneCrystal.GetIsActive()) continue;
			//プレイヤーとの距離を計算
			float Length = VSize(VSub(m_Player.GetPos(), OneCrystal.GetPos()));
			//現在の攻撃対象より一定距離近ければ更新
			if (Length < CurrentTargetLength - CHANGE_DISTANCE) {
				CurrentTargetLength = Length;
				AttackTarget = &OneCrystal;
			}
		}
		//攻撃対象を設定
		m_Player.SetAttackTarget(AttackTarget);
		//攻撃対象が存在する場合は位置を設定
		if (AttackTarget != nullptr) {
			m_Player.SetAttackTargetPos(AttackTarget->GetPos());
		}
	}

	m_Player.Step();
	m_Sword.Step();
	m_Shield.Step();
}
//敵関連Step
void BossScene::EnemyStep() {
	m_Boss.SetPlayerPos(m_Player.GetPos());
	m_Boss.Step();
	m_BossGimmickManager.Step();
}
//カメラ関連Step
void BossScene::CameraStep() {
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
		if (InputManager::IsPushTrg(InputManager::TagInput::INPUT_CAMERA_CHANGE)) {
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
//当たり判定関係
void BossScene::HitCheck() {
	//プレイヤーとボスの押し合い当たり判定
	HitCheck::ObjectToObjectPush(m_Player, m_Boss);
	//ボスと剣の攻撃当たり判定
	HitCheck::ObjectToObjectAttack(m_Boss, m_Sword);
	//盾とボスの攻撃当たり判定
	HitCheck::ObjectToObjectAttack(m_Shield, m_Boss);
	//プレイヤーとボスの攻撃当たり判定
	HitCheck::ObjectToObjectAttack(m_Player, m_Boss);
	//プレイヤーとボスの位置関係判定
	HitCheck::ObjectToObjectRelativePos(m_Player, m_Boss);
	//ステージとプレイヤーの当たり判定
	HitCheck::ObjectToField(m_Player, m_BossArea);
	//Bossが使うObjectを管理するクラスとオブジェクトの当たり判定
	HitCheck::BossGimmickManagerToObjectPush(m_BossGimmickManager,m_Player);
	//Bossが使うObjectを管理するクラスとオブジェクトの当たり判定
	HitCheck::BossGimmickManagerToObjectAttack(m_BossGimmickManager, m_Sword);
}
