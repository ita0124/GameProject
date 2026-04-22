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
	DrawFormatStringToHandle((int)SCREEN_HALF_X,(int)SCREEN_HALF_Y, WHITE, DxLibFont::FONTHNDL_N15,"タイトル");
}
//初期化処理管理関数
void StageScene::Init() {
}
//データ破棄処理管理関数
void StageScene::Exit() {
}
//データ読み込み処理管理関数
void StageScene::Load() {
}
//毎フレーム呼び出す処理管理関数
int StageScene::Step() {
	int Res = 0;

	if (InputKey::IsPushKeyTrg(KEY_INPUT_SPACE)) {
		Res = 1;
	}

	if (InputPad::IsPushPadTrg(XINPUT_BUTTON_B)) {
		Res = 1;
	}

	return Res;
}