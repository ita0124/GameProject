#include "PlatformManager.h"

namespace {
	constexpr const char* PLATFORM_ID_FILE_CSV_PATH[PlatformManager::TagMapKinds::MAP_NUM] = {			//プラットフォームIDCSVファイルパス
	"Data/CSV/Map1/Platform/PlatformId.csv",
	"Data/CSV/Map2/Platform/PlatformId.csv",
	};

	constexpr const char* PLATFORM_DATA_FILE_CSV_PATH[PlatformManager::TagMapKinds::MAP_NUM] = {		//プラットフォームデータCSVファイルパス
	"Data/CSV/Map1/Platform/PlatformData.csv",
	"Data/CSV/Map2/Platform/PlatformData.csv",
	};

	constexpr const char* MODEL_FILE_PATH[PlatformBase::TagPlatformKinds::PLATFORM_NUM] = {				//モデルファイルパス
	"Data/Model/Stage/NormalPlatform/NormalPlatform.mv1",
	"Data/Model/Stage/MovingPlatform/MovingPlatform.mv1",
	"Data/Model/Stage/FallingPlatform/FallingPlatform.mv1",
	"",
	"Data/Model/Stage/RespawnPlatform/RespawnPlatform.mv1",
	"Data/Model/Stage/GoalPlatform/GoalPlatform.mv1",
	"Data/Model/Stage/BarrierWall/BarrierWall.mv1",
	"Data/Model/Stage/FlowersPlatform/FlowersPlatform.mv1",
	"Data/Model/Stage/GrassPlatform/GrassPlatform.mv1",
	"Data/Model/Stage/FencePlatform/FencePlatform.mv1",
	"Data/Model/Stage/TreePlatform/TreePlatform.mv1",
	"Data/Model/Stage/ArrowPlayform/ArrowPlayform.mv1",
	"Data/Model/Stage/MushroomsPlatform/MushroomsPlatform.mv1",
	};
}

