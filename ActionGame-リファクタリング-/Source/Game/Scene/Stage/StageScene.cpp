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
			//BGMを呼び出す
			SoundManager::Play(SoundManager::TagID::BGM_TITLE, DX_PLAYTYPE_LOOP);
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
	DrawFormatStringToHandle((int)SCREEN_HALF_X,(int)SCREEN_HALF_Y, WHITE, DxLibFont::FONTHNDL_N15,"ステージ");

	m_Sky.Draw();
	m_Player.Draw();
	m_NormalPlatform.Draw();
	m_MovingPlatform.Draw();
	m_JumpPlatform.Draw();
	m_FallingPlatform.Draw();
	m_BattlePlatform.Draw();
}
//初期化処理管理関数
void StageScene::Init() {
	m_Sky.Init();
	m_Player.Init();
	m_NormalPlatform.Init();
	m_MovingPlatform.Init();
	m_JumpPlatform.Init();
	m_FallingPlatform.Init();
	m_CameraManager.Init();
	m_BattlePlatform.Init();
}
//データ破棄処理管理関数
void StageScene::Exit() {
	m_Sky.Exit();
	m_Player.Exit();
	m_NormalPlatform.Exit();
	m_MovingPlatform.Exit();
	m_JumpPlatform.Exit();
	m_FallingPlatform.Exit();
	m_BattlePlatform.Exit();
}
//データ読み込み処理管理関数
void StageScene::Load() {
	m_Sky.Load();
	m_Player.Load();
	m_NormalPlatform.Load();
	m_MovingPlatform.Load();
	m_JumpPlatform.Load();
	m_FallingPlatform.Load();
	m_BattlePlatform.Load();
}
//毎フレーム呼び出す処理管理関数
int StageScene::Step() {
	int Res = 0;

	m_Sky.Step();						//天球クラス
	m_NormalPlatform.Step();
	m_MovingPlatform.Step();
	m_JumpPlatform.Step();
	m_FallingPlatform.Step();
	m_BattlePlatform.Step();

	m_Player.SetCameraRot(m_CameraManager.GetCameraRot());
	m_Player.Step();

	if (InputKey::IsPushKeyTrg(KEY_INPUT_N)) {
		Res = 1;
	}

	Update();

	return Res;
}
//モデル更新処理
void StageScene::Update() {
	m_Sky.Update();						//天球クラス
	m_NormalPlatform.Update();
	m_MovingPlatform.Update();
	m_JumpPlatform.Update();
	m_FallingPlatform.Update();
	m_BattlePlatform.Update();
	m_Player.Update();
	m_CameraManager.Update();			//カメラマネージャークラス
	m_CameraManager.Step(m_Player.GetPos(), VZERO, false);
}
