#include "TargetCamera.h"

namespace {
	constexpr VECTOR	UP_VEC = { 0.0f,10.0f,0.0f };
	constexpr float		CAMERA_POS_LERP_RATE = 0.2f;				//１フレームの補完率
	constexpr float		TARGET_POS_LERP_RATE = 0.2f;				//１フレームの補完率
	constexpr VECTOR	CAMERA_OFFSET = { 0.0f,25.0f,100.0f };		// プレイヤー基準のカメラ配置オフセット
	constexpr float		CAMERA_MIN_HEIGHT = 20.0f;					// カメラの最低高度
	constexpr float		TARGET_HEIGHT_OFFSET = 10.0f;				// ロックオン注視点の高さ補正
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
	m_UpVec = UP_VEC;				//？？？
	m_CameraRot = VZERO;
	m_CameraPoint = VZERO;
	m_CalcRot = VZERO;

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
	VECTOR Offset = CAMERA_OFFSET;

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
	if (m_CameraPoint.y < CAMERA_MIN_HEIGHT) {
		m_CameraPoint.y = CAMERA_MIN_HEIGHT;
	}

	//_TargetPos代入
	m_TargetPoint = _Player.GetAttackTargetPos();
	m_TargetPoint.y += TARGET_HEIGHT_OFFSET;

	if (_Player.GetState() != Player::TagState::SKILL_ATTACK) {
		m_CameraRot.y = m_CalcRot.y;
	}

	//補間
	m_CameraPos = CameraLerp(m_CameraPos, m_CameraPoint, CAMERA_POS_LERP_RATE);
	m_TargetPos = CameraLerp(m_TargetPos, m_TargetPoint, TARGET_POS_LERP_RATE);
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

	DrawSphere3D(m_TargetPoint, 10.0f, DIV, RED, RED, false);

	DrawFormatString(50, 400, RED, "注視点座標X:%f", m_TargetPos.x);
	DrawFormatString(50, 425, RED, "注視点座標Y:%f", m_TargetPos.y);
	DrawFormatString(50, 450, RED, "注視点座標Z:%f", m_TargetPos.z);
}
//現在座標から目標座標へ線形補間した座標を返す
VECTOR TargetCamera::CameraLerp(VECTOR _CurrentPos, VECTOR _TargetPos, float _LerpRate) {
	//現在座標から目標座標への方向ベクトルを生成
	VECTOR MoveVec = VSub(_TargetPos, _CurrentPos);
	//移動量計算
	VECTOR AddVec = VScale(MoveVec, _LerpRate);
	//座標更新
	return VAdd(_CurrentPos, AddVec);
}
