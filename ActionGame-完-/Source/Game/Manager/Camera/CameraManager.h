#pragma once
#include "Comon.h"
#include "Game/Camera/Play/PlayCamera.h"

//カメラを管理するクラス
class CameraManager {
public:

	//使用するカメラを切り替えるためのタグ
	enum TagCamera {
		PLAY,			//プレイカメラ

		CAMERA_NUM
	};

	enum TagPlayCamera {
		PLAYER,
		ENEMY,

		PLAYCAMERA_NUM
	};

private:
	TagCamera m_Camera;
	TagPlayCamera m_Target;

	PlayCamera m_PlayCamera;

public:
	//コンストラクタ
	CameraManager();

	//初期化
	void Init();

	//毎フレーム呼び出す処理
	void Step(VECTOR _PlayerPos, VECTOR _TargetEnemyPos, bool _IsSPAtk);

	//更新処理
	void Update();

	void ChangeCamera(TagCamera _Camera) { m_Camera = _Camera; }

	void ChangeTarget(TagPlayCamera _Target) { m_Target = _Target; }

	TagCamera GetID() { return m_Camera; }
	TagPlayCamera GetTatget() { return m_Target; }

	//カメラの回転率
	VECTOR GetCameraRot() { return m_PlayCamera.GetCameraRot(); }

	VECTOR GetCameraPos() { return m_PlayCamera.GetCameraPos(); }
	VECTOR GetTarget() { return m_PlayCamera.GetTarget(); }

	bool GetIsTarget1() { return m_PlayCamera.GetIsTarget1(); }
	bool GetIsTarget2() { return m_PlayCamera.GetIsTarget2(); }

	void Draw() { m_PlayCamera.Draw(); }
};

