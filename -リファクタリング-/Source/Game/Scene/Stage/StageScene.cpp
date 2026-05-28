#include "StageScene.h"

//コンストラクタ
StageScene::StageScene() {
	//タグをINITに設定
	m_ID = INIT;
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
		Fade::RequestIn();
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
		Fade::RequestOut();
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
	DrawFormatStringToHandle((int)SCREEN_HALF_X, (int)SCREEN_HALF_Y, WHITE, DxLibFont::FONTHNDL_N15, "ステージ");

	m_Sky.Draw();									//天球クラス
	m_Player.Draw();								//プレイヤークラス
	m_Sword.Draw();									//剣クラス
	m_Shield.Draw();								//盾クラス
	m_PlatformManager.Draw();						//プラットフォームマネージャークラス
	m_HitPoints.Draw();								//体力UIクラス
	m_SkillPoints.Draw();							//スキルポイントUIクラス
	m_Stamina.Draw();								//スタミナUIクラス
}
//初期化処理管理関数
void StageScene::Init() {
	m_Sky.Init();									//天球クラス
	m_Player.Init();								//プレイヤークラス
	//オーナーを設定
	m_Sword.Init(&m_Player);						//剣クラス
	//オーナーを設定
	m_Shield.Init(&m_Player);						//盾クラス
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
	m_HitPoints.Exit();								//体力UIクラス
	m_SkillPoints.Exit();							//スキルポイントUIクラス
	m_Stamina.Exit();								//スタミナUIクラス
	m_PlatformManager.Exit();						//プラットフォームマネージャークラス
}
//データ読み込み処理管理関数
void StageScene::Load() {
	m_Sky.Load();									//天球クラス
	m_Player.Load();								//プレイヤークラス
	m_Sword.Load();									//剣クラス
	m_Shield.Load();								//盾クラス
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

	m_Player.SetCameraRot(m_CameraManager.GetCameraRot());
	m_Player.Step();
	m_Sword.Step();
	m_Shield.Step();

	HitCheck::ObjectToPlatform(m_Player, m_PlatformManager);

	if (InputKey::IsPushKeyTrg(KEY_INPUT_N)) {
		Res = 1;
	}

	//終了位置設定　後にゴールオブジェクトを追加する
	VECTOR Pos = m_Player.GetPos();
	if (Pos.x <= 30.0f && Pos.x >= -30.0f && Pos.y >= 1200 && Pos.z <= 670.0f && Pos.z >= 610.0f) {
		Res = 1;
	}

	m_CameraManager.Step(m_Player.GetPos(), VZERO, false);

	int HitPoints = m_Player.GetHitPoints();
	m_HitPoints.SetHitPoints(HitPoints);			//体力UIクラス
	int SkillPoints = m_Player.GetSkillPoints();
	m_SkillPoints.SetSkillPoints(SkillPoints);		//スキルポイントUIクラス
	int Stamina = m_Player.GetStamina();
	m_Stamina.SetStamina(Stamina);					//スタミナUIクラス

	Update();

	return Res;
}
//モデル更新処理
void StageScene::Update() {
	m_Sky.Update();												//天球クラス
	m_PlatformManager.Update();
	m_Player.Update();
	m_Sword.Update();
	m_Shield.Update();
	m_CameraManager.Update();									//カメラマネージャークラス
}
