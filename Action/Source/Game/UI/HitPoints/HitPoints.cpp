#include "HitPoints.h"

namespace {
	const char* FILE_PATH[HitPoints::TagGraphicsNum::GRAPHICS_NUM] = {			//画像パス
	"Data/UI/HitPoints.png",
	"Data/UI/Gray.png",
	};

	const int DIVIDE_NUM[HitPoints::TagGraphicsNum::GRAPHICS_NUM] = {			//画像の分割数
		1,
		1,
	};

	const int		GRAPHICS_SIZEX = 5;					//画像サイズ(X軸)
	const int		GRAPHICS_SIZEY = 25;				//画像サイズ(Y軸)

	const VECTOR	HEAD_POS = { 50.0f,50.0f,0.0f };	//先頭座標
	const int		MULT_X = 5;							//次の画像をどれだけ離すか(X軸)

	const int		MAX_NUM = 100;
}

//コンストラクタ
HitPoints::HitPoints() {
	Init();
}
//デストラクタ
HitPoints::~HitPoints() {
	Exit();
}
//初期化処理
void HitPoints::Init() {
	Object2DBase::Init();
	m_Pos = HEAD_POS;									//先頭座標設定
	m_HitPoints = 0;									//取得した体力を保存する
}
//データ読み込み処理
void HitPoints::Load() {
	for (int Index = 0;Index < GRAPHICS_NUM;Index++) {
		Object2DBase::Load(FILE_PATH[Index], DIVIDE_NUM[Index], 1, 1, GRAPHICS_SIZEX, GRAPHICS_SIZEY);
	}
}
//数値更新
void HitPoints::Step() {

}
//描画処理
void HitPoints::Draw() {
	if (!m_IsActive || m_Hndl.size() == 0)return;
	for (int Index = 0;Index < MAX_NUM;Index++) {
		int X = MULT_X * Index;
		DrawRotaGraph((int)m_Pos.x + X, (int)m_Pos.y, 1.0f, 0.0f, m_Hndl[GRAY][0], TRUE);
	}
	for (int Index = 0;Index < m_HitPoints;Index++) {
		int X = MULT_X * Index;
		DrawRotaGraph((int)m_Pos.x + X, (int)m_Pos.y, 1.0f, 0.0f, m_Hndl[HIT_POINTS][0], TRUE);
	}
}
