#include "PlatformManager.h"

namespace {
	const char CSV_PLATFORM_FILE_PATH[] = ("Data/CSV/Stage/Platform.csv");	//プラットフォームIDCSVファイルパス
	const char CSV_STAGE_FILE_PATH[] = ("Data/CSV/Stage/Stage.csv");		//ステージCSVファイルパス
}

//初期化処理
void PlatformManager::Init() {
	//CSVファイルからデータを読む込む
	FILE* PlatformIdFile;
	//ファイルを開く
	if (fopen_s(&PlatformIdFile, CSV_PLATFORM_FILE_PATH, "r") != 0)return;
	//データ取得
	for (int PlatformIndex = 0;PlatformIndex < PLATFORM_MAX;PlatformIndex++) {
		fscanf_s(PlatformIdFile, "%d", &m_PlatformID[PlatformIndex][0]);
		//カンマor改行を飛ばす
		fgetc(PlatformIdFile);
	}
	//開いたファイルを閉じる
	fclose(PlatformIdFile);
	//初期化
	for (int Index = 0; Index < PLATFORM_MAX; Index++) {
		m_Platform[Index] = nullptr;
	}
	//newする
	for (int PlatformIndex = 0;PlatformIndex < PLATFORM_MAX;PlatformIndex++) {
		switch (m_PlatformID[PlatformIndex][0])
		{
		case NORMAL:
			m_Platform[PlatformIndex] = new NormalPlatform;
			break;
		case MOVING:
			m_Platform[PlatformIndex] = new MovingPlatform;
			break;
		case FALLING:
			m_Platform[PlatformIndex] = new FallingPlatform;
			break;
		case BATTLE:
			m_Platform[PlatformIndex] = new BattlePlatform;
			break;
		case JUMP:
			m_Platform[PlatformIndex] = new JumpPlatform;
			break;
		}
	}
	//初期化をここで行う
	for (int PlatformIndex = 0;PlatformIndex < PLATFORM_MAX;PlatformIndex++) {
		m_Platform[PlatformIndex]->Init();
	}
	//CSVファイルからデータを読む込む
	FILE* StageFile;
	//ファイルを開く
	if (fopen_s(&StageFile, CSV_STAGE_FILE_PATH, "r") != 0)return;
	//データ取得
	for (int PlatformIndex = 0;PlatformIndex < PLATFORM_MAX;PlatformIndex++) {
		for (int RequestDataIndex = 0;RequestDataIndex < REQUEST_DATA_MAX;RequestDataIndex++) {
			//データ一つ分取得
			fscanf_s(StageFile, "%f", &m_RequestData[PlatformIndex][RequestDataIndex]);
			//カンマor改行を飛ばす
			fgetc(StageFile);
		}
	}
	//開いたファイルを閉じる
	fclose(PlatformIdFile);

	for (int PlatformIndex = 0;PlatformIndex < PLATFORM_MAX;PlatformIndex++) {
		if (m_Platform[PlatformIndex] != nullptr) {
			m_Platform[PlatformIndex]->SetNormalRequestDataPosX(m_RequestData[PlatformIndex][0]);		//X座標
			m_Platform[PlatformIndex]->SetNormalRequestDataPosY(m_RequestData[PlatformIndex][1]);		//Y座標
			m_Platform[PlatformIndex]->SetNormalRequestDataPosZ(m_RequestData[PlatformIndex][2]);		//Z座標
			m_Platform[PlatformIndex]->SetNormalRequestDataRotX(m_RequestData[PlatformIndex][3]);		//X回転率
			m_Platform[PlatformIndex]->SetNormalRequestDataRotY(m_RequestData[PlatformIndex][4]);		//Y回転率
			m_Platform[PlatformIndex]->SetNormalRequestDataRotZ(m_RequestData[PlatformIndex][5]);		//Z回転率
			m_Platform[PlatformIndex]->SetNormalRequestDataScaleX(m_RequestData[PlatformIndex][6]);		//X拡縮
			m_Platform[PlatformIndex]->SetNormalRequestDataScaleX(m_RequestData[PlatformIndex][7]);		//Y拡縮
			m_Platform[PlatformIndex]->SetNormalRequestDataScaleX(m_RequestData[PlatformIndex][8]);		//Z拡縮
			m_Platform[PlatformIndex]->SetNormalRequestDataSizeX(m_RequestData[PlatformIndex][9]);		//Xサイズ
			m_Platform[PlatformIndex]->SetNormalRequestDataSizeX(m_RequestData[PlatformIndex][10]);		//Yサイズ
			m_Platform[PlatformIndex]->SetNormalRequestDataSizeX(m_RequestData[PlatformIndex][11]);		//Zサイズ
		}
	}
}
//データ破棄処理
void PlatformManager::Exit() {
	for (int PlatformIndex = 0;PlatformIndex < PLATFORM_MAX;PlatformIndex++) {
		if (m_Platform[PlatformIndex] != nullptr) {
			delete m_Platform[PlatformIndex];
			m_Platform[PlatformIndex] = nullptr;
		}
	}
}
//データ読み込み処理
void PlatformManager::Load() {

}
//描画処理
void PlatformManager::Draw() {

}
//毎フレーム呼び出す処理
void PlatformManager::Step() {

}
//モデル更新処理
void PlatformManager::Update() {

}
//リクエスト
void PlatformManager::Request() {

}
