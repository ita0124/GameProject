#include "ItemBase.h"

//コンストラクタ
ItemBase::ItemBase() {
	//画像ハンドル
	m_Hndl = -1;

	Init();
}

//デストラクタ
ItemBase::~ItemBase() {
	Exit();
}

// データ読み込み処理
void ItemBase::Load(const char* FilePath) {
	if (m_Hndl == -1)
	{
		//画像データ読み込み
		//失敗していたら-1が変数に入る
		m_Hndl = LoadGraph(FilePath);
	}
}

//初期化処理
void ItemBase::Init() {
	m_PosX = FZERO;			//座標X
	m_PosY = FZERO;			//座標Y

	m_Quantity = 0;			//アイテム個数
	m_MaxQuantity = 0;		//アイテム最大個数

	m_IsUse = false;		//使用判定
}

//データ破棄処理
void ItemBase::Exit() {
	if (m_Hndl == -1)return;
	DeleteGraph(m_Hndl);
	m_Hndl = -1;
}

//描画処理
void ItemBase::Draw() {
	DrawRotaGraph((int)m_PosX, (int)m_PosY, 5.0f, 0.0f, m_Hndl, TRUE);
}
