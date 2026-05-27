#include "LoadMaterial.h"

int LoadMaterial::MATERIAL_BLACK;
int LoadMaterial::MATERIAL_BLUE;
int LoadMaterial::MATERIAL_RED;
int LoadMaterial::MATERIAL_GRADATION_BLUE;
int LoadMaterial::MATERIAL_GRADATION_RED;
int LoadMaterial::MATERIAL_GRADATION_YELLOW;
int LoadMaterial::MATERIAL_DAMAGE;
int LoadMaterial::MATERIAL_GRAY;

namespace {
	static const char FILEPATH1[] = ("Data/Color/Black.png");
	static const char FILEPATH2[] = ("Data/Color/Blue.png");
	static const char FILEPATH3[] = ("Data/Color/Red.png");
	static const char FILEPATH4[] = ("Data/Color/GraBlue.png");
	static const char FILEPATH5[] = ("Data/Color/GraRed.png");
	static const char FILEPATH6[] = ("Data/Color/Yellow.png");
	static const char FILEPATH7[] = ("Data/Color/Damege.png");
	static const char FILEPATH8[] = ("Data/Color/Dimgray.png");
}

//コンストラクタ・デストラクタ
LoadMaterial::LoadMaterial() {
	Init();
}

LoadMaterial::~LoadMaterial() {
	Exit();
}

//初期化
void LoadMaterial::Init() {
	MATERIAL_BLACK = -1;
	MATERIAL_BLUE = -1;
	MATERIAL_RED = -1;
	MATERIAL_GRADATION_BLUE = -1;
	MATERIAL_GRADATION_RED = -1;
	MATERIAL_GRADATION_YELLOW = -1;
	MATERIAL_DAMAGE = -1;
	MATERIAL_GRAY = -1;
}

//データ読み込み
void LoadMaterial::Load() {
	if (MATERIAL_BLACK == -1) {
		MATERIAL_BLACK = LoadGraph(FILEPATH1);
	}
	if (MATERIAL_BLUE == -1) {
		MATERIAL_BLUE = LoadGraph(FILEPATH2);
	}
	if (MATERIAL_RED == -1) {
		MATERIAL_RED = LoadGraph(FILEPATH3);
	}
	if (MATERIAL_GRADATION_BLUE == -1) {
		MATERIAL_GRADATION_BLUE = LoadGraph(FILEPATH4);
	}
	if (MATERIAL_GRADATION_RED == -1) {
		MATERIAL_GRADATION_RED = LoadGraph(FILEPATH5);
	}
	if (MATERIAL_GRADATION_YELLOW == -1) {
		MATERIAL_GRADATION_YELLOW = LoadGraph(FILEPATH6);
	}
	if (MATERIAL_DAMAGE == -1) {
		MATERIAL_DAMAGE = LoadGraph(FILEPATH7);
	}
	if (MATERIAL_GRAY == -1) {
		MATERIAL_GRAY = LoadGraph(FILEPATH8);
	}
}

//データ破棄
void LoadMaterial::Exit() {
	if (MATERIAL_BLACK != -1) {
		DeleteGraph(MATERIAL_BLACK);
		MATERIAL_BLACK = -1;
	}
	if (MATERIAL_BLUE != -1) {
		DeleteGraph(MATERIAL_BLUE);
		MATERIAL_BLUE = -1;
	}
	if (MATERIAL_RED != -1) {
		DeleteGraph(MATERIAL_RED);
		MATERIAL_RED = -1;
	}
	if (MATERIAL_GRADATION_BLUE != -1) {
		DeleteGraph(MATERIAL_GRADATION_BLUE);
		MATERIAL_GRADATION_BLUE = -1;
	}
	if (MATERIAL_GRADATION_RED != -1) {
		DeleteGraph(MATERIAL_GRADATION_RED);
		MATERIAL_GRADATION_RED = -1;
	}
	if (MATERIAL_GRADATION_YELLOW != -1) {
		DeleteGraph(MATERIAL_GRADATION_YELLOW);
		MATERIAL_GRADATION_YELLOW = -1;
	}
	if (MATERIAL_DAMAGE != -1) {
		DeleteGraph(MATERIAL_DAMAGE);
		MATERIAL_DAMAGE = -1;
	}
	if (MATERIAL_GRAY != -1) {
		DeleteGraph(MATERIAL_GRAY);
		MATERIAL_GRAY = -1;
	}
}