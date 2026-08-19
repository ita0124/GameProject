#include "BossGimmickManager.h"

namespace {

	constexpr const VECTOR CRYSTAL_REQUEST_POS[CRYSTAL_MAX]{
		{250.0f,0.0f,250.0f},
		{-250.0f,0.0f,250.0f},
		{250.0f,0.0f,-250.0f},
		{-250.0f,0.0f,-250.0f}
	};
}

//コンストラクタ
BossGimmickManager::BossGimmickManager() {
	Init();
}
//デストラクタ
BossGimmickManager::~BossGimmickManager() {
	Exit();
}
//初期化処理
void BossGimmickManager::Init(Boss* _Owner) {
	//オーナー変数がnullなら
	if (m_Owner == nullptr) {
		m_Owner = _Owner;
	}
	//クリスタル
	for (int Index = 0;Index < CRYSTAL_MAX;Index++) {
		m_Crystal[Index].Init();
	}
}
//データ破棄処理
void BossGimmickManager::Exit() {
	//クリスタル
	for (int Index = 0;Index < CRYSTAL_MAX;Index++) {
		m_Crystal[Index].Exit();
	}
}
//データ読み込み処理
void BossGimmickManager::Load() {
	//クリスタル
	for (int Index = 0;Index < CRYSTAL_MAX;Index++) {
		m_Crystal[Index].Load();
	}
}
//描画処理
void BossGimmickManager::Draw() {
	//クリスタル
	for (int Index = 0;Index < CRYSTAL_MAX;Index++) {
		m_Crystal[Index].Draw();
	}
}
//毎フレーム呼び出す処理
void BossGimmickManager::Step() {
	//リクエスト処理
	Request();
	//クリスタル
	//生存しているクリスタルの数を保存する
	int CrystalCount = 0;
	for (int Index = 0;Index < CRYSTAL_MAX;Index++) {
		//生存フラグがオフなら実行しない
		if (!m_Crystal[Index].GetIsActive())continue;
		m_Crystal[Index].Step();
		//生存数を加算
		CrystalCount++;
	}
	//生存しているクリスタルの数を設定
	m_Owner->SetCrystalCount(CrystalCount);
}
//モデル更新処理
void BossGimmickManager::Update() {
	//クリスタル
	for (int Index = 0;Index < CRYSTAL_MAX;Index++) {
		m_Crystal[Index].Update();
	}
}

//リクエスト処理
void BossGimmickManager::Request() {
	if (m_Owner->GetIsCrystalRequest()) {
		//クリスタル
		for (int Index = 0;Index < CRYSTAL_MAX;Index++) {
			m_Crystal[Index].Request(CRYSTAL_REQUEST_POS[Index]);
		}
	}
	if (m_Owner->GetIsCrystalDeathRequest()) {
		//クリスタル
		for (int Index = 0;Index < CRYSTAL_MAX;Index++) {
			m_Crystal[Index].SetIsActive(false);
		}
	}
}
//初期化処理
void BossGimmickManager::Init() {
	m_Owner = nullptr;
}
