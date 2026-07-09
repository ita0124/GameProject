#include "Game/UI/Stamina/Stamina.h"

namespace {
	constexpr const char* FILE_PATH[Stamina::TagGraphicsNum::GRAPHICS_NUM] = {			//画像パス
	"Data/UI/Stamina.png",
	"Data/UI/Gray.png",
	};

	constexpr int DIVIDE_NUM[Stamina::TagGraphicsNum::GRAPHICS_NUM] = {			//画像の分割数
		1,
		1,
	};

	constexpr int		GRAPHICS_SIZEX = 5;					//画像サイズ(X軸)
	constexpr int		GRAPHICS_SIZEY = 25;				//画像サイズ(Y軸)

	constexpr VECTOR	HEAD_POS = { 50.0f,100.0f,0.0f };	//先頭座標
	constexpr int		MULT_X = 5;							//次の画像をどれだけ離すか(X軸)

	constexpr int		MAX_NUM = 100;
}

//コンストラクタ
Stamina::Stamina() {
	Init();
}
//デストラクタ
Stamina::~Stamina() {
	Exit();
}
//初期化処理
void Stamina::Init() {
	Object2DBase::Init();
	m_Pos = HEAD_POS;									//先頭座標設定
	m_Stamina = 0;									//取得したスキルポイントを保存する
}
//データ読み込み処理
void Stamina::Load() {
	for (int Index = 0;Index < GRAPHICS_NUM;Index++) {
		Object2DBase::Load(FILE_PATH[Index], DIVIDE_NUM[Index], 1, 1, GRAPHICS_SIZEX, GRAPHICS_SIZEY);
	}
}
//数値更新
void Stamina::Step() {

}
//描画処理
void Stamina::Draw() {
	if (!m_IsActive || m_Hndl.size() == 0)return;
	for (int Index = 0;Index < MAX_NUM;Index++) {
		int X = MULT_X * Index;
		DrawRotaGraph((int)m_Pos.x + X, (int)m_Pos.y, 1.0f, 0.0f, m_Hndl[GRAY][0], TRUE);
	}
	for (int Index = 0;Index < m_Stamina;Index++) {
		int X = MULT_X * Index;
		DrawRotaGraph((int)m_Pos.x + X, (int)m_Pos.y, 1.0f, 0.0f, m_Hndl[STAMINA][0], TRUE);
	}
}
