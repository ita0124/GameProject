#pragma once
#include "Comon.h"
#include "Lib/Input/InputKey.h"
#include "Lib/Input/InputPad.h"
#include "Game/Object/Actor/Character/Player/Player.h"

//ゲームプレイ中に使うカメラクラス
class PlayerCamera {
private:
	VECTOR m_CameraPos;		//カメラ位置
	VECTOR m_TargetPos;		//注視点
	VECTOR m_UpVec;			//カメラの上方向
	VECTOR m_CameraRot;		//カメラ回転角度
	VECTOR m_CalcRot;

	VECTOR m_CameraPoint;
	VECTOR m_TargetPoint;

	bool	m_IsCameraPosSetEnd;
	bool	m_IsTargetPosSetEnd;
public:
	//コンストラクタ
	PlayerCamera();

	//初期化
	void Init();

	//毎フレーム呼び出す処理(ノーマル)
	void Step(Player& _Player);

	//更新処理
	void Update();

	//デバック用
	void Draw();

	//カメラの回転率
	VECTOR GetCameraRot() { return m_CameraRot; }

	VECTOR GetCameraPos() { return m_CameraPos; }

	VECTOR GetTargetPos() { return m_TargetPos; }
};
