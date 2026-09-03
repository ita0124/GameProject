#include "StageScene.h"

namespace {
	constexpr int	FADE_SPEED = 5;					//フェードスピード
	constexpr int	PLAYER_RESPAWN_FADE_SPEED = 5;	//リスポーン完了フェードスピード
	constexpr int	PLAYER_DEATH_FADE_SPEED = 5;	//プレイヤー落下フェードスピード
	constexpr float CHANGE_DISTANCE = 50.0f;		//攻撃対象を切り替える距離差
	constexpr float SHADOW_RANGE = 200.0f;			//影の判定範囲
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
	SetShadowMapLightDirection(m_ShadowHndl, { 1.0f, -1.0f, 1.0f });
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
			//BGMを呼び出す
			SoundManager::Play(SoundManager::TagID::BGM_NORMAL_MAP, DX_PLAYTYPE_LOOP);
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

		Res = m_Result;
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

		//影生成セットアップ
		ShadowMap_DrawSetup(m_ShadowHndl);
		m_PlatformManager.ShadowMapDraw();			//プラットフォームマネージャークラス
		m_Player.Draw();							//プレイヤークラス
		m_Sword.Draw();								//剣クラス
		m_Shield.Draw();							//盾クラス
		m_MobEnemyManager.Draw();					//モブ敵マネージャークラス
		m_DropItemManager.Draw();					//ドロップアイテムマネージャー
		//影設定終了
		ShadowMap_DrawEnd();
		//影生成
		SetUseShadowMap(0, m_ShadowHndl);
		m_Player.Draw();							//プレイヤークラス
		m_Sword.Draw();								//剣クラス
		m_Shield.Draw();							//盾クラス
		m_MobEnemyManager.Draw();					//モブ敵マネージャークラス
		m_PlatformManager.Draw();					//プラットフォームマネージャークラス
		m_DropItemManager.Draw();					//ドロップアイテムマネージャー
		//影生成終了
		SetUseShadowMap(0, -1);
		m_StatusDrawManager.Draw();					//ステータス描画マネージャー
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

	Coin::Init();									//コイン

	const int Map = 0;

	m_PlatformManager.Init(Map);					//プラットフォームマネージャークラス
	m_MobEnemyManager.Init(Map);					//モブ敵マネージャークラス
	m_GimmickManager.Init(Map);						//ギミックマネージャークラス
	m_CameraManager.Init();							//カメラマネージャークラス
	m_CameraManager.SetOwner(&m_Player);			//オーナー設定
	m_StatusDrawManager.Init(&m_Player);			//ステータス描画マネージャー
	m_DropItemManager.Init();						//ドロップアイテムマネージャー

