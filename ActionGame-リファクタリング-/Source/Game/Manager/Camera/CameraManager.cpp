#include "CameraManager.h"

//コンストラクタ
CameraManager::CameraManager() {
	Init();
}

//初期化
void CameraManager::Init() {
	m_Camera = PLAY;
	m_Target = PLAYER;

	m_PlayCamera.Init();
	m_DebugCamera.Init();
	SetCameraNearFar(1.0f, 5000.0f);
}

//毎フレーム呼び出す処理
void CameraManager::Step(VECTOR _PlayerPos, VECTOR _TargetEnemyPos, bool _IsSPAtk) {
	switch (m_Camera) {
	case PLAY:
		switch (m_Target) {
		case PLAYER:
			m_PlayCamera.Step(_PlayerPos);
			break;
		case ENEMY:
			m_PlayCamera.Step(_TargetEnemyPos, _PlayerPos, _IsSPAtk);
		}
		break;
	case DEBUG:
		m_DebugCamera.Step();
		break;
	}
}

//更新処理
void CameraManager::Update() {
	switch (m_Camera) {
	case PLAY:
		m_PlayCamera.Update();
		break;
	case DEBUG:
		m_DebugCamera.Update();
	}
}
