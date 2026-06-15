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

	Update();
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

	if (!_Player.GetIsAction(Player::TagState::SKILL_ATTACK)) {
		m_CameraRot.y = m_CalcRot.y;
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
