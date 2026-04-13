#include "HPItem.h"

namespace {
	const char HP_PATH[] = "Data/UI/HpPotion.png";
}


//コンストラクタ
HPItem::HPItem() {
	Init();
}

//デストラクタ
HPItem::~HPItem() {
	Exit();
}

//初期化処理
void HPItem::Init() {
	ItemBase::Init();

	m_PosX = SCREEN_MAX_X - 100;
	m_PosY = SCREEN_MAX_Y - 100;

	m_Quantity = 5;				//アイテム個数
	m_MaxQuantity = m_Quantity;	//アイテム最大個数
}

// データ読み込み処理
void HPItem::Load() {
	ItemBase::Load(HP_PATH);
}

//アイテム使用処理
void HPItem::Use(Player& _Player) {
	if (m_Quantity >= 1) {
		m_Quantity -= 1;
		float HP = _Player.GetHitPoints();
		HP += 25;
		_Player.SetHitPoints(HP);
	}
}

//描画処理
void HPItem::Draw() {
	ItemBase::Draw();

	DrawFormatStringToHandle((int)m_PosX-25, (int)m_PosY + 50, WHITE, DxLibFont::FONTHNDL_N30, "×");
	DrawFormatStringToHandle((int)m_PosX, (int)m_PosY + 50, WHITE, DxLibFont::FONTHNDL_N30, "%d", m_Quantity);
}
