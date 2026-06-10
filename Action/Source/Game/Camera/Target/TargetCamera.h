#pragma once
#include "Comon.h"
#include "Lib/Input/InputKey.h"
#include "Lib/Input/InputPad.h"
#include "Game/Object/Actor/Character/Player/Player.h"

//ゲームプレイ中に使うカメラクラス
class TargetCamera {
private:
	VECTOR	m_CameraPos;		//カメラ位置
	VECTOR	m_TargetPos;		//注視点
	VECTOR	m_UpVec;			//カメラの上方向
	VECTOR	m_CameraRot;		//カメラ回転角度
	VECTOR	m_CalcRot;

	VECTOR	m_CameraPoint;
	VECTOR	m_TargetPoint;

	bool	m_IsCameraPosSetEnd;
	bool	m_IsTargetPosSetEnd;
public:
	//コンストラクタ
	TargetCamera();

	//初期化
	void Init();

	//毎フレーム呼び出す処理(ノーマル)
	void Step(Player& _Player);

	//更新処理
	void Update();

	//デバック用
	void Draw();

	//カメラ位置
	VECTOR GetCameraPos() { return m_CameraPos; }
	//注視点
	VECTOR GetTargetPos() { return m_TargetPos; }
	//カメラの回転率
	VECTOR GetCameraRot() { return m_CameraRot; }

	//フラグセット
	//プレイヤーカメラとターゲットカメラが切り替わるタイミングにマネージャーで呼ぶ
	void SetIsCameraPosSetEndFalse() { m_IsCameraPosSetEnd = false; }
	void SetIsTargetPosSetEndFalse() { m_IsTargetPosSetEnd = false; }
	//カメラ位置
	void SetCameraPos(VECTOR _CameraPos) { m_CameraPos = _CameraPos; }
	//注視点
	void SetTargetPos(VECTOR _TargetPos) { m_TargetPos = _TargetPos; }
	//カメラ回転角度
	void SetCameraRot(VECTOR _CameraRot) { m_CameraRot = _CameraRot; }
};
