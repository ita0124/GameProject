#include "TargetCamera.h"

namespace {
	const float TARGET_POS = 50.0f;
	const VECTOR UP_VEC = { 0.0f,10.0f,0.0f };
	const float CALC_LEN = 5.0f;
}

//コンストラクタ
TargetCamera::TargetCamera() {
	Init();
}
//初期化
void TargetCamera::Init() {
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
//ロックオン
void TargetCamera::Step(Player& _Player) {
	//プレイヤー→敵の方向から回転角を作る
	VECTOR Dir = VSub(_Player.GetAttackTargetPos(), _Player.GetPos());
	Dir = VNorm(Dir);

	//Y軸回転
	m_CalcRot.y = atan2f(-Dir.x, -Dir.z);

	//X軸回転
	m_CalcRot.x = atan2f(Dir.y, sqrtf(Dir.x * Dir.x + Dir.z * Dir.z));

	//プレイヤー背面のオフセットを作る
	VECTOR Offset = VGet(0.0f, 20.0f, 100.0f);

	//回転行列を作る
	MATRIX MatRotX = MGetRotX(m_CalcRot.x);
	MATRIX MatRotY = MGetRotY(m_CalcRot.y);

	//X回転→Y回転
	MATRIX MatRot = MMult(MatRotX, MatRotY);

	//オフセットを回転させる
	Offset = VTransform(Offset, MatRot);

	//プレイヤー座標に足してカメラ座標へ
	m_CameraPoint = VAdd(_Player.GetPos(), Offset);

	//地面めり込み防止
	if (m_CameraPoint.y < 20.0f) {
		m_CameraPoint.y = 20.0f;
	}

	//_TargetPos代入
	m_TargetPoint = _Player.GetAttackTargetPos();
	m_TargetPoint.y -= 20.0f;

	/*m_TargetPos = m_TargetPoint;
	m_CameraPos = m_CameraPoint;*/

	if (!_Player.GetIsAction(Player::TagState::SKILL_ATTACK)) {
		m_CameraRot.y = m_CalcRot.y;
	}

	if (!m_IsCameraPosSetEnd || !m_IsTargetPosSetEnd) {
		VECTOR v1 = VSub(m_CameraPoint, m_CameraPos);

		float f1 = VSize(v1);
		if (f1 > CALC_LEN) {
			v1 = VNorm(v1);
			v1 = VScale(v1, 5.0f);
			m_CameraPos = VAdd(m_CameraPos, v1);
		}
		else {
			m_IsCameraPosSetEnd = true;
			m_CameraPos = m_CameraPoint;
		}

		VECTOR v2 = VSub(m_TargetPoint, m_TargetPos);
		float f2 = VSize(v2);
		if (f2 > CALC_LEN) {
			v2 = VNorm(v2);
			v2 = VScale(v2, 5.0f);
			m_TargetPos = VAdd(m_TargetPos, v2);
		}
		else {
			m_IsTargetPosSetEnd = true;
			m_TargetPos = m_TargetPoint;
		}
	}
	else {
		m_TargetPos = m_TargetPoint;
		m_CameraPos = m_CameraPoint;
	}
}
//更新処理
void TargetCamera::Update() {
	SetCameraPositionAndTargetAndUpVec(m_CameraPos, m_TargetPos, m_UpVec);
	VECTOR SoundUpVecY = VScale(m_CameraPos, -1.0f);
	SoundUpVecY = VAdd(m_TargetPos, SoundUpVecY);
	Set3DSoundListenerPosAndFrontPos_UpVecY(m_TargetPos, SoundUpVecY);
}
//デバック用
void TargetCamera::Draw() {
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
