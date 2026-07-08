#pragma once
#include "Comon.h"
#include "Game/Camera/Player/PlayerCamera.h"
#include "Game/Camera/Target/TargetCamera.h"

//カメラを管理するクラス
class CameraManager {
public:

	//使用するカメラを切り替えるためのタグ
	enum TagCamera {
		PLAYER,			//プレイヤーカメラ
		TARGET,			//ターゲットカメラ

		CAMERA_NUM
	};
private:
	TagCamera m_Camera;

	PlayerCamera m_PlayCamera;
	TargetCamera m_TargetCamera;

public:
	//コンストラクタ
	CameraManager();

	//初期化
	void Init();

	//毎フレーム呼び出す処理
	void Step(Player& _Player);

	//更新処理
	void Update();

	void ChangeCamera(TagCamera _Camera);

	TagCamera GetID() { return m_Camera; }
	
	//カメラの回転率
	VECTOR GetCameraRot();

	VECTOR GetCameraPos();
	VECTOR GetTargetPos();

	void Draw();
};

