#pragma once
#include "Comon.h"
#include "Lib/Input/InputKey.h"
#include "Lib/Input/InputPad.h"
#include "Lib/Fade/Fade.h"
#include "Game/Sound/SoundManager.h"

class SceneBase {
protected:
	//処理を管理するタグ
	enum Tag_Scene {
		INIT,			//初期化処理を行う
		LOAD,			//データ読み込み処理を行う
		STARTWAIT,		//フェードイン処理を確実に行う	BGM再生なども
		STEP,			//毎フレーム呼び出す処理を行う
		ENDWAIT,		//フェードアウトを確実に行う
		END,			//データ破棄を行う

		SCENE_NUM
	};

	//タグ変数
	Tag_Scene m_ID;

public:
	//コンストラクタ・デストラクタ
	SceneBase();
	virtual ~SceneBase() {};

	//シーン中繰り返し行う処理
	virtual int Loop() = 0;

	//描画処理
	virtual void Draw() = 0;

protected:
	//初期化処理管理関数
	virtual void Init() = 0;

	//データ破棄処理管理関数
	virtual void  Exit() = 0;

	//データ読み込み処理管理関数
	virtual void Load() = 0;

	//毎フレーム呼び出す処理管理関数
	virtual int Step() = 0;
};
