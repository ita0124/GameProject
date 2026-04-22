#include "BossScene.h"

namespace {
	const int	LOAD_TIME = 60;			//非同期処理の最低時間
	const float	HNDLNUM_ADD = 0.1f;		//HndlNumに１フレームずつ加算する

	const char* FILE_PATH[HNDL_MAX] = {	//非同期中に表示する画像のファイルパス
	"Data/Load/Elephant1.png",			//象１頭
	"Data/Load/Elephant2.png",			//象２頭
	"Data/Load/Elephant3.png",			//象３頭
	"Data/Load/Elephant4.png",			//象４頭
	};
}

//コンストラクタ
BossScene::BossScene() {
	//タグをINITに設定
	m_ID = INIT;
}
//デストラクタ
BossScene::~BossScene() {
	//データ破棄処理管理関数呼び出す
	Exit();
}
//シーン中繰り返し行う処理
int	BossScene::Loop() {
	int Res = 0;

	switch (m_ID)
	{
		//初期化処理を行う
	case INIT:
		//初期化処理管理関数を呼び出す
		Init();
		//フェードイン関数を呼び出す
		Fade::RequestIn();
		//フェードインが終わったら
		if (Fade::IsEndIn()) {
			//状態をLOADに
			m_ID = LOAD;
		}
		break;
		//データ読み込み処理を行う
	case LOAD:
		//データ読み込み処理(非同期)
		if (LoadASync()) {
			SetUseASyncLoadFlag(false);
			//状態をSTARTWAITに
			m_ID = STARTWAIT;
		}
		break;
		//フェードイン処理を確実に行う
	case STARTWAIT:
		//フェードイン関数を呼び出す
		Fade::RequestIn();
		//フェードインが終わったら
		if (Fade::IsEndIn()) {
			//状態をSTEPに
			m_ID = STEP;
		}
		break;
		//毎フレーム呼び出す処理を行う
	case STEP:
		//毎フレーム呼び出す処理管理関数
		//0以外が返ってきたら
		if (Step() != 0) {
			//状態をENDWAITに
			m_ID = ENDWAIT;
		}
		break;
		//フェードアウトを確実に行う
	case ENDWAIT:
		//フェードアウト関数を呼び出す
		Fade::RequestOut();
		//フェードアウトが終わったら
		if (Fade::IsEndOut()) {
			//状態をENDに
			m_ID = END;
		}
		break;
		//データ破棄を行う
	case END:
		//データ破棄処理管理関数を呼び出す
		Exit();
		//状態をINITに
		m_ID = INIT;

		Res = 1;
		break;
	}
	//0以外の数値が入ると次のシーンへ移行する
	return Res;
}
//描画処理管理関数
void BossScene::Draw() {
	switch (m_ID) {
	case LOAD:
		DrawRotaGraph((int)SCREEN_HALF_X,(int)SCREEN_HALF_Y,10,0,m_Load.Hndl[(int)m_Load.HndlNum%4],TRUE);
		break;
	case STARTWAIT:
		m_Sky.Draw();
		m_BossArea.Draw();
		break;
	case STEP:
	case ENDWAIT:
		m_Sky.Draw();
		m_BossArea.Draw();
		m_Player.Draw();
		m_Boss.Draw();
		m_UIFrame.Draw();
		m_HitPoints.Draw();
		m_SkillPoints.Draw();
		m_Stamina.Draw();
		m_ItemManger.Draw();
	}
}
//初期化処理管理関数
void BossScene::Init() {
	m_BossArea.Init();				//ボス戦の足場クラス
	m_Sky.Init();					//天球クラス
	m_Player.Init();				//プレイヤークラス
	m_Boss.Draw();
	m_CameraManager.Init();			//カメラマネージャークラス
	m_ItemManger.Init();			//アイテムマネーシャークラス
	m_HitPoints.Init();				//体力UIクラス
	m_SkillPoints.Init();			//スキルポイントUIクラス
	m_Stamina.Init();				//スタミナUIクラス
	m_Traget.Init();				//ターゲットクラス
	m_UIFrame.Init();				//UIフレームクラス

	m_Load.LoadTime=0;				//非同期処理継続時間
	m_Load.HndlNum=0.0f;			//非同期処理時に表示する画像
	m_Load.IsLoadASync=false;		//非同期処理中か
	for (int Index = 0; Index < HNDL_MAX; Index++) {
		m_Load.Hndl[Index]=-1;		//画像ハンドル
	}
}
//データ破棄処理管理関数
void BossScene::Exit() {
	m_BossArea.Exit();				//ボス戦の足場クラス
	m_Sky.Exit();					//天球クラス
	m_Player.Exit();				//プレイヤークラス
	m_Boss.Draw();
	m_ItemManger.Exit();			//アイテムマネーシャークラス
	m_HitPoints.Exit();				//体力UIクラス
	m_SkillPoints.Exit();			//スキルポイントUIクラス
	m_Stamina.Exit();				//スタミナUIクラス
	m_Traget.Exit();				//ターゲットクラス
	m_UIFrame.Exit();				//UIフレームクラス
}
//データ読み込み処理管理関数
void BossScene::Load() {
	m_ItemManger.Load();			//アイテムマネーシャークラス
	m_HitPoints.Load();				//体力UIクラス
	m_SkillPoints.Load();			//スキルポイントUIクラス
	m_Stamina.Load();				//スタミナUIクラス
	m_Traget.Load();				//ターゲットクラス
	m_UIFrame.Load();				//UIフレームクラス
	//非同期読み込みを行う
	SetUseASyncLoadFlag(true);

	m_BossArea.Load();				//ボス戦の足場クラス
	m_Sky.Load();					//天球クラス
	m_Player.Load();				//プレイヤークラス
	m_Boss.Load();

	//非同期読み込みを行わない
	SetUseASyncLoadFlag(false);
}
//データ読み込み処理(非同期)
bool BossScene::LoadASync() {
	if (m_Load.IsLoadASync) {
		m_Load.HndlNum += HNDLNUM_ADD;
		m_Load.LoadTime++;

		//今非同期処理をしている総数
		int ASyncLoadNum = GetASyncLoadNum();

		if (ASyncLoadNum == 0 && m_Load.LoadTime > LOAD_TIME) {
			//非同期処理終了
			m_Load.IsLoadASync = false;
			m_Load.HndlNum = 0.0f;
			m_Load.LoadTime = 0;

			return true;
		}
	}
	else {
		for (int Index = 0; Index < HNDL_MAX; Index++) {
			if (m_Load.Hndl[Index] != -1)continue;
			m_Load.Hndl[Index] = LoadGraph(FILE_PATH[Index]);		//画像ハンドル
		}
		//データ読み込み処理管理関数を呼び出す
		Load();
		//非同期処理中に変更
		m_Load.IsLoadASync = true;
	}
	return false;
}
//毎フレーム呼び出す処理管理関数
int BossScene::Step() {
	int Res = 0;

	CameraStep();
	PlayerStep();
	EnemyStep();
	HitCheck();

	Update();

	return Res;
}
//モデル更新処理
void BossScene::Update() {
	m_BossArea.Update();								//ボス戦の足場クラス
	m_Sky.Update();										//天球クラス
	m_Player.Update();									//プレイヤークラス
	m_Boss.Update();
	m_CameraManager.Update();							//カメラマネージャークラス

	int HitPoints=(int)m_Player.GetHitPoints();
	m_HitPoints.Update(HitPoints);						//体力UIクラス
	int SkillPoints = (int)m_Player.GetSkillPoints();
	m_SkillPoints.Update(SkillPoints);					//スキルポイントUIクラス
	int Stamina = (int)m_Player.GetStamina();
	m_Stamina.Update(Stamina);							//スタミナUIクラス

	VECTOR Traget = VZERO;
	m_Traget.Update(Traget);							//ターゲットクラス
}
//プレイヤー関連Step
void BossScene::PlayerStep() {
	m_BossArea.Step();									//ボス戦の足場クラス
	m_Sky.Step();										//天球クラス

	m_Player.SetCameraRot(m_CameraManager.GetCameraRot());
	m_Player.Step();
}
//敵関連Step
void BossScene::EnemyStep() {
	m_Boss.SetPlayerPos(m_Player.GetPos());
	m_Boss.Step();
}
//カメラ関連Step
void BossScene::CameraStep() {
	m_CameraManager.Step(m_Player.GetPos(),VZERO,false);
}
//当たり判定関係
void BossScene::HitCheck() {
	HitCheck::ObjectToObjectPush(m_Player,m_Boss);
	HitCheck::CollToObject(m_BossArea,m_Player);
}
