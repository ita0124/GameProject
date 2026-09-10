#include "TitleCamera.h"

namespace {
	constexpr VECTOR	UP_VEC = { 0.0f,10.0f,0.0f };
	constexpr float		CAMERA_POS_LERP_RATE = 0.01f;				//１フレームの補完率
	constexpr float		TARGET_POS_LERP_RATE = 0.01f;				//１フレームの補完率
	constexpr VECTOR	CAMERA_OFFSET = { 0.0f,25.0f,100.0f };		// ターゲットから見たカメラの相対位置

	constexpr VECTOR	PART_ONE_INIT_POS = { 3030.0f,360.0f,-3030.0f };
	constexpr VECTOR	PART_ONE_INIT_ROT = { 0.0f,90.0f * RADIAN_CALC,0.0f };
	constexpr float		PART_ONE_MOVE_X = 0.5f;

	constexpr VECTOR	PART_TWO_INIT_POS = { 0.0f,340.0f,-3220.0f };
	constexpr VECTOR	PART_TWO_INIT_ROT = { 0.0f,180.0f * RADIAN_CALC,0.0f };
	constexpr float		PART_TWO_MOVE_Z = 0.5f;
	constexpr float		PART_TWO_END_Z = -2660.0f;

	constexpr VECTOR	PART_THREE_INIT_POS = { 0.0f,340.0f,-2660.0f };
	constexpr VECTOR	PART_THREE_INIT_ROT = { -90.0f * RADIAN_CALC,0.0f,0.0f };
	constexpr float		PART_THREE_MOVE_Y = 0.5f;
	constexpr float		PART_THREE_END_Y = 120.0f;

	constexpr VECTOR	PART_FOUR_INIT_POS = { 0.0f,120.0f,-2660.0f };
	constexpr VECTOR	PART_FOUR_INIT_ROT = { 0.0f,180.0f * RADIAN_CALC,0.0f };
	constexpr float		PART_FOUR_MOVE_Z = 0.5f;

	constexpr VECTOR	PART_FIVE_INIT_POS = { 0.0f,30.0,00.0f };
	constexpr VECTOR	PART_FIVE_INIT_ROT = { 0.0f,0.0f,0.0f };
	constexpr int		PART_FIVE_WAIT_TIME = 300;
}

//コンストラクタ
TitleCamera::TitleCamera() {
	Init();
}
//初期化
void TitleCamera::Init() {
	m_CameraPoint = PART_ONE_INIT_POS;			//カメラ位置
	m_CameraPos = PART_ONE_INIT_POS;
	m_TargetPoint = PART_ONE_INIT_POS;			//注視点
	m_TargetPos = PART_ONE_INIT_POS;
	m_UpVec = UP_VEC;							//？？？
	m_CameraRot = PART_ONE_INIT_ROT;
	m_CalcRot = PART_ONE_INIT_ROT;
	m_State = PART_ONE;
	m_PartFiveWaitTime = 0;

	Update();
}
//プレイヤー
void TitleCamera::Step() {
	switch (m_State) {
	case PART_ONE:
		PartOne();
		break;
	case PART_TWO:
		PartTwo();
		break;
	case PART_THREE:
		PartThree();
		break;
	case PART_FOUR:
		PartFour();
		break;
	case PART_FIVE:
		PartFive();
		break;
	}

	//回転行列作成
	MATRIX MatRotX = MGetRotX(m_CalcRot.x);
	MATRIX MatRotY = MGetRotY(m_CalcRot.y);

	//合成(Y→X)
	MATRIX MatRot = MMult(MatRotX, MatRotY);

	//相対ベクトル
	VECTOR OffSet = CAMERA_OFFSET;

	//カメラ位置
	VECTOR CameraPosCalc = VTransform(OffSet, MatRot);
	m_CameraPoint = VAdd(m_TargetPoint, CameraPosCalc);
	//カメラのY軸回転角を設定
	m_CameraRot.y = m_CalcRot.y;
	//現在の注視点と目標注視点の高低差を取得
	float TargetPosYDif = fabsf(m_TargetPoint.y - m_TargetPos.y);
	//カメラ座標を目標座標へ補間
	m_CameraPos = CameraLerp(m_CameraPos, m_CameraPoint, CAMERA_POS_LERP_RATE);
	//注視点座標を目標座標へ補間
	m_TargetPos = CameraLerp(m_TargetPos, m_TargetPoint, TARGET_POS_LERP_RATE);

	if (m_CameraPos.y <= 0.0f) {
		m_CameraPos.y = 0.0f;
	}
}
//更新処理
void TitleCamera::Update() {
	SetCameraPositionAndTargetAndUpVec(m_CameraPos, m_TargetPos, m_UpVec);
	VECTOR SoundUpVecY = VScale(m_CameraPos, -1.0f);
	SoundUpVecY = VAdd(m_TargetPos, SoundUpVecY);
	Set3DSoundListenerPosAndFrontPos_UpVecY(m_TargetPos, SoundUpVecY);
}
//デバック用
void TitleCamera::Draw() {
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
VECTOR TitleCamera::CameraLerp(VECTOR _CurrentPos, VECTOR _TargetPos, float _LerpRate) {
	//現在座標から目標座標への方向ベクトルを生成
	VECTOR MoveVec = VSub(_TargetPos, _CurrentPos);
	//移動量計算
	VECTOR AddVec = VScale(MoveVec, _LerpRate);
	//座標更新
	return VAdd(_CurrentPos, AddVec);
}
//まとめ関数
void TitleCamera::PartOne() {
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//ターゲット座標を設定
		m_TargetPoint = PART_ONE_INIT_POS;
	}
	m_TargetPoint.x += -PART_ONE_MOVE_X;

	if (m_TargetPoint.x <= 0.0f) {
		m_State = PART_TWO;
	}
}
void TitleCamera::PartTwo() {
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//ターゲット座標を設定
		m_TargetPoint = PART_TWO_INIT_POS;
		//
		m_CalcRot = PART_TWO_INIT_ROT;
	}
	m_TargetPoint.z += PART_TWO_MOVE_Z;

	if (m_TargetPoint.z >= PART_TWO_END_Z) {
		m_State = PART_THREE;
	}
}
void TitleCamera::PartThree() {
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//ターゲット座標を設定
		m_TargetPoint = PART_THREE_INIT_POS;
		//
		m_CalcRot = PART_THREE_INIT_ROT;
	}
	m_TargetPoint.y += -PART_THREE_MOVE_Y;

	if (m_TargetPoint.y <= PART_THREE_END_Y) {
		m_State = PART_FOUR;
	}
}
void TitleCamera::PartFour() {
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//ターゲット座標を設定
		m_TargetPoint = PART_FOUR_INIT_POS;
		//
		m_CalcRot = PART_FOUR_INIT_ROT;
	}
	m_TargetPoint.z += PART_FOUR_MOVE_Z;

	if (m_TargetPoint.z >= 0.0f) {
		m_State = PART_FIVE;
	}
}
void TitleCamera::PartFive() {
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//ターゲット座標を設定
		m_TargetPoint = PART_FIVE_INIT_POS;
		//
		m_CalcRot = PART_FIVE_INIT_ROT;
		//
		m_PartFiveWaitTime = 0;
	}
	if (m_PartFiveWaitTime >= PART_FIVE_WAIT_TIME) {
		Init();
	}
	else {
		m_PartFiveWaitTime++;
	}
}
