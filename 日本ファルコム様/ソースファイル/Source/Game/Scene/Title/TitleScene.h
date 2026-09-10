#pragma once
#include "Game/Base/Scene/SceneBase.h"
#include "Game/Title/Title.h"
#include "Game/Object/Sky/Sky.h"
#include "Game/Manager/Platform/PlatformManager.h"
#include "Game/Manager/Camera/CameraManager.h"

class TitleScene :public SceneBase {
private:
	Title				m_Title;
	Sky					m_Sky;					//天球クラス
	PlatformManager		m_PlatformManager;		//プラットフォームマネージャークラス
	CameraManager		m_CameraManager;		//カメラマネージャークラス

	int					m_ShadowHndl;			//影生成用のハンドル
public:
	//コンストラクタ・デストラクタ
	TitleScene();
	~TitleScene();

	//シーン中繰り返し行う処理
	int Loop() override;

	//描画処理管理関数
	void Draw() override;

private:
	//初期化処理管理関数
	void Init() override;

	//データ破棄処理管理関数
	void Exit() override;

	//データ読み込み処理管理関数
	void Load() override;

	//毎フレーム呼び出す処理管理関数
	int Step() override;
};
