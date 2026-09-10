#include "LoadMaterial.h"


using namespace std;

vector<int>LoadMaterial::m_Hndl;

namespace {
	constexpr const char* FilePath[LoadMaterial::TagMaterial::MATERIAL_MAX] = {
	"Data/Color/Black.png",
	"Data/Color/Blue.png",
	"Data/Color/Red.png",
	"Data/Color/BeforeRespawn.png",
	"Data/Color/AfterRespawn.png",
	"Data/Color/GradationBlue.png",
	"Data/Color/GradationYellow.png",
	"Data/Color/GradationRed.png",
	};
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
	m_Hndl.clear();
}

//データ読み込み
void LoadMaterial::Load() {
	for (int Index = 0; Index < MATERIAL_MAX; Index++) {
		int Hndl = LoadGraph(FilePath[Index]);
		m_Hndl.push_back(Hndl);
	}
}

//データ破棄
void LoadMaterial::Exit() {
	for (int Index = 0; Index < m_Hndl.size(); Index++) {
		if (m_Hndl[Index] != -1) {
			DeleteGraph(m_Hndl[Index]);
			m_Hndl[Index] = -1;
		}
	}
}