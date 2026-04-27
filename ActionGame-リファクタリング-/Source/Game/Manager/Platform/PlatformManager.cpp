#include "Game/Manager/Platform/PlatformManager.h"
#include "Game/Object/Platform/NormalPlatform/NormalPlatform.h"
#include "Game/Object/Platform/MovingPlatform/MovingPlatform.h"
#include "Game/Object/Platform/JumpPlatform/JumpPlatform.h"
#include "Game/Object/Platform/FallingPlatform/FallingPlatform.h"
#include "Game/Object/Platform/BattlePlatform/BattlePlatform.h"

namespace {
	const char		CSV_FILE_PATH[] = ("Data/CSV/Stage/Stage.csv");				//CSVファイルパス
}

//コンストラクタ
PlatformManager::PlatformManager() {
	for (int Index = 0; Index < PLATFORM_MAX; Index++) {
		m_PlatformBase[Index] = nullptr;
	}
}
//デストラクタ
PlatformManager::~PlatformManager() {
	Exit();
}
//初期化処理
void PlatformManager::Init() {
	//CSVファイルからデータを読み込む
	FILE* CsvFile;
	//ファイルを開く　失敗したらこれ以降の処理を行わない
	if (fopen_s(&CsvFile, CSV_FILE_PATH, "r") != 0)return;
	//データ取得
	for (int Index = 0; Index < PLATFORM_MAX; Index++) {
		for (int a = 0;a < 13;a++)
			//データ一つ分取得
			fscanf_s(CsvFile, "%d", &Stage[a][Index]);
		//カンマor改行を飛ばす
		fgetc(CsvFile);
	}
	//開いたファイルを閉じる
	fclose(CsvFile);
}
//データ破棄処理
void PlatformManager::Exit() {
	for (int Index = 0; Index < PLATFORM_MAX; Index++) {
		if (m_PlatformBase[Index] != nullptr) {
			m_PlatformBase[Index]->Exit();

			delete m_PlatformBase[Index];
			m_PlatformBase[Index] = nullptr;
		}
	}
}
// データ読み込み処理
void PlatformManager::Load() {
	for (int Index = 0; Index < PLATFORM_MAX; Index++) {
		switch (Stage[Index][0]) {
		case NORMAL:
			m_PlatformBase[Index] = new NormalPlatform;
			break;
		case MOVING:
			m_PlatformBase[Index] = new MovingPlatform;
			break;
		case FALLINP:
			m_PlatformBase[Index] = new FallingPlatform;
			break;
		case BATTLE:
			m_PlatformBase[Index] = new BattlePlatform;
			break;
		case JUMP:
			m_PlatformBase[Index] = new JumpPlatform;
			break;
		}
	}
}
//モデル更新処理
void PlatformManager::Update() {
	for (int Index = 0; Index < PLATFORM_MAX; Index++) {
		if (m_PlatformBase[Index] != nullptr) {
			m_PlatformBase[Index]->Update();
		}
	}
}
//描画処理
void PlatformManager::Draw() {
	for (int Index = 0; Index < PLATFORM_MAX; Index++) {
		if (m_PlatformBase[Index] != nullptr) {
			m_PlatformBase[Index]->Draw();
		}
	}
}
//毎フレーム呼び出す処理
void PlatformManager::Step() {
	for (int Index = 0; Index < PLATFORM_MAX; Index++) {
		if (m_PlatformBase[Index] != nullptr) {
			m_PlatformBase[Index]->Step();
		}
	}
}
//リクエスト
void PlatformManager::Request() {

}
