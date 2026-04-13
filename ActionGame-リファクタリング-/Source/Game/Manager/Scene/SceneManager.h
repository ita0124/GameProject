#pragma once
#include "Game/Scene/Main/MainScene.h"
#include "Game/Scene/Result/ResultScene.h"
#include "Game/Scene/Title/TitleScene.h"
#include "Game/Scene/Select/SelectScene.h"

//シーンを管理するクラス
class SceneManager {
private:
	//シーンを管理するタグ
	enum TagScene {
		TITLE,
		SELECT,
		MAIN,
		RESULT,

		SCENE_NUM
	};

	//
	static TagScene m_ID;
	//
	static SceneBase* m_Scene[SCENE_NUM];

public:
	//初期化処理
	static void Init();

	//データ破棄処理
	static void Exit();

	//実行中繰り返し行われる処理管理関数
	static int Loop();

	//描画処理管理
	static void Draw();
};
