#include "MobEnemyManager.h"

namespace {
	constexpr const char* MOB_ENEMY_ID_FILE_CSV_PATH[MobEnemyManager::TagMapKinds::MAP_NUM] = {			//モブ敵IDCSVファイルパス
	"Data/CSV/MobEnemy1/MobEnemyId.csv",
	"Data/CSV/MobEnemy2/MobEnemyId.csv",
	};

	constexpr const char* MOB_ENEMY_DATA_FILE_CSV_PATH[MobEnemyManager::TagMapKinds::MAP_NUM] = {		//モブ敵データCSVファイルパス
	"Data/CSV/MobEnemy1/MobEnemyData.csv",
	"Data/CSV/MobEnemy2/MobEnemyData.csv",
	};
}

//初期化処理
void MobEnemyManager::Init(int _Map) {
	//初期化
	for (int Index = 0; Index < MOB_ENEMY_MAX; Index++) {
		m_MobEnemy[Index] = nullptr;
	}

	//CSVファイルからデータを読む込む
	FILE* MobEnemyIdFile;
	//ファイルを開く
	if (fopen_s(&MobEnemyIdFile, MOB_ENEMY_ID_FILE_CSV_PATH[_Map], "r") != 0)return;
	//データ取得
	for (int MobEnemyIndex = 0; MobEnemyIndex < MOB_ENEMY_MAX; MobEnemyIndex++) {
		fscanf_s(MobEnemyIdFile, "%d", &m_MobEnemyID[MobEnemyIndex][0]);
		//カンマor改行を飛ばす
		fgetc(MobEnemyIdFile);
	}
	//開いたファイルを閉じる
	fclose(MobEnemyIdFile);
	//newする
	for (int MobEnemyIndex = 0; MobEnemyIndex < MOB_ENEMY_MAX; MobEnemyIndex++) {
		switch (m_MobEnemyID[MobEnemyIndex][0])
		{
		case MobEnemyBase::TagMobEnemyKinds::BOAR:
			//イノシシクラスをnew
			m_MobEnemy[MobEnemyIndex] = new Boar;
			break;
		case MobEnemyBase::TagMobEnemyKinds::WOLF:
			//オオカミクラスをnew
			m_MobEnemy[MobEnemyIndex] = new Wolf;
			break;
		}
	}
	//初期化をここで行う
	for (int MobEnemyIndex = 0; MobEnemyIndex < MOB_ENEMY_MAX; MobEnemyIndex++) {
		//nullなら行わない
		if (m_MobEnemy[MobEnemyIndex] != nullptr) {
			m_MobEnemy[MobEnemyIndex]->Init();
		}
	}
	//CSVファイルからデータを読む込む
	FILE* MobEnemyDataFile;
	//ファイルを開く
	if (fopen_s(&MobEnemyDataFile, MOB_ENEMY_DATA_FILE_CSV_PATH[_Map], "r") != 0)return;
	//データ取得
	for (int MobEnemyIndex = 0; MobEnemyIndex < MOB_ENEMY_MAX; MobEnemyIndex++) {
		for (int RequestDataIndex = 0; RequestDataIndex < MOB_ENEMY_REQUEST_DATA_MAX; RequestDataIndex++) {
			//データ一つ分取得
			if (fscanf_s(MobEnemyDataFile, "%f", &m_RequestData[MobEnemyIndex][RequestDataIndex]) == 0) {
				m_RequestData[MobEnemyIndex][RequestDataIndex] = 0;
			}
			//カンマor改行を飛ばす
			fgetc(MobEnemyDataFile);
		}
	}
	//開いたファイルを閉じる
	fclose(MobEnemyDataFile);

	for (int MobEnemyIndex = 0; MobEnemyIndex < MOB_ENEMY_MAX; MobEnemyIndex++) {
		//nullなら行わない
		if (m_MobEnemy[MobEnemyIndex] != nullptr) {
			//リクエストに最低限必要なデータ
			m_MobEnemy[MobEnemyIndex]->SetRequestDataPosX(m_RequestData[MobEnemyIndex][0]);					//X座標
			m_MobEnemy[MobEnemyIndex]->SetRequestDataPosY(m_RequestData[MobEnemyIndex][1]);					//Y座標
			m_MobEnemy[MobEnemyIndex]->SetRequestDataPosZ(m_RequestData[MobEnemyIndex][2]);					//Z座標
			m_MobEnemy[MobEnemyIndex]->SetRequestDataRotX(m_RequestData[MobEnemyIndex][3]);					//X回転率
			m_MobEnemy[MobEnemyIndex]->SetRequestDataRotY(m_RequestData[MobEnemyIndex][4]);					//Y回転率
			m_MobEnemy[MobEnemyIndex]->SetRequestDataRotZ(m_RequestData[MobEnemyIndex][5]);					//Z回転率
			m_MobEnemy[MobEnemyIndex]->SetRequestDataScaleX(m_RequestData[MobEnemyIndex][6]);				//X拡縮
			m_MobEnemy[MobEnemyIndex]->SetRequestDataScaleY(m_RequestData[MobEnemyIndex][7]);				//Y拡縮
			m_MobEnemy[MobEnemyIndex]->SetRequestDataScaleZ(m_RequestData[MobEnemyIndex][8]);				//Z拡縮
			m_MobEnemy[MobEnemyIndex]->SetRequestDataSizeX(m_RequestData[MobEnemyIndex][9]);				//Xサイズ
			m_MobEnemy[MobEnemyIndex]->SetRequestDataSizeY(m_RequestData[MobEnemyIndex][10]);				//Yサイズ
			m_MobEnemy[MobEnemyIndex]->SetRequestDataSizeZ(m_RequestData[MobEnemyIndex][11]);				//Zサイズ
			m_MobEnemy[MobEnemyIndex]->SetRequestDataGimmickType((int)m_RequestData[MobEnemyIndex][12]);	//ギミックタイプ
			m_MobEnemy[MobEnemyIndex]->SetRequestDataPower(m_RequestData[MobEnemyIndex][13]);				//攻撃力
			m_MobEnemy[MobEnemyIndex]->SetRequestDataHitPoints(m_RequestData[MobEnemyIndex][14]);			//体力
			//リクエストしてオブジェクトを生成
			m_MobEnemy[MobEnemyIndex]->Request(m_MobEnemy[MobEnemyIndex]->GetRequestData());
		}
	}
}
//データ破棄処理
void MobEnemyManager::Exit() {
	for (int MobEnemyIndex = 0; MobEnemyIndex < MOB_ENEMY_MAX; MobEnemyIndex++) {
		//nullなら行わない
		if (m_MobEnemy[MobEnemyIndex] != nullptr) {
			//データ破棄
			m_MobEnemy[MobEnemyIndex]->Exit();
			//newを消す
			delete m_MobEnemy[MobEnemyIndex];
			//nullを入れておく
			m_MobEnemy[MobEnemyIndex] = nullptr;
		}
	}
}
//データ読み込み処理
void MobEnemyManager::Load() {
	for (int MobEnemyIndex = 0; MobEnemyIndex < MOB_ENEMY_MAX; MobEnemyIndex++) {
		//nullなら行わない
		if (m_MobEnemy[MobEnemyIndex] != nullptr) {
			m_MobEnemy[MobEnemyIndex]->Load();
		}
	}
}
//描画処理
void MobEnemyManager::Draw() {
	for (int MobEnemyIndex = 0; MobEnemyIndex < MOB_ENEMY_MAX; MobEnemyIndex++) {
		//nullなら行わない
		if (m_MobEnemy[MobEnemyIndex] != nullptr && m_MobEnemy[MobEnemyIndex]->GetIsActive()) {
			m_MobEnemy[MobEnemyIndex]->Draw();
		}
	}
}
//毎フレーム呼び出す処理
void MobEnemyManager::Step() {
	for (int MobEnemyIndex = 0; MobEnemyIndex < MOB_ENEMY_MAX; MobEnemyIndex++) {
		//nullなら行わない
		if (m_MobEnemy[MobEnemyIndex] != nullptr && m_MobEnemy[MobEnemyIndex]->GetIsActive()) {
			m_MobEnemy[MobEnemyIndex]->Step();
		}
	}
}
//モデル更新処理
void MobEnemyManager::Update() {
	for (int MobEnemyIndex = 0; MobEnemyIndex < MOB_ENEMY_MAX; MobEnemyIndex++) {
		//nullなら行わない
		if (m_MobEnemy[MobEnemyIndex] != nullptr && m_MobEnemy[MobEnemyIndex]->GetIsActive()) {
			m_MobEnemy[MobEnemyIndex]->Update();
		}
	}
}
//当たり判定後の処理
void MobEnemyManager::HitCalc(const int& _Num, ObjectBase* _Object) {
	if (m_MobEnemy[_Num] != nullptr) {
		m_MobEnemy[_Num]->HitCalc(_Object);
	}
}
//当たり判定後の処理(当たっていない場合)
void MobEnemyManager::NotHitCalc(const int& _Num, ObjectBase* _Object) {
	if (m_MobEnemy[_Num] != nullptr) {
		m_MobEnemy[_Num]->NotHitCalc(_Object);
	}
}//Set
void MobEnemyManager::SetPlayerPos(VECTOR _PlayerPos) {
	for (int MobEnemyIndex = 0; MobEnemyIndex < MOB_ENEMY_MAX; MobEnemyIndex++) {
		//nullなら行わない
		if (m_MobEnemy[MobEnemyIndex] != nullptr) {
			m_MobEnemy[MobEnemyIndex]->SetPlayerPos(_PlayerPos);
		}
	}
}
