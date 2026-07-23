#include "CameraManager.h"

//コンストラクタ
CameraManager::CameraManager() {
	Init();
}
//初期化
void CameraManager::Init() {
	m_Camera = PLAYER;

	m_PlayCamera.Init();
	SetCameraNearFar(1.0f, 5000.0f);
}
//毎フレーム呼び出す処理
void CameraManager::Step(Player& _Player) {
	switch (m_Camera) {
	case PLAYER:
		m_PlayCamera.Step(_Player);
		break;
	case TARGET:
		m_TargetCamera.Step(_Player);
		break;
	}
}
//更新処理
void CameraManager::Update() {
	switch (m_Camera) {
	case PLAYER:
		m_PlayCamera.Update();
		break;
	case TARGET:
		m_TargetCamera.Update();
		break;
	}
}
void CameraManager::ChangeCamera(TagCamera _Camera) {
	m_Camera = _Camera;
	switch (m_Camera) {
	case PLAYER:
		//カメラ位置
		m_PlayCamera.SetCameraPos(m_TargetCamera.GetCameraPos());
		//注視点
		m_PlayCamera.SetTargetPos(m_TargetCamera.GetTargetPos());
		//カメラ回転角度
		m_PlayCamera.SetCameraRot(m_TargetCamera.GetCameraRot());
		break;
	case TARGET:
		//カメラ位置
		m_TargetCamera.SetCameraPos(m_PlayCamera.GetCameraPos());
		//注視点
		m_TargetCamera.SetTargetPos(m_PlayCamera.GetTargetPos());
		//カメラ回転角度
		m_TargetCamera.SetCameraRot(m_PlayCamera.GetCameraRot());
		break;
	}
}
VECTOR CameraManager::GetCameraRot() {
	VECTOR Rot = VZERO;
	switch (m_Camera) {
	case PLAYER:
		Rot = m_PlayCamera.GetCameraRot();
		break;
	case TARGET:
		Rot = m_TargetCamera.GetCameraRot();
		break;
	}
	return Rot;
}
VECTOR CameraManager::GetCameraPos() {
	VECTOR Pos = VZERO;
	switch (m_Camera) {
	case PLAYER:
		Pos = m_PlayCamera.GetCameraPos();
		break;
	case TARGET:
		Pos = m_TargetCamera.GetCameraPos();
		break;
	}
	return Pos;
}
VECTOR CameraManager::GetTargetPos() {
	VECTOR Pos = VZERO;
	switch (m_Camera) {
	case PLAYER:
		Pos = m_PlayCamera.GetTargetPos();
		break;
	case TARGET:
		Pos = m_TargetCamera.GetTargetPos();
		break;
	}
	return Pos;
}
void CameraManager::Draw() {
	switch (m_Camera) {
	case PLAYER:
		return m_PlayCamera.Draw();
		break;
	case TARGET:
		return m_TargetCamera.Draw();
		break;
	}
}
