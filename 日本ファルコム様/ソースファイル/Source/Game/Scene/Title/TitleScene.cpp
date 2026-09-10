#include "TitleScene.h"
#include "Lib/Input/InputManager.h"

namespace {
	constexpr int	FADE_SPEED = 5;			//フェードスピード
	constexpr float SHADOW_RANGE = 200.0f;			//影の判定範囲
}

//コンストラクタ
TitleScene::TitleScene() {
	//タグをINITに設定
	m_ID = INIT;
	//影生成用のハンドル
	m_ShadowHndl = MakeShadowMap(4096, 4096);
	//影用ライト
	SetShadowMapLightDirection(m_ShadowHndl, { 1.0f, -1.0f, 1.0f });
}

//デストラクタ
TitleScene::~TitleScene() {
	//データ破棄処理管理関数呼び出す
	Exit();
}

//シーン中繰り返し行う処理
int TitleScene::Loop() {
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
void TitleScene::Draw() {
	switch (m_ID) {
	case STEP:
	case ENDWAIT:
		m_Sky.Draw();								//天球クラス

		//影生成セットアップ
		ShadowMap_DrawSetup(m_ShadowHndl);
		m_PlatformManager.ShadowMapDraw();			//プラットフォームマネージャークラス
		//影設定終了
		ShadowMap_DrawEnd();
		//影生成
		SetUseShadowMap(0, m_ShadowHndl);
		m_PlatformManager.Draw();					//プラットフォームマネージャークラス
		//影生成終了
		SetUseShadowMap(0, -1);
		m_Title.Draw();
		break;
	}
}

//初期化処理管理関数
void TitleScene::Init() {
	m_Sky.Init();																	//天球クラス
	m_Title.Init();

	const int Map = 0;

	m_PlatformManager.Init(Map);													//プラットフォームマネージャークラス
	
	m_CameraManager.Init(CameraManager::TagCamera::TITLE);							//カメラマネージャークラス
}

//データ破棄処理管理関数
void TitleScene::Exit() {
	m_Sky.Exit();									//天球クラス
	m_Title.Exit();
	m_PlatformManager.Exit();						//プラットフォームマネージャークラス
	DeleteShadowMap(m_ShadowHndl);
}

//データ読み込み処理管理関数
void TitleScene::Load() {
	m_Sky.Load();									//天球クラス
	m_Title.Load();
	m_PlatformManager.Load();						//プラットフォームマネージャークラス
}

//毎フレーム呼び出す処理管理関数
int TitleScene::Step() {
	int Res = 0;
	m_PlatformManager.Step();

	m_Title.Step();

	if (InputManager::IsPushTrg(InputManager::TagInput::INPUT_JUMP)) {
		Res = 1;
	}

	m_CameraManager.Step();
	VECTOR Pos = m_CameraManager.GetCameraPos();
	m_Sky.Step(Pos);								//天球クラス

	float ShadowPos = SHADOW_RANGE;
	VECTOR MinShadow = VGet(Pos.x - ShadowPos, Pos.y - ShadowPos, Pos.z - ShadowPos);
	VECTOR MaxShadow = VGet(Pos.x + ShadowPos, Pos.y + ShadowPos, Pos.z + ShadowPos);

	SetShadowMapDrawArea(m_ShadowHndl, MinShadow, MaxShadow);

	m_CameraManager.Update();						//カメラマネージャークラス
	m_PlatformManager.Update();
	m_Sky.Update();									//天球クラス

	return Res;
}