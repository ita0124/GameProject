#include "ItemManager.h"

//コンストラクタ
ItemManager::ItemManager() {
	Init();
}

//デストラクタ
ItemManager::~ItemManager() {
	Exit();
}

//初期化処理
void ItemManager::Init() {
	for (int i = 0; i < SCENE_NUM; i++) {
		m_Item[i] = nullptr;
	}

	m_ID = HP;

	//newする
	//HP
	if (m_Item[HP] == nullptr) {
		m_Item[HP] = new HPItem;
	}
	//スタミナ
	if (m_Item[STAMINA] == nullptr) {
		m_Item[STAMINA] = new StaminaItem;
	}
}

// データ読み込み処理
void ItemManager::Load() {
	for (int i = 0; i < SCENE_NUM; i++) {
		m_Item[i]->Load();
	}
}

//データ破棄処理
void ItemManager::Exit() {
	for (int i = 0; i < SCENE_NUM; i++) {
		if (m_Item[i] != nullptr) {
			m_Item[i]->Exit();
			delete m_Item[i];
			m_Item[i] = nullptr;
		}
	}
}

//実行中繰り返し行われる処理管理関数
void ItemManager::Step(Player& _Player) {

	if (InputKey::IsPushKeyTrg(KEY_INPUT_J) || InputPad::IsPushPadTrg(XINPUT_BUTTON_DPAD_LEFT)) {
		int Num = m_ID;
		Num = Num - 1;
		if (Num >= HP) {
			m_ID = (TagItem)(Num);
		}
	}
	if (InputKey::IsPushKeyTrg(KEY_INPUT_K) || InputPad::IsPushPadTrg(XINPUT_BUTTON_DPAD_RIGHT)) {
		int Num = m_ID;
		Num = Num + 1;
		if (Num <= SCENE_NUM - 1) {
			m_ID = (TagItem)(Num);
		}
	}

	if (InputKey::IsPushKeyTrg(KEY_INPUT_U) || InputPad::IsPushPadTrg(XINPUT_BUTTON_DPAD_UP)) {
		m_Item[m_ID]->Use(_Player);
	}
}

//描画処理管理
void ItemManager::Draw() {
	m_Item[m_ID]->Draw();
}
