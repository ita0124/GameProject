#include "PlayerCamera.h"

namespace {
	constexpr VECTOR	UP_VEC = { 0.0f,10.0f,0.0f };
	constexpr float		LERP_RATE = 0.05f;							//１フレームの補完率
	constexpr float		PLAYER_FOLLOW_ROTY_SPEED = 0.015f;			//プレイヤー移動方向へのY軸回転スピード
	constexpr float		CONTROL_ROTX_SPEED = 0.05f;					//X軸回転スピード
	constexpr float		CONTROL_ROTY_SPEED = 0.05f;					//Y軸回転スピード
	constexpr float		TARGET_HEIGHT_ADD = 25.0f;					// 注視点の高さ補正
	constexpr VECTOR	CAMERA_OFFSET = { 0.0f,25.0f,100.0f };	// ターゲットから見たカメラの相対位置
	constexpr float		ROT_X_UP_LIMIT = 45.0f * RADIAN_CALC;		// カメラの上方向回転制限
	constexpr float		ROT_X_DOWN_LIMIT = 45.0f * RADIAN_CALC;		// カメラの下方向回転制限
}

//コンストラクタ
PlayerCamera::PlayerCamera() {
	Init();
}
//初期化
void PlayerCamera::Init() {
	m_CameraPoint = VZERO;			//カメラ位置
	m_CameraPos = VZERO;
	m_TargetPoint = VZERO;			//注視点
	m_TargetPos = VZERO;
	m_UpVec = UP_VEC;			//？？？
	m_CameraRot = VZERO;
	m_CalcRot = VZERO;

	Update();
}
//プレイヤー
void PlayerCamera::Step(Player& _Player) {
	m_TargetPoint = _Player.GetPos();
	float RotX = 0.0f;
	float RotY = 0.0f;
	//キーボード
	//X軸
	if (InputKey::IsPushKeyRep(KEY_INPUT_UP)) {
		RotX = -CONTROL_ROTX_SPEED;
	}
	else if (InputKey::IsPushKeyRep(KEY_INPUT_DOWN)) {
		RotX = CONTROL_ROTX_SPEED;
	}
	//Y軸
	//プレイヤーの左右移動に連動
	if (InputKey::IsPushKeyRep(KEY_INPUT_A)) {
		RotY = -PLAYER_FOLLOW_ROTY_SPEED;
	}
	else if (InputKey::IsPushKeyRep(KEY_INPUT_D)) {
		RotY = PLAYER_FOLLOW_ROTY_SPEED;
	}
	//自分で動かす
	if (InputKey::IsPushKeyRep(KEY_INPUT_LEFT)) {
		RotY = -CONTROL_ROTY_SPEED;
	}
	else if (InputKey::IsPushKeyRep(KEY_INPUT_RIGHT)) {
		RotY = CONTROL_ROTY_SPEED;
	}
	//パッド
	//X軸
	RotX = (float)InputPad::GetRAnalogYInput() * CONTROL_ROTX_SPEED;
	//Y軸
	//プレイヤーの左右移動に連動
	if (InputPad::GetLAnalogXInput() < 0.0f) {
		RotY = InputPad::GetLAnalogXInput() * PLAYER_FOLLOW_ROTY_SPEED;
	}
	else if (InputPad::GetLAnalogXInput() > 0.0f) {
		RotY = InputPad::GetLAnalogXInput() * PLAYER_FOLLOW_ROTY_SPEED;
	}
	//自分で動かす
	if (InputPad::GetRAnalogXInput() < 0.0f) {
		RotY = InputPad::GetRAnalogXInput() * CONTROL_ROTY_SPEED;
	}
	else if (InputPad::GetRAnalogXInput() > 0.0f) {
		RotY = InputPad::GetRAnalogXInput() * CONTROL_ROTY_SPEED;
	}

	m_CalcRot.x += RotX;
	m_CalcRot.y += RotY;

	float DownLimit = ROT_X_UP_LIMIT;
	float UpLimit = ROT_X_DOWN_LIMIT;

	if (m_CalcRot.x > DownLimit) {
		m_CalcRot.x = DownLimit;
	}
	else if (m_CalcRot.x < -UpLimit) {
		m_CalcRot.x = -UpLimit;
	}

	//回転行列作成
	MATRIX MatRotX = MGetRotX(m_CalcRot.x);
	MATRIX MatRotY = MGetRotY(m_CalcRot.y);

	//合成(Y→X)
	MATRIX MatRot = MMult(MatRotX, MatRotY);

	//相対ベクトル
	m_TargetPoint.y += TARGET_HEIGHT_ADD;
	VECTOR OffSet = CAMERA_OFFSET;

	//カメラ位置
	VECTOR CameraPosCalc = VTransform(OffSet, MatRot);
	m_CameraPoint = VAdd(m_TargetPoint, CameraPosCalc);

	m_CameraRot.y = m_CalcRot.y;

	//補間
	if (!_Player.GetIsGravity()) {
		m_CameraPos = CameraLerp(m_CameraPos, m_CameraPoint, LERP_RATE);
		m_TargetPos = CameraLerp(m_TargetPos, m_TargetPoint, LERP_RATE);
	}
	else {
		m_CameraPoint.y = m_CameraPos.y;
		m_TargetPoint.y = m_TargetPos.y;
		m_CameraPos = CameraLerp(m_CameraPos, m_CameraPoint, LERP_RATE);
		m_TargetPos = CameraLerp(m_TargetPos, m_TargetPoint, LERP_RATE);
	}
}
//更新処理
void PlayerCamera::Update() {
	SetCameraPositionAndTargetAndUpVec(m_CameraPos, m_TargetPos, m_UpVec);
	VECTOR SoundUpVecY = VScale(m_CameraPos, -1.0f);
	SoundUpVecY = VAdd(m_TargetPos, SoundUpVecY);
	Set3DSoundListenerPosAndFrontPos_UpVecY(m_TargetPos, SoundUpVecY);
}
//デバック用
void PlayerCamera::Draw() {
	DrawFormatString(50, 100, RED, "最終カメラ座標X:%f", m_CameraPoint.x);
	DrawFormatString(50, 125, RED, "最終カメラ座標Y:%f", m_CameraPoint.y);
	DrawFormatString(50, 150, RED, "最終カメラ座標Z:%f", m_CameraPoint.z);

	DrawFormatString(50, 200, RED, "カメラ座標X:%f", m_CameraPos.x);
	DrawFormatString(50, 225, RED, "カメラ座標Y:%f", m_CameraPos.y);
	DrawFormatString(50, 250, RED, "カメラ座標Z:%f", m_CameraPos.z);

	DrawFormatString(50, 300, RED, "最終注視点座標X:%f", m_TargetPoint.x);
	DrawFormatString(50, 325, RED, "最終注視点座標Y:%f", m_TargetPoint.y);
	DrawFormatString(50, 350, RED, "最終注視点座標Z:%f", m_TargetPoint.z);

	DrawFormatString(50, 400, RED, "注視点座標X:%f", m_TargetPos.x);
	DrawFormatString(50, 425, RED, "注視点座標Y:%f", m_TargetPos.y);
	DrawFormatString(50, 450, RED, "注視点座標Z:%f", m_TargetPos.z);
}
//現在座標から目標座標へ線形補間した座標を返す
VECTOR PlayerCamera::CameraLerp(VECTOR _CurrentPos, VECTOR _TargetPos, float _LerpRate) {
	//現在座標から目標座標への方向ベクトルを生成
	VECTOR MoveVec = VSub(_TargetPos, _CurrentPos);
	//移動量計算
	VECTOR AddVec = VScale(MoveVec, _LerpRate);
	//座標更新
	return VAdd(_CurrentPos, AddVec);
}

