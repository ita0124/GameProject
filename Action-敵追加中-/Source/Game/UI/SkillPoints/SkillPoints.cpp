#include "SkillPoints.h"

namespace {
	constexpr const char* FILE_PATH[SkillPoints::TagGraphicsNum::GRAPHICS_NUM] = {			//画像パス
	"Data/UI/SkillPoints.png",
	"Data/UI/Gray.png",
	};

	constexpr int DIVIDE_NUM[SkillPoints::TagGraphicsNum::GRAPHICS_NUM] = {			//画像の分割数
		1,
		1,
	};

	constexpr int		GRAPHICS_SIZEX = 5;					//画像サイズ(X軸)
	constexpr int		GRAPHICS_SIZEY = 25;				//画像サイズ(Y軸)

	constexpr VECTOR	HEAD_POS = { 50.0f,150.0f,0.0f };	//先頭座標
	constexpr int		MULT_X = 5;							//次の画像をどれだけ離すか(X軸)

	constexpr int		MAX_NUM = 100;
}

//コンストラクタ
SkillPoints::SkillPoints() {
	Init();
}
//デストラクタ
SkillPoints::~SkillPoints() {
	Exit();
}
//初期化処理
void SkillPoints::Init() {
	Object2DBase::Init();
	m_Pos = HEAD_POS;									//先頭座標設定
	m_SkillPoints = 0;									//取得したスキルポイントを保存する
}
//データ読み込み処理
void SkillPoints::Load() {
	for (int Index = 0;Index < GRAPHICS_NUM;Index++) {
		Object2DBase::Load(FILE_PATH[Index], DIVIDE_NUM[Index], 1, 1, GRAPHICS_SIZEX, GRAPHICS_SIZEY);
	}
}
//数値更新
void SkillPoints::Step() {

}
//描画処理
void SkillPoints::Draw() {
	if (!m_IsActive || m_Hndl.size() == 0)return;
	for (int Index = 0;Index < MAX_NUM;Index++) {
		int X = MULT_X * Index;
		DrawRotaGraph((int)m_Pos.x + X, (int)m_Pos.y, 1.0f, 0.0f, m_Hndl[GRAY][0], TRUE);
	}
	for (int Index = 0;Index < m_SkillPoints;Index++) {
		int X = MULT_X * Index;
		DrawRotaGraph((int)m_Pos.x + X, (int)m_Pos.y, 1.0f, 0.0f, m_Hndl[SKILL_POINTS][0], TRUE);
	}
}