	m_Result = 0;
}
//データ破棄処理管理関数
void StageScene::Exit() {
	m_Sky.Exit();									//天球クラス
	m_Player.Exit();								//プレイヤークラス
	m_Sword.Exit();									//剣クラス
	m_Shield.Exit();								//盾クラス
	m_PlatformManager.Exit();						//プラットフォームマネージャークラス
	m_MobEnemyManager.Exit();						//モブ敵マネージャークラス
	m_GimmickManager.Exit();						//ギミックマネージャークラス
	m_StatusDrawManager.Exit();						//ステータス描画マネージャー
	m_DropItemManager.Exit();						//ドロップアイテムマネージャー

	DeleteShadowMap(m_ShadowHndl);
}
//データ読み込み処理管理関数
void StageScene::Load() {
	m_Sky.Load();									//天球クラス
	m_Player.Load();								//プレイヤークラス
	m_Sword.Load();									//剣クラス
	m_Shield.Load();								//盾クラス
	m_PlatformManager.Load();						//プラットフォームマネージャークラス
	m_MobEnemyManager.Load();						//モブ敵マネージャークラス
	m_GimmickManager.Load();						//ギミックマネージャークラス
	m_StatusDrawManager.Load();						//ステータス描画マネージャー
	m_DropItemManager.Load();						//ドロップアイテムマネージャー
}
//毎フレーム呼び出す処理管理関数
int StageScene::Step() {
	int Res = 0;

	m_Sky.Step(m_Player.GetPos());					//天球クラス
	m_PlatformManager.Step();

	switch (m_GameID) {
	case GAME_RESET:
		//プレイヤーのリスポーン処理
		m_Player.Respawn();
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
			//BGMを呼び出す
			SoundManager::Play(SoundManager::TagID::SE_FALL);
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

	float ShadowPos = SHADOW_RANGE;
	VECTOR MinShadow = VGet(m_Player.GetPos().x - ShadowPos, m_Player.GetPos().y - ShadowPos, m_Player.GetPos().z - ShadowPos);
	VECTOR MaxShadow = VGet(m_Player.GetPos().x + ShadowPos, m_Player.GetPos().y + ShadowPos, m_Player.GetPos().z + ShadowPos);

	SetShadowMapDrawArea(m_ShadowHndl, MinShadow, MaxShadow);

	m_MobEnemyManager.SetPlayerPos(m_Player.GetPos());
	m_MobEnemyManager.Step();										//モブ敵マネージャークラス
	m_MobEnemyManager.RequestDropItem(m_DropItemManager);
	m_DropItemManager.Step();
	m_GimmickManager.Step(m_MobEnemyManager, m_PlatformManager);	//ギミックマネージャークラス

	CameraStep();

	//プレイヤーとモブ敵の押し合い当たり判定
	HitCheck::MobEnemyToObjectPush(m_MobEnemyManager, m_Player);
	//モブ敵に対する剣の攻撃当たり判定
	HitCheck::MobEnemyToObjectAttack(m_MobEnemyManager, m_Sword);
	//盾とのモブ敵の攻撃当たり判定
	HitCheck::ObjectToMobEnemyAttack(m_Shield, m_MobEnemyManager);
	//プレイヤーとモブ敵の攻撃当たり判定
	HitCheck::ObjectToMobEnemyAttack(m_Player, m_MobEnemyManager);
	//プレイヤーと足場の当たり判定
	HitCheck::ObjectToPlatform(m_Player, m_PlatformManager);
	//モブ敵と足場の当たり判定
	HitCheck::MobEnemyToPlatform(m_MobEnemyManager, m_PlatformManager);
	//オブジェクトとギミックの当たり判定
	HitCheck::ObjectToGimmick(m_Player, m_GimmickManager);
	//ドロップアイテムとオブジェクトの押し合い当たり判定
	HitCheck::DropItemToObject(m_DropItemManager, m_Player);
	//ドロップアイテムと足場の当たり判定
	HitCheck::DropItemToPlatform(m_DropItemManager, m_PlatformManager);

	if (!m_Player.GetIsActive()) {
		m_Result = 1;
		ResultNum::SetNum(m_Result);
	}
	//終了位置設定
	VECTOR Pos = m_Player.GetPos();
	if (m_Player.GetIsGoal()) {
		m_Result = 2;
	}
	if (CheckHitKey(KEY_INPUT_N)) {
		m_Result = 2;
	}

	StatusStep();

	Update();

	return m_Result;
}
//モデル更新処理
void StageScene::Update() {
	m_Sky.Update();										//天球クラス
	m_PlatformManager.Update();
	m_Player.Update();
	m_Sword.Update();
	m_Shield.Update();
	m_CameraManager.Update();							//カメラマネージャークラス
	m_MobEnemyManager.Update();							//モブ敵マネージャークラス
	m_GimmickManager.Update();							//ギミックマネージャークラス

	m_DropItemManager.Update();							//ドロップアイテムマネージャー
}
//プレイヤー関連Step
void StageScene::PlayerStep() {
	m_Player.SetCameraRot(m_CameraManager.GetCameraRot());

	//現在の攻撃対象を取得
	ObjectBase* AttackTarget = m_Player.GetAttackTarget();
	//現在の攻撃対象までの距離
	float CurrentTargetLength = FLT_MAX;
	//攻撃対象が存在する場合は距離を計算
	if (AttackTarget != nullptr)
	{
		VECTOR CurrentDistance = VSub(m_Player.GetPos(), AttackTarget->GetPos());
		CurrentTargetLength = VSize(CurrentDistance);
	}
	for (int MobIndex = 0; MobIndex < MOB_ENEMY_MAX; MobIndex++)
	{
		//モブ敵を取得
		MobEnemyBase& OneMobEnemy = m_MobEnemyManager.GetMobEnemy(MobIndex);
		//取得したモブ敵クラスの生存フラグがオフになっていれば次のforへ
		if (!OneMobEnemy.GetIsActive()) continue;
		//プレイヤーとの距離を計算
		float Length = VSize(VSub(m_Player.GetPos(), OneMobEnemy.GetPos()));
		//現在の攻撃対象より一定距離近ければ更新
		if (Length < CurrentTargetLength - CHANGE_DISTANCE)
		{
			CurrentTargetLength = Length;
			AttackTarget = &OneMobEnemy;
		}
	}
	if (AttackTarget != nullptr){
		m_Player.SetAttackTarget(AttackTarget);
		m_Player.SetAttackTargetPos(AttackTarget->GetPos());
	}

	m_Player.Step();
	m_Sword.Step();
	m_Shield.Step();
}
//ステータス関連Step
void StageScene::StatusStep() {
	m_StatusDrawManager.SetStatus();						//ステータス描画マネージャー
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

		Dot = Dot * RADIAN_CALC;
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

	m_CameraManager.Step();
}
