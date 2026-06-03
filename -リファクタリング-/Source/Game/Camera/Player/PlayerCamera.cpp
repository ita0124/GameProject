#include "PlayerCamera.h"

namespace {
	const VECTOR UP_VEC = { 0.0f,10.0f,0.0f };
	const float CALC_LEN = 5.0f;
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
	m_CameraPoint = VZERO;
	m_CalcRot = VZERO;

	//ゲーム開始時にプレイヤーの後頭部から始まるようにする
	m_CameraPoint = { -0.5f,25.0f,52.5f };
	m_CameraPos = { -0.5f,25.0f,52.5f };

	m_IsCameraPosSetEnd = false;
	m_IsTargetPosSetEnd = false;
}
//毎フレーム呼び出す処理(ノーマル)
void PlayerCamera::Step(Player& _Player) {
	m_TargetPoint = _Player.GetPos();
	float RotSpeed = 0.05f;
	float RotX = 0.0f;
	float RotY = 0.0f;

	if (InputKey::IsPushKeyRep(KEY_INPUT_UP)) {
		RotX += -RotSpeed;
	}
	else if (InputKey::IsPushKeyRep(KEY_INPUT_DOWN)) {
		RotX += RotSpeed;
	}

	if (InputKey::IsPushKeyRep(KEY_INPUT_LEFT)) {
		RotY += -RotSpeed;
	}
	else if (InputKey::IsPushKeyRep(KEY_INPUT_RIGHT)) {
		RotY += RotSpeed;
	}

	if (InputPad::GetRAnalogYInput() > 0.0f) {
		RotX += (float)InputPad::GetRAnalogYInput()*0.05f;
	}
	else if (InputPad::GetRAnalogYInput() < 0.0f) {
		RotX += (float)InputPad::GetRAnalogYInput() * 0.05f;
	}

	if (InputPad::GetRAnalogXInput() < 0.0f) {
		RotY += InputPad::GetRAnalogXInput() * 0.05f;
	}
	else if (InputPad::GetRAnalogXInput() > 0.0f) {
		RotY += InputPad::GetRAnalogXInput() * 0.05f;
	}

	m_CalcRot.x += RotX;
	m_CalcRot.y += RotY;

	float DownLimit = DX_PI_F * 0.0f / 180.0f;
	float UpLimit = DX_PI_F * 45.0f / 180.0f;
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
	m_TargetPoint.y+= 25.0f;
	VECTOR OffSet = VGet(0.0f, 25.0f, 100.0f);

	//カメラ位置
	VECTOR CameraPosCalc = VTransform(OffSet, MatRot);
	m_CameraPoint = VAdd(m_TargetPoint, CameraPosCalc);
	/*m_CameraPoint.y = 50.0f;*/

	/*m_TargetPos = m_TargetPoint;
	m_CameraPos = m_CameraPoint;*/
	m_CameraRot.y = m_CalcRot.y;

	if (!m_IsCameraPosSetEnd || !m_IsTargetPosSetEnd) {
		VECTOR v1 = VSub(m_CameraPoint, m_CameraPos);
		float f1 = VSize(v1);
		if (f1 > CALC_LEN) {
			v1 = VNorm(v1);
			v1 = VScale(v1, 5.0f);
			m_CameraPos = VAdd(m_CameraPos, v1);
		}
		else {
			m_CameraPos = m_CameraPoint;
			m_IsCameraPosSetEnd = true;
		}

		VECTOR v2 = VSub(m_TargetPoint, m_TargetPos);
		float f2 = VSize(v2);
		if (f2 > CALC_LEN) {
			v2 = VNorm(v2);
			v2 = VScale(v2, 5.0f);
			m_TargetPos = VAdd(m_TargetPos, v2);
		}
		else {
			m_TargetPos = m_TargetPoint;
			m_IsTargetPosSetEnd = true;
		}
	}
	else {
		m_CameraPos = m_CameraPoint;
		m_TargetPos = m_TargetPoint;
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