//初期化処理
void PlatformManager::Init(int _Map) {
	//初期化
	for (int Index = 0; Index < PLATFORM_MAX; Index++) {
		m_Platform[Index] = nullptr;
	}

	//CSVファイルからデータを読む込む
	FILE* PlatformIdFile;
	//ファイルを開く
	if (fopen_s(&PlatformIdFile, PLATFORM_ID_FILE_CSV_PATH[_Map], "r") != 0)return;
	//データ取得
	for (int PlatformIndex = 0; PlatformIndex < PLATFORM_MAX; PlatformIndex++) {
		fscanf_s(PlatformIdFile, "%d", &m_PlatformID[PlatformIndex][0]);
		//カンマor改行を飛ばす
		fgetc(PlatformIdFile);
	}
	//開いたファイルを閉じる
	fclose(PlatformIdFile);
	//newする
	for (int PlatformIndex = 0; PlatformIndex < PLATFORM_MAX; PlatformIndex++) {
		switch (m_PlatformID[PlatformIndex][0])
		{
		case PlatformBase::TagPlatformKinds::NORMAL:
			//通常足場クラスをnew
			m_Platform[PlatformIndex] = new NormalPlatform;
			break;
		case PlatformBase::TagPlatformKinds::MOVING:
			//動く足場クラスをnew
			m_Platform[PlatformIndex] = new MovingPlatform;
			break;
		case PlatformBase::TagPlatformKinds::FALLING:
			//落ちる足場クラスをnew
			m_Platform[PlatformIndex] = new FallingPlatform;
			break;
		case PlatformBase::TagPlatformKinds::WALL:
			//壁クラスをnew
			m_Platform[PlatformIndex] = new WallPlatform;
			break;
		case PlatformBase::TagPlatformKinds::RESPAWN:
			//リスポーン地点クラスをnew
			m_Platform[PlatformIndex] = new RespawnPlatform;
			break;
		case PlatformBase::TagPlatformKinds::GOAL:
			//ゴール地点クラスをnew
			m_Platform[PlatformIndex] = new GoalPlatform;
			break;
		case PlatformBase::TagPlatformKinds::BARRIER:
			//バリアクラスをnew
			m_Platform[PlatformIndex] = new BarrierPlatform;
			break;
		case PlatformBase::TagPlatformKinds::FLOWERS:
			//花クラスをnew
			m_Platform[PlatformIndex] = new FlowersPlatform;
			break;
		case PlatformBase::TagPlatformKinds::GRASS:
			//草
			m_Platform[PlatformIndex] = new GrassPlatform;
			break;
		case PlatformBase::TagPlatformKinds::FENCE:
			//塀クラスをnew
			m_Platform[PlatformIndex] = new FencePlatform;
			break;
		case PlatformBase::TagPlatformKinds::TREE:
			//木クラスをnew
			m_Platform[PlatformIndex] = new TreePlatform;
			break;
		case PlatformBase::TagPlatformKinds::ARROW:
			//矢印
			m_Platform[PlatformIndex] = new ArrowPlatform;
			break;
		case PlatformBase::TagPlatformKinds::MUSHROOMS:
			//キノコ
			m_Platform[PlatformIndex] = new MushroomsPlatform;
			break;
		}
	}
	//初期化をここで行う
	for (int PlatformIndex = 0; PlatformIndex < PLATFORM_MAX; PlatformIndex++) {
		//nullなら行わない
		if (m_Platform[PlatformIndex] != nullptr) {
			m_Platform[PlatformIndex]->Init();
		}
	}
	//CSVファイルからデータを読む込む
	FILE* PlatformDataFile;
	//ファイルを開く
	if (fopen_s(&PlatformDataFile, PLATFORM_DATA_FILE_CSV_PATH[_Map], "r") != 0)return;
	//データ取得
	for (int PlatformIndex = 0; PlatformIndex < PLATFORM_MAX; PlatformIndex++) {
		for (int RequestDataIndex = 0; RequestDataIndex < PLATFORM_REQUEST_DATA_MAX; RequestDataIndex++) {
			//データ一つ分取得
			if (fscanf_s(PlatformDataFile, "%f", &m_RequestData[PlatformIndex][RequestDataIndex]) == 0) {
				m_RequestData[PlatformIndex][RequestDataIndex] = 0;
			}
			//カンマor改行を飛ばす
			fgetc(PlatformDataFile);
		}
	}
	//開いたファイルを閉じる
	fclose(PlatformDataFile);

	for (int PlatformIndex = 0; PlatformIndex < PLATFORM_MAX; PlatformIndex++) {
		//nullなら行わない
		if (m_Platform[PlatformIndex] != nullptr) {
			//リクエストに最低限必要なデータ
			m_Platform[PlatformIndex]->SetNormalRequestDataPosX(m_RequestData[PlatformIndex][0]);					//X座標
			m_Platform[PlatformIndex]->SetNormalRequestDataPosY(m_RequestData[PlatformIndex][1]);					//Y座標
			m_Platform[PlatformIndex]->SetNormalRequestDataPosZ(m_RequestData[PlatformIndex][2]);					//Z座標
			m_Platform[PlatformIndex]->SetNormalRequestDataRotX(m_RequestData[PlatformIndex][3]);					//X回転率
			m_Platform[PlatformIndex]->SetNormalRequestDataRotY(m_RequestData[PlatformIndex][4]);					//Y回転率
			m_Platform[PlatformIndex]->SetNormalRequestDataRotZ(m_RequestData[PlatformIndex][5]);					//Z回転率
			m_Platform[PlatformIndex]->SetNormalRequestDataScaleX(m_RequestData[PlatformIndex][6]);					//X拡縮
			m_Platform[PlatformIndex]->SetNormalRequestDataScaleY(m_RequestData[PlatformIndex][7]);					//Y拡縮
			m_Platform[PlatformIndex]->SetNormalRequestDataScaleZ(m_RequestData[PlatformIndex][8]);					//Z拡縮
			m_Platform[PlatformIndex]->SetNormalRequestDataSizeX(m_RequestData[PlatformIndex][9]);					//Xサイズ
			m_Platform[PlatformIndex]->SetNormalRequestDataSizeY(m_RequestData[PlatformIndex][10]);					//Yサイズ
			m_Platform[PlatformIndex]->SetNormalRequestDataSizeZ(m_RequestData[PlatformIndex][11]);					//Zサイズ
			m_Platform[PlatformIndex]->SetNormalRequestDataGimmickType((int)m_RequestData[PlatformIndex][12]);		//ギミックタイプ
			////動く床なら
			if (m_PlatformID[PlatformIndex][0] == PlatformBase::TagPlatformKinds::MOVING) {
				//---データ使いまわし---
				m_Platform[PlatformIndex]->SetMovingPlatformRequestDataFirstPosX(m_RequestData[PlatformIndex][0]);	//X初期座標
				m_Platform[PlatformIndex]->SetMovingPlatformRequestDataFirstPosY(m_RequestData[PlatformIndex][1]);	//Y初期座標
				m_Platform[PlatformIndex]->SetMovingPlatformRequestDataFirstPosZ(m_RequestData[PlatformIndex][2]);	//Z初期座標
				//----------------------
				m_Platform[PlatformIndex]->SetMovingPlatformRequestDataEndPosX(m_RequestData[PlatformIndex][13]);	//X終端座標
				m_Platform[PlatformIndex]->SetMovingPlatformRequestDataEndPosY(m_RequestData[PlatformIndex][14]);	//Y終端座標
				m_Platform[PlatformIndex]->SetMovingPlatformRequestDataEndPosZ(m_RequestData[PlatformIndex][15]);	//Z終端座標
				m_Platform[PlatformIndex]->SetMovingPlatformRequestDataMoveSpeed(m_RequestData[PlatformIndex][16]);	//移動速度
			}
			//リクエストしてオブジェクトを生成
			m_Platform[PlatformIndex]->Request(m_Platform[PlatformIndex]->GetNormalRequestData());
		}
	}
}
//データ破棄処理
void PlatformManager::Exit() {
	for (int PlatformIndex = 0; PlatformIndex < PLATFORM_MAX; PlatformIndex++) {
		//nullなら行わない
		if (m_Platform[PlatformIndex] != nullptr) {
			//データ破棄
			m_Platform[PlatformIndex]->Exit();
			//newを消す
			delete m_Platform[PlatformIndex];
			//nullを入れておく
			m_Platform[PlatformIndex] = nullptr;
		}
	}
}
//データ読み込み処理
void PlatformManager::Load() {
	int Hndl[PlatformBase::TagPlatformKinds::PLATFORM_NUM] = {};
	//
	for (int LoadIndex = 0;LoadIndex < PlatformBase::TagPlatformKinds::PLATFORM_NUM;LoadIndex++) {
		Hndl[LoadIndex] = MV1LoadModel(MODEL_FILE_PATH[LoadIndex]);
	}
	for (int PlatformIndex = 0; PlatformIndex < PLATFORM_MAX; PlatformIndex++) {
		//nullなら行わない
		if (m_Platform[PlatformIndex] != nullptr) {
			m_Platform[PlatformIndex]->Load(Hndl[m_PlatformID[PlatformIndex][0]]);
		}
	}
}
//描画処理
void PlatformManager::Draw() {
	for (int PlatformIndex = 0; PlatformIndex < PLATFORM_MAX; PlatformIndex++) {
		//nullなら行わない
		if (m_Platform[PlatformIndex] != nullptr) {
			m_Platform[PlatformIndex]->Draw();
		}
	}
}
//影を生成する物
void PlatformManager::ShadowMapDraw() {
	for (int PlatformIndex = 0; PlatformIndex < PLATFORM_MAX; PlatformIndex++) {
		//nullなら行わない
		if (m_Platform[PlatformIndex] != nullptr && m_Platform[PlatformIndex]->GetPlatformKinds() != PlatformBase::TagPlatformKinds::BARRIER) {

			m_Platform[PlatformIndex]->Draw();
		}
	}
}
//毎フレーム呼び出す処理
void PlatformManager::Step() {
	for (int PlatformIndex = 0; PlatformIndex < PLATFORM_MAX; PlatformIndex++) {
		//nullなら行わない
		if (m_Platform[PlatformIndex] != nullptr) {
			m_Platform[PlatformIndex]->Step();
		}
	}
}
//モデル更新処理
void PlatformManager::Update() {
	for (int PlatformIndex = 0; PlatformIndex < PLATFORM_MAX; PlatformIndex++) {
		//nullなら行わない
		if (m_Platform[PlatformIndex] != nullptr) {
			m_Platform[PlatformIndex]->Update();
		}
	}
}
//当たり判定後の処理
void PlatformManager::HitCalc(const int& _Num, ObjectBase* _Object) {
	if (m_Platform[_Num] != nullptr) {
		m_Platform[_Num]->HitCalc(_Object);
	}
}
//当たり判定後の処理(当たっていない場合)
void PlatformManager::NotHitCalc(const int& _Num, ObjectBase* _Object) {
	if (m_Platform[_Num] != nullptr) {
		m_Platform[_Num]->NotHitCalc(_Object);
	}
}
