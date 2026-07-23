#pragma once
#include "Comon.h"
#include "Game/Object/Actor/Character/Player/Player.h"
#include "Lib/Input/InputManager.h"

//ゲームプレイ中に使うカメラクラス
class PlayerCamera {
private:
	VECTOR	m_CameraPos;		//カメラ位置
	VECTOR	m_TargetPos;		//注視点
	VECTOR	m_UpVec;			//カメラの上方向
	VECTOR	m_CameraRot;		//カメラ回転角度
	VECTOR	m_CalcRot;

	VECTOR	m_CameraPoint;		//移動先のカメラ座標
	VECTOR	m_TargetPoint;		//移動先の注視点座標

	//現在座標から目標座標へ線形補間した座標を返す
	//_CurrentPos		:現在座標
	//_TargetPos		:目標座標
	//_LerpRate			:補間率 
	//return			:VECTOR
	VECTOR	CameraLerp(VECTOR _CurrentPos, VECTOR _TargetPos, float _LerpRate);
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

	//フラグセット
	//カメラ位置
	void SetCameraPos(VECTOR _CameraPos) { m_CameraPos = _CameraPos; }
	//注視点
	void SetTargetPos(VECTOR _TargetPos) { m_TargetPos = _TargetPos; }
	//カメラ回転角度
	void SetCameraRot(VECTOR _CameraRot) { m_CameraRot = _CameraRot; }
};
