#include "GimmickManager.h"

namespace {
	constexpr const char* GIMMICK_ID_FILE_CSV_PATH[GimmickManager::TagMapKinds::MAP_NUM] = {			//モブ敵IDCSVファイルパス
	"Data/CSV/Gimmick1/GimmickId.csv",
	"Data/CSV/Gimmick2/GimmickId.csv",
	};

	constexpr const char* GIMMICK_DATA_FILE_CSV_PATH[GimmickManager::TagMapKinds::MAP_NUM] = {			//モブ敵データCSVファイルパス
	"Data/CSV/Gimmick1/GimmickData.csv",
	"Data/CSV/Gimmick2/GimmickData.csv",
	};
}

//初期化処理
void GimmickManager::Init(int _Map) {
	//初期化
	for (int Index = 0; Index < GIMMICK_MAX; Index++) {
		m_Gimmick[Index] = nullptr;
	}

	//CSVファイルからデータを読む込む
	FILE* GimmickIdFile;
	//ファイルを開く
	if (fopen_s(&GimmickIdFile, GIMMICK_ID_FILE_CSV_PATH[_Map], "r") != 0)return;
	//データ取得
	for (int GimmickIndex = 0; GimmickIndex < GIMMICK_MAX; GimmickIndex++) {
		fscanf_s(GimmickIdFile, "%d", &m_GimmickID[GimmickIndex][0]);
		//カンマor改行を飛ばす
		fgetc(GimmickIdFile);
	}
	//開いたファイルを閉じる
	fclose(GimmickIdFile);
	//newする
	for (int GimmickIndex = 0; GimmickIndex < GIMMICK_MAX; GimmickIndex++) {
		switch (m_GimmickID[GimmickIndex][0])
		{
		case GimmickBase::TagGimmickKinds::BARRIERACTIVATOR:
			//イノシシクラスをnew
			m_Gimmick[GimmickIndex] = new BarrierActivator;
			break;
		}
	}
	//初期化をここで行う
	for (int GimmickIndex = 0; GimmickIndex < GIMMICK_MAX; GimmickIndex++) {
		//nullなら行わない
		if (m_Gimmick[GimmickIndex] != nullptr) {
			m_Gimmick[GimmickIndex]->Init();
		}
	}
	//CSVファイルからデータを読む込む
	FILE* GimmickDataFile;
	//ファイルを開く
	if (fopen_s(&GimmickDataFile, GIMMICK_DATA_FILE_CSV_PATH[_Map], "r") != 0)return;
	//データ取得
	for (int GimmickIndex = 0; GimmickIndex < GIMMICK_MAX; GimmickIndex++) {
		for (int RequestDataIndex = 0; RequestDataIndex < GIMMICK_REQUEST_DATA_MAX; RequestDataIndex++) {
			//データ一つ分取得
			if (fscanf_s(GimmickDataFile, "%f", &m_RequestData[GimmickIndex][RequestDataIndex]) == 0) {
				m_RequestData[GimmickIndex][RequestDataIndex] = 0;
			}
			//カンマor改行を飛ばす
			fgetc(GimmickDataFile);
		}
	}
	//開いたファイルを閉じる
	fclose(GimmickDataFile);

	for (int GimmickIndex = 0; GimmickIndex < GIMMICK_MAX; GimmickIndex++) {
		//nullなら行わない
		if (m_Gimmick[GimmickIndex] != nullptr) {
			//リクエストに最低限必要なデータ
			m_Gimmick[GimmickIndex]->SetRequestDataPosX(m_RequestData[GimmickIndex][0]);					//X座標
			m_Gimmick[GimmickIndex]->SetRequestDataPosY(m_RequestData[GimmickIndex][1]);					//Y座標
			m_Gimmick[GimmickIndex]->SetRequestDataPosZ(m_RequestData[GimmickIndex][2]);					//Z座標
			m_Gimmick[GimmickIndex]->SetRequestDataRotX(m_RequestData[GimmickIndex][3]);					//X回転率
			m_Gimmick[GimmickIndex]->SetRequestDataRotY(m_RequestData[GimmickIndex][4]);					//Y回転率
			m_Gimmick[GimmickIndex]->SetRequestDataRotZ(m_RequestData[GimmickIndex][5]);					//Z回転率
			m_Gimmick[GimmickIndex]->SetRequestDataScaleX(m_RequestData[GimmickIndex][6]);					//X拡縮
			m_Gimmick[GimmickIndex]->SetRequestDataScaleY(m_RequestData[GimmickIndex][7]);					//Y拡縮
			m_Gimmick[GimmickIndex]->SetRequestDataScaleZ(m_RequestData[GimmickIndex][8]);					//Z拡縮
			m_Gimmick[GimmickIndex]->SetRequestDataSizeX(m_RequestData[GimmickIndex][9]);					//Xサイズ
			m_Gimmick[GimmickIndex]->SetRequestDataSizeY(m_RequestData[GimmickIndex][10]);					//Yサイズ
			m_Gimmick[GimmickIndex]->SetRequestDataSizeZ(m_RequestData[GimmickIndex][11]);					//Zサイズ
			m_Gimmick[GimmickIndex]->SetRequestDataGimmickType((int)m_RequestData[GimmickIndex][12]);		//ギミックタイプ
			//リクエストしてオブジェクトを生成
			m_Gimmick[GimmickIndex]->Request(m_Gimmick[GimmickIndex]->GetRequestData());
		}
	}
}
//データ破棄処理
void GimmickManager::Exit() {
	for (int GimmickIndex = 0; GimmickIndex < GIMMICK_MAX; GimmickIndex++) {
		//nullなら行わない
		if (m_Gimmick[GimmickIndex] != nullptr) {
			//データ破棄
			m_Gimmick[GimmickIndex]->Exit();
			//newを消す
			delete m_Gimmick[GimmickIndex];
			//nullを入れておく
			m_Gimmick[GimmickIndex] = nullptr;
		}
	}
}
//データ読み込み処理
void GimmickManager::Load() {
	for (int GimmickIndex = 0; GimmickIndex < GIMMICK_MAX; GimmickIndex++) {
		//nullなら行わない
		if (m_Gimmick[GimmickIndex] != nullptr) {
			m_Gimmick[GimmickIndex]->Load();
		}
	}
}
//描画処理
void GimmickManager::Draw() {
	for (int GimmickIndex = 0; GimmickIndex < GIMMICK_MAX; GimmickIndex++) {
		//nullなら行わない
		if (m_Gimmick[GimmickIndex] != nullptr && m_Gimmick[GimmickIndex]->GetIsActive()) {
			m_Gimmick[GimmickIndex]->Draw();
		}
	}
}
//毎フレーム呼び出す処理
void GimmickManager::Step(MobEnemyManager _MobEnemyManager, PlatformManager _PlatformManager) {
	for (int GimmickIndex = 0; GimmickIndex < GIMMICK_MAX; GimmickIndex++) {
		//nullなら行わない
		if (m_Gimmick[GimmickIndex] != nullptr && m_Gimmick[GimmickIndex]->GetIsActive()) {
			m_Gimmick[GimmickIndex]->Step(_MobEnemyManager, _PlatformManager);
		}
	}
}
//モデル更新処理
void GimmickManager::Update() {
	for (int GimmickIndex = 0; GimmickIndex < GIMMICK_MAX; GimmickIndex++) {
		//nullなら行わない
		if (m_Gimmick[GimmickIndex] != nullptr && m_Gimmick[GimmickIndex]->GetIsActive()) {
			m_Gimmick[GimmickIndex]->Update();
		}
	}
}
//当たり判定後の処理
void GimmickManager::HitCalc(const int& _Num, ObjectBase* _Object) {
	if (m_Gimmick[_Num] != nullptr) {
		m_Gimmick[_Num]->HitCalc(_Object);
	}
}
