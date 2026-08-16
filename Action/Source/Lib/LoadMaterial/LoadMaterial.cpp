#include "LoadMaterial.h"

int LoadMaterial::MATERIAL_BLACK;
int LoadMaterial::MATERIAL_BLUE;
int LoadMaterial::MATERIAL_RED;
int LoadMaterial::MATERIAL_GRAY;
int LoadMaterial::MATERIAL_BEFORE_RESPAWN;
int LoadMaterial::MATERIAL_AFTER_RESPAWN;

namespace {
	constexpr char FILEPATH1[] = ("Data/Color/Black.png");
	constexpr char FILEPATH2[] = ("Data/Color/Blue.png");
	constexpr char FILEPATH3[] = ("Data/Color/Red.png");
	constexpr char FILEPATH4[] = ("Data/Color/Dimgray.png");
	constexpr char FILEPATH5[] = ("Data/Color/BeforeRespawn.png");
	constexpr char FILEPATH6[] = ("Data/Color/AfterRespawn.png");
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
	MATERIAL_GRAY = -1;
	MATERIAL_BEFORE_RESPAWN = -1;
	MATERIAL_AFTER_RESPAWN = -1;
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
	if (MATERIAL_GRAY == -1) {
		MATERIAL_GRAY = LoadGraph(FILEPATH4);
	}
	if (MATERIAL_BEFORE_RESPAWN == -1) {
		MATERIAL_BEFORE_RESPAWN = LoadGraph(FILEPATH5);
	}
	if (MATERIAL_AFTER_RESPAWN == -1) {
		MATERIAL_AFTER_RESPAWN = LoadGraph(FILEPATH6);
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
	if (MATERIAL_GRAY != -1) {
		DeleteGraph(MATERIAL_GRAY);
		MATERIAL_GRAY = -1;
	}
	if (MATERIAL_BEFORE_RESPAWN != -1) {
		DeleteGraph(MATERIAL_BEFORE_RESPAWN);
		MATERIAL_BEFORE_RESPAWN = -1;
	}
	if (MATERIAL_AFTER_RESPAWN != -1) {
		DeleteGraph(MATERIAL_AFTER_RESPAWN);
		MATERIAL_AFTER_RESPAWN = -1;
	}
}