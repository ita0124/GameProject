#include "SkillPoints.h"

namespace {
	constexpr const char* FILE_PATH[SkillPoints::TagGraphicsNum::GRAPHICS_NUM] = {			//画像パス
	"Data/UI/SkillPoints.png",
	"Data/UI/Gray.png",
	"Data/UI/SkillPointsIcon.png",
	"Data/UI/SkillPointsDecoration.png",
	};

	constexpr int DIVIDE_NUM[SkillPoints::TagGraphicsNum::GRAPHICS_NUM] = {			//画像の分割数
		1,
		1,
		1,
		1,
	};

	constexpr int		GRAPHICS_SIZEX = 4;					//画像サイズ(X軸)
	constexpr int		GRAPHICS_SIZEY = 24;				//画像サイズ(Y軸)

	constexpr int		IMAGE_SPACING_X = 4;				//次の画像をどれだけ離すか(X軸)

	constexpr int		TEXT_POS_X = 330;					//ステータス表示位置のX座標
	constexpr int		TEXT_POS_Y = -30;					//ステータス表示位置のY座標
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
	StatusDrawBase::Init();
}
//初期化処理
void SkillPoints::Init(VECTOR _Pos, float _MaxStatus) {
	Init();

	m_Pos = _Pos;
	m_MaxStatus = _MaxStatus;
	m_ImageSpacing = IMAGE_SPACING_X;
}
//データ読み込み処理
void SkillPoints::Load() {
	for (int Index = 0;Index < GRAPHICS_NUM;Index++) {
		Object2DBase::Load(FILE_PATH[Index], DIVIDE_NUM[Index], 1, 1, GRAPHICS_SIZEX, GRAPHICS_SIZEY);
	}
}
//描画処理
void SkillPoints::Draw() {
	StatusDrawBase::Draw();

	//DrawFormatStringToHandle((int)m_Pos.x + TEXT_POS_X, (int)m_Pos.y + TEXT_POS_Y, WHITE, DxLibFont::FONTHNDL_N20, "%d/%d", (int)m_Status, (int)m_MaxStatus);
	//DrawFormatStringToHandle((int)m_Pos.x, (int)m_Pos.y + TEXT_POS_Y, WHITE, DxLibFont::FONTHNDL_N20, "SkillPoints");
}
