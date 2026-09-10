#include "DropItemManager.h"

namespace {
	constexpr const char* DROP_ITEM_FILE_PATH[DropItemBase::TagDropItemKinds::DROP_ITEM_NUM] = {				//モデルファイルパス
		"Data/Model/DropItem/Coin/Coin.mv1",
		"Data/Model/DropItem/HitPoints/HitPoints.mv1",
		"Data/Model/DropItem/SkillPoints/SkillPoints.mv1",
	};

	constexpr int COINT_INDEX = DROP_ITEM_MAX / 3;
	constexpr int HIT_POINTS_INDEX = (int)(DROP_ITEM_MAX / 1.5f);
}

//初期化処理
void DropItemManager::Init() {
	//初期化
	for (int Index = 0; Index < DROP_ITEM_MAX; Index++) {
		m_DropItem[Index] = nullptr;
	}
	//newする
	for (int DropItemIndex = 0; DropItemIndex < COINT_INDEX; DropItemIndex++) {
		//ドロップコインをnew
		m_DropItem[DropItemIndex] = new DropCoin;
	}
	for (int DropItemIndex = COINT_INDEX; DropItemIndex < HIT_POINTS_INDEX; DropItemIndex++) {
		//ドロップコインをnew
		m_DropItem[DropItemIndex] = new DropHitPoints;
	}
	for (int DropItemIndex = HIT_POINTS_INDEX; DropItemIndex < DROP_ITEM_MAX; DropItemIndex++) {
		//ドロップコインをnew
		m_DropItem[DropItemIndex] = new DropSkillPoints;
	}
	//初期化をここで行う
	for (int DropItemIndex = 0; DropItemIndex < DROP_ITEM_MAX; DropItemIndex++) {
		//nullなら行わない
		if (m_DropItem[DropItemIndex] != nullptr) {
			m_DropItem[DropItemIndex]->Init();
		}
	}
}
//データ破棄処理
void DropItemManager::Exit() {
	for (int DropItemIndex = 0; DropItemIndex < DROP_ITEM_MAX; DropItemIndex++) {
		//nullなら行わない
		if (m_DropItem[DropItemIndex] != nullptr) {
			//データ破棄
			m_DropItem[DropItemIndex]->Exit();
			//newを消す
			delete m_DropItem[DropItemIndex];
			//nullを入れておく
			m_DropItem[DropItemIndex] = nullptr;
		}
	}
}
//データ読み込み処理
void DropItemManager::Load() {
	int Hndl[DropItemBase::TagDropItemKinds::DROP_ITEM_NUM] = {};
	//
	for (int LoadIndex = 0;LoadIndex < DropItemBase::TagDropItemKinds::DROP_ITEM_NUM;LoadIndex++) {
		Hndl[LoadIndex] = MV1LoadModel(DROP_ITEM_FILE_PATH[LoadIndex]);
	}
	for (int DropItemIndex = 0; DropItemIndex < DROP_ITEM_MAX; DropItemIndex++) {
		//nullなら行わない
		if (m_DropItem[DropItemIndex] != nullptr) {
			m_DropItem[DropItemIndex]->Load(Hndl[m_DropItem[DropItemIndex]->GetTagDropItemKinds()]);
		}
	}
}
//描画処理
void DropItemManager::Draw() {
	for (int DropItemIndex = 0; DropItemIndex < DROP_ITEM_MAX; DropItemIndex++) {
		//nullなら行わない
		if (m_DropItem[DropItemIndex] != nullptr && m_DropItem[DropItemIndex]->GetIsActive()) {
			m_DropItem[DropItemIndex]->Draw();
		}
	}
}
//毎フレーム呼び出す処理
void DropItemManager::Step() {
	for (int DropItemIndex = 0; DropItemIndex < DROP_ITEM_MAX; DropItemIndex++) {
		//nullなら行わない
		if (m_DropItem[DropItemIndex] != nullptr && m_DropItem[DropItemIndex]->GetIsActive()) {
			m_DropItem[DropItemIndex]->Step();
		}
	}
}
//モデル更新処理
void DropItemManager::Update() {
	for (int DropItemIndex = 0; DropItemIndex < DROP_ITEM_MAX; DropItemIndex++) {
		//nullなら行わない
		if (m_DropItem[DropItemIndex] != nullptr && m_DropItem[DropItemIndex]->GetIsActive()) {
			m_DropItem[DropItemIndex]->Update();
		}
	}
}
//当たり判定後の処理
void DropItemManager::HitCalc(const int& _Num, ObjectBase* _Object) {
	if (m_DropItem[_Num] != nullptr) {
		m_DropItem[_Num]->HitCalc(_Object);
	}
}
