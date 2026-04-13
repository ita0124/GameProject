#include "StaminaItem.h"

namespace {
	static const char STAMINA_PATH[] = "Data/UI/StaminaPotion.png";
}

//コンストラクタ
StaminaItem::StaminaItem() {
	Init();
}

//デストラクタ
StaminaItem::~StaminaItem() {
	Exit();
}

//初期化処理
void StaminaItem::Init() {
	ItemBase::Init();

	m_PosX = SCREEN_MAX_X - 100;
	m_PosY = SCREEN_MAX_Y - 100;

	m_Quantity = 5;				//アイテム個数
	m_MaxQuantity = m_Quantity;	//アイテム最大個数
}

// データ読み込み処理
void StaminaItem::Load() {
	ItemBase::Load(STAMINA_PATH);
}

//アイテム使用処理
void StaminaItem::Use(Player& _Player) {
	if (m_Quantity >= 1) {
		m_Quantity -= 1;
		float Stamina = _Player.GetStamina();
		Stamina += 10;
		_Player.SetStamina(Stamina);
	}
}

//描画処理
void StaminaItem::Draw() {
	ItemBase::Draw();

	DrawFormatStringToHandle((int)m_PosX - 25, (int)m_PosY + 50, WHITE, DxLibFont::FONTHNDL_N30, "×");
	DrawFormatStringToHandle((int)m_PosX, (int)m_PosY + 50, WHITE, DxLibFont::FONTHNDL_N30, "%d", m_Quantity);
}
