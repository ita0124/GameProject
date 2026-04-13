#include "SceneManager.h"

SceneManager::TagScene SceneManager::m_ID;
SceneBase* SceneManager::m_Scene[SCENE_NUM];

//初期化処理
void SceneManager::Init() {
	for (int i = 0; i < SCENE_NUM; i++) {
		m_Scene[i] = nullptr;
	}

	m_ID = TITLE;

	//newする
	//タイトル
	if (m_Scene[TITLE] == nullptr) {
		m_Scene[TITLE] = new TitleScene;
	}
	//セレクト
	if (m_Scene[SELECT] == nullptr) {
		m_Scene[SELECT] = new SelectScene;
	}
	//ゲーム本編
	if (m_Scene[MAIN] == nullptr) {
		m_Scene[MAIN] = new MainScene;
	}
	//リザルト
	if (m_Scene[RESULT] == nullptr) {
		m_Scene[RESULT] = new ResultScene;
	}
}

//データ破棄処理
void SceneManager::Exit() {
	for (int i = 0; i < SCENE_NUM; i++) {
		if (m_Scene[i] != nullptr) {
			delete m_Scene[i];
			m_Scene[i] = nullptr;
		}
	}
}

//実行中繰り返し行われる処理管理関数
int SceneManager::Loop() {
	int Res = 0;
	//ゲーム全体のシーンを管理する
	switch (m_ID) {
	case TITLE:
		//終われば次へ
		if (m_Scene[TITLE]->Loop() != 0) {
			m_ID = SELECT;
		}
		break;

	case SELECT:
		//終われば次へ
		if (m_Scene[SELECT]->Loop() != 0) {
			m_ID = MAIN;
		}
		break;

	case MAIN:
		//終われば次へ
		if (m_Scene[MAIN]->Loop() != 0) {
			m_ID = RESULT;
		}
		break;

	case RESULT:
		//終われば次へ
		if (m_Scene[RESULT]->Loop() != 0) {
			m_ID = TITLE;
		}
		break;
	}

	return Res;
}

//描画処理管理
void SceneManager::Draw() {
	m_Scene[m_ID]->Draw();
}
