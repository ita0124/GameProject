#pragma once
#include "Comon.h"
#include "Game/Camera/Player/PlayerCamera.h"
#include "Game/Camera/Target/TargetCamera.h"
#include "Game/Camera/Title/TitleCamera.h"

//カメラを管理するクラス
class CameraManager {
public:

	//使用するカメラを切り替えるためのタグ
	enum TagCamera {
		PLAYER,			//プレイヤーカメラ
		TARGET,			//ターゲットカメラ
		TITLE,			//タイトル専用

		CAMERA_NUM
	};
private:
	TagCamera		m_Camera;

	PlayerCamera	m_PlayCamera;
	TargetCamera	m_TargetCamera;
	TitleCamera		m_TitleCamera;

	Player*			m_Owner;

public:
	//コンストラクタ
	CameraManager();

	//初期化
	void Init(TagCamera _Camera = PLAYER);

	//毎フレーム呼び出す処理
	void Step();

	//更新処理
	void Update();

	void ChangeCamera(TagCamera _Camera);

	TagCamera GetID() { return m_Camera; }

	//カメラの回転率
	VECTOR GetCameraRot();

	VECTOR GetCameraPos();
	VECTOR GetTargetPos();

	void SetOwner(Player* _Owner) { m_Owner = _Owner; }

	void Draw();
};

