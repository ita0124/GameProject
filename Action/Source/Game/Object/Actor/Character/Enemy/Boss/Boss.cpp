#include "Boss.h"
#include "Game/Object/Actor/Character/Player/Player.h"

namespace {
	constexpr VECTOR	INIT_POS = { 0.0f,0.0f,-400.0f };												//初期座標

	constexpr float		RAD = 50.0f;																	//半径
	constexpr VECTOR	BOSS_SIZE = { RAD,RAD,RAD };													//ボックス当たり判定

	constexpr float		HIT_POINTS = 1000.0f;															//体力
	constexpr float		MAX_HITPOINTS = 1000.0f;														//最大体力

	constexpr float		ACTION_IDEL_DISTANCE = 750.0f;													//IDELに移行するプレイヤーとの距離
	constexpr float		ACTION_ATTACK_DISTANCE = 75.0f;													//攻撃に移行するプレイヤーとの距離

	constexpr float		WALK_MULT = 5.0f;																//歩き時の移動乗算値
	constexpr float		REAR_ATTACK_MULT = -7.5f;														//後方攻撃の移動乗算値
	constexpr float		JUMP_MULT = 10.0f;																//ジャンプの移動乗算値
	constexpr float		CHARGE_MULT = 25.0f;															//突進の移動乗算値
	constexpr float		SPECIALSTART_MULT = 10.0f;														//必殺開始の移動乗算値

	constexpr float		NORMAL_MOVE_ROTATE_SPEED = 0.2f;												//通常移動時の回転速度

	constexpr float		NORMAL_ATTACK1_CHANGE_MATERIAL_START = 7.0f;									//通常攻撃１段目のマテリアル変更開始フレーム
	constexpr float		NORMAL_ATTACK1_CHANGE_MATERIAL_END = 10.0f;										//通常攻撃１段目のマテリアル変更終了フレーム
	constexpr float		NORMAL_ATTACK1_COLLISION_START = 10.0f;											//通常攻撃１段目の当たり判定開始フレーム
	constexpr float		NORMAL_ATTACK1_COLLISION_END = 15.0f;											//通常攻撃１段目の当たり判定終了フレーム
	constexpr float		NORMAL_ATTACK1_EFFECT_REQUEST = 12.0f;											//通常攻撃１段目のエフェクト呼び出しフレーム

	constexpr float		NORMAL_ATTACK2_CHANGE_MATERIAL_START = 5.0f;									//通常攻撃２段目のマテリアル変更開始フレーム
	constexpr float		NORMAL_ATTACK2_CHANGE_MATERIAL_END = 10.0f;										//通常攻撃２段目のマテリアル変更終了フレーム
	constexpr float		NORMAL_ATTACK2_COLLISION_START = 10.0f;											//通常攻撃２段目の当たり判定開始フレーム
	constexpr float		NORMAL_ATTACK2_COLLISION_END = 20.0f;											//通常攻撃２段目の当たり判定終了フレーム
	constexpr float		NORMAL_ATTACK2_EFFECT_REQUEST = 15.0f;											//通常攻撃２段目のエフェクト呼び出しフレーム

	constexpr float		NORMAL_ATTACK3_CHANGE_MATERIAL_START = 25.0f;									//通常攻撃３段目のマテリアル変更開始フレーム
	constexpr float		NORMAL_ATTACK3_CHANGE_MATERIAL_END = 30.0f;										//通常攻撃３段目のマテリアル変更終了フレーム
	constexpr float		NORMAL_ATTACK3_COLLISION_START = 30.0f;											//通常攻撃３段目の当たり判定開始フレーム
	constexpr float		NORMAL_ATTACK3_COLLISION_END = 40.0f;											//通常攻撃３段目の当たり判定終了フレーム
	constexpr float		NORMAL_ATTACK3_EFFECT_REQUEST = 40.0f;											//通常攻撃３段目のエフェクト呼び出しフレーム

	constexpr float		REAR_ATTACK_CHANGE_MATERIAL_START = 10.0f;										//後方攻撃のマテリアル変更開始フレーム
	constexpr float		REAR_ATTACK_CHANGE_MATERIAL_END = 15.0f;										//後方攻撃のマテリアル変更終了フレーム
	constexpr float		REAR_ATTACK_COLLISION_START = 15.0f;											//後方攻撃の当たり判定開始フレーム
	constexpr float		REAR_ATTACK_COLLISION_END = 25.0f;												//後方攻撃の当たり判定終了フレーム

	constexpr float		SIDE_ATTACK_CHANGE_MATERIAL_START = 20.0f;										//回転攻撃のマテリアル変更開始フレーム
	constexpr float		SIDE_ATTACK_CHANGE_MATERIAL_END = 25.0f;										//回転攻撃のマテリアル変更終了フレーム
	constexpr float		SIDE_ATTACK_COLLISION_START = 25.0f;											//回転攻撃の当たり判定開始フレーム
	constexpr float		SIDE_ATTACK_COLLISION_END = 65.0f;												//回転攻撃の当たり判定終了フレーム

	constexpr float		JUMP_END_SIZE = 25.0f;															//ジャンプから次の状態に移行する最低距離

	constexpr float		CHARGE_CHANGE_MATERIAL_START_LEN = 500.0f;										//突進のマテリアル変更最低距離
	constexpr float		CHARGE_END_LEN = RAD + 10.0f;													//突進アクションから次の状態に移行する最低距離

	constexpr float		SPECIAL_START_END_LEN = 25.0f;													//必殺開始アクションから次の状態に移行する最低距離

	constexpr float		SPECIAL_CHARGE_ANIME_SPEED_CHANGE = 0.025f;										//必殺チャージ中のアニメーション再生速度加算値
	constexpr int		SPECIAL_CHARGE_END_TIME = 1200;													//必殺チャージ終了フレーム

	constexpr VECTOR	SPECIAL_INIT_VECTOR = { 0.0f,500.0f,0.0f };										//必殺の初めに座標を変更するときの値

	constexpr float		SPECIAL_POSY_CALC = -50.0f;														//必殺中のY座標計算値

	constexpr float		NORMAL_ATTACK1_COLLISION_RAD = 15.0f;											//通常攻撃１段目の攻撃当たり判定の半径
	constexpr float		NORMAL_ATTACK2_COLLISION_RAD = 20.0f;											//通常攻撃２段目の攻撃当たり判定の半径
	constexpr float		NORMAL_ATTACK3_COLLISION_RAD = 25.0f;											//通常攻撃３段目の攻撃当たり判定の半径
	constexpr float		REAR_ATTACK_COLLISION_RAD = 50.0f;												//後方攻撃の攻撃当たり判定の半径
	constexpr float		SIDE_ATTACK_COLLISION_RAD = 25.0f;												//回転攻撃の攻撃当たり判定の半径

	constexpr float		ANIME_SPEED = 0.35f;															//アニメーション再生スピード
	constexpr float		CHARGE_START_ANIME_SPEED = 0.5f;												//突進チャージアニメーション再生スピード
	constexpr float		SIDE_ATTACK_ANIME_SPEED = 0.5f;													//回転攻撃アニメーション再生スピード

	constexpr float		NORMAL_ATTACK1_POWER = 10.0f;													//通常攻撃１段目時の攻撃力
	constexpr float		NORAML_ATTACK2_POWER = 15.0f;													//通常攻撃２段目時の攻撃力
	constexpr float		NORAML_ATTACK3_POWER = 20.0f;													//通常攻撃３段目時の攻撃力
	constexpr float		REAR_ATTACK_POWER = 20.0f;														//後方攻撃時の攻撃力
	constexpr float		SIDE_ATTACK_POWER = 10.0f;														//回転攻撃時の攻撃力
	constexpr float		CHARGE_ATTACK_POWER = 25.0f;													//突進攻撃時の攻撃力

	constexpr int		DAMAGE_TIME = 20;																//ダメージ状態の継続時間

	constexpr float		DOWN_DAMAGE_TAKEN_MULT = 1.5f;													//ダウン時の被ダメ増加量

	constexpr float		PARRY_DOWN_POWER_THRESHOLD = 20.0f;												//パリィされたときにダウンへ移行する攻撃力
	constexpr float		PARRY_DOWN_TIME_MULT = 5.0f;													//パリィされたときに攻撃力に乗算してダウン時間を設定する

	constexpr float		ATTACK_PATTERN_HP_RATE_LOW = 0.5f;												//攻撃パターンの変化するHP割合
	constexpr float		ATTACK_PATTERN_HP_RATE_HIGH = 0.9f;												//攻撃パターンの変化するHP割合

	constexpr int		ATTACK_PATTERN_RANDOM_MAX = 1;													//攻撃パターンのランダム範囲
	constexpr int		ATTACK_PATTERN_LOW_HP_RANDOM_MAX = 2;											//攻撃パターンのランダム範囲


	constexpr int		ATTACK_PATTERN_LOW_HP_START_INDEX = 2;											//低HP時の攻撃パターン開始番号

	constexpr char		MODEL_FILE_PATH[] = ("Data/Model/Enemy//Boss/MainBody/Boss.mv1");				//モデルファイルパス
	constexpr char		ATTACK_CSV_FILE_PATH[] = ("Data/CSV/Boss/AttackPatterns/AttackPatterns.csv");	//攻撃パターンCSVのファイルパス
}

//コンストラクタ
Boss::Boss() {
	Init();
}
//デストラクタ
Boss::~Boss() {
	Exit();
}
//初期化処理
void Boss::Init() {
	EnemyBase::Init();

	m_Kinds = ENEMY;

	m_Pos = INIT_POS;									//座標
	m_Rad = RAD;										//半径
	m_Size = BOSS_SIZE;									//ボックス当たり判定
	m_Rot.y = m_Rot.y * -1.0f;							//Y軸回転値を反転

	m_HitPoints = HIT_POINTS;							//体力
	m_MaxHitPoints = m_HitPoints;						//最大体力

	m_FrameNumber = FRAME_NUM;							//最大ボーン数を保存

	m_State = IDEL;										//ボス状態変数
	m_PrevState = m_State;								//１フレーム前の状態
	m_DamageTime = 0;									//ダメージ処理の継続時間
	m_BeforJumpPos = VZERO;								//ジャンプ直前の座標を保存
	m_PredictedLandingPos = VZERO;						//着地予定座標
	m_SpecialChargeTime = 0;							//必殺チャージの継続時間
	m_DirectNum = 0;									//どの方向判定ボーンと当たったかを保存する

	for (int Index = 0; Index <= FRAME_NUM; Index++) {
		FRAME_DATA FrameData;
		FrameData.Pos = VZERO;					//ボーン座標
		FrameData.Rad = 0.0f;					//ボーン半径
		FrameData.IsCollision = false;			//ボーン当たり判定
		FrameData.IsAttackFlg = false;			//ボーン攻撃判定
		m_FrameData.push_back(FrameData);
	}

	for (int Row = 0; Row < PATTERN_INDEX; Row++) {
		for (int Column = 0; Column < ATTACK_INDEX; Column++) {
			m_AttackPatterns[Row][Column] = -1;			//攻撃パターンの配列
		}
	}
	m_PatternIndex = 0;									//攻撃パターンの種類
	m_AttackIndex = -1;									//パターン内の攻撃順
	m_NextAttack = -1;									//次に行う予定の攻撃
}
//データ読み込み処理
void Boss::Load() {
	ObjectBase::Load(MODEL_FILE_PATH);

	//CSVファイルからデータを読み込む
	FILE* AttackPatternsFile;
	//ファイルを開く　失敗したらこれ以降の処理を行わない
	if (fopen_s(&AttackPatternsFile, ATTACK_CSV_FILE_PATH, "r") != 0)return;
	//データ取得
	for (int Column = 0; Column < PATTERN_INDEX; Column++) {
		for (int Row = 0; Row < ATTACK_INDEX; Row++) {
			//データ一つ分取得
			fscanf_s(AttackPatternsFile, "%d", &m_AttackPatterns[Column][Row]);
			//カンマor改行を飛ばす
			fgetc(AttackPatternsFile);
		}
	}
	//開いたファイルを閉じる
	fclose(AttackPatternsFile);
}
//毎フレーム呼び出す処理
void Boss::Step() {
	if (m_HitPoints <= 0) {
		m_HitPoints = 0;
		m_State = DEATH;
	}

	if (m_DamageTime <= 0) {
		//当たり判定オン
		m_IsCollision = true;
		//ダメージ処理の継続時間をリセット
		m_DamageTime = 0;
	}
	else {
		m_DamageTime--;
	}
	//当たり判定設定
	for (int Index = FRONT; Index < FRAME_NUM; Index++) {
		SetFrameDataIsCollision(Index, 50.0f);
	}
	//状態遷移
	StateManager();

#ifdef _DEBUG
	DrawFormatStringToHandle(50, 420, RED, DxLibFont::FONTHNDL_N20, "攻撃力:%.0f", m_Power);
	DrawFormatStringToHandle(50, 460, RED, DxLibFont::FONTHNDL_N20, "攻撃パターン配列番号:%d", m_PatternIndex);
	DrawFormatStringToHandle(50, 480, RED, DxLibFont::FONTHNDL_N20, "攻撃種配列番号:%d", m_AttackIndex);
	DrawFormatStringToHandle(50, 500, RED, DxLibFont::FONTHNDL_N20, "今の攻撃種:%d", m_State);
	DrawFormatStringToHandle(50, 520, RED, DxLibFont::FONTHNDL_N20, "次の攻撃種:%d", m_NextAttack);
	DrawFormatStringToHandle(50, 540, RED, DxLibFont::FONTHNDL_N20, "体力:%.0f", m_HitPoints);
	DrawFormatStringToHandle(50, 560, RED, DxLibFont::FONTHNDL_N20, "%d", m_IsCollision);
#endif // DEBUG
}
//当たり判定後の処理(当たっている場合)
void Boss::HitCalc(ObjectBase* _Object) {
	//プレイヤークラスデータを保存する変数
	Player* PointerPlayer = nullptr;
	//プレイヤークラスをダウンキャスト
	PointerPlayer = dynamic_cast<Player*>(_Object);
	if (PointerPlayer != nullptr) {
		if (PointerPlayer->GetIsParryCollision()) {
			if (m_Power >= PARRY_DOWN_POWER_THRESHOLD) {
				//ダウン状態へ
				m_State = DOWN;
				//ダウン状態継続時間を設定
				m_DownTime = (int)(m_Power * PARRY_DOWN_TIME_MULT);
			}
		}
		else {
			//ダウン状態の時
			if (m_State == DOWN) {
				//プレイヤーの攻撃力に被ダメ率を乗算後HPを消費
				m_HitPoints = m_HitPoints - (PointerPlayer->GetPower() * DOWN_DAMAGE_TAKEN_MULT);
			}
			else {
				//HPを消費
				m_HitPoints = m_HitPoints - PointerPlayer->GetPower();
			}
		}
		//ダメージ処理の継続時間セット
		m_DamageTime = DAMAGE_TIME;
		//当たり判定オフ
		m_IsCollision = false;
	}
}
//方向判定ボーンと当たった場合
void Boss::HitFrame(int _FrameNum) {
	m_DirectNum = _FrameNum;
}
//待機
void Boss::Idel() {
	//待機アニメーションループ再生
	RequestLoop(ANIME_IDEL);
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//全てのボーン攻撃判定を削除する
		AllDeleteFrameDataIsAttackFlg();
	}
	//設定した時間分待機を続けたら
	if (m_NextActionTime <= 0) {
		//歩き状態へ
		m_State = WALK;
		//初期化
		m_NextActionTime = 0;
	}
	else {
		m_NextActionTime--;
	}
}
//ダウン
void Boss::Down() {
	//ダウンアニメーションループ再生
	RequestLoop(ANIME_DOWN);

	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//輪郭線のマテリアルをマテリアル青に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_BLUE), FALSE);
		//全てのボーン攻撃判定を削除する
		AllDeleteFrameDataIsAttackFlg();
	}
	if (m_DownTime <= 0) {
		//待機状態へ
		m_State = IDEL;
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_BLACK), FALSE);
	}
	else {
		//ダウン状態継続時間を減算
		m_DownTime--;
	}
}
//死亡
void Boss::Death() {
	//死亡アニメーション再生
	RequestEndLoop(ANIME_DEATH, ANIME_SPEED);

	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_BLACK), FALSE);
		//全てのボーン攻撃判定を削除する
		AllDeleteFrameDataIsAttackFlg();
	}
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//生存フラグをオフ
		m_IsActive = false;
	}
}
//歩き
void Boss::Walk() {
	//歩きアニメーションループ再生
	RequestLoop(ANIME_WALK);
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
	}
	//行動管理
	ActionManager();
	if (m_State == WALK) {
		//正規化されたベクトルを取得
		VECTOR DirToPlayer = GetDirectionNotY(m_Pos, m_PlayerPos, TRUE);
		//1フレームで移動する距離を生成
		DirToPlayer = VScale(DirToPlayer, WALK_MULT);
		//座標に加算
		m_Pos = VAdd(m_Pos, DirToPlayer);
		//方向ベクトルを反転
		DirToPlayer = VScale(DirToPlayer, -1.0f);
		//移動方向を向く
		UpdateRotation(DirToPlayer, NORMAL_MOVE_ROTATE_SPEED);
	}
}
//通常攻撃１段目　攻撃終了(鼻)
void Boss::BreakNormalAttack1() {
	//通常攻撃１段目　攻撃終了(鼻)アニメーション再生
	RequestEndLoop(ANIME_BREAK_NORMAL_ATTACK1, ANIME_SPEED);
	//通常攻撃１段目共通処理
	NoormalAttack1(BREAK_NORMAL_ATTACK1);
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = IDEL;
	}
}
//通常攻撃１段目　攻撃継続(鼻)
void Boss::ChainNormalAttack1() {
	//通常攻撃１段目　攻撃継続(鼻)アニメーション再生
	RequestEndLoop(ANIME_CHAIN_NORMAL_ATTACK1, ANIME_SPEED);
	//通常攻撃１段目共通処理
	NoormalAttack1(CHAIN_NORMAL_ATTACK1);
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//攻撃パターン管理
		AttackPatternManager();
	}
}
//通常攻撃１段目共通処理
void Boss::NoormalAttack1(TagState _State) {
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//攻撃力設定
		m_Power = NORMAL_ATTACK1_POWER;
		///演出を実行オフ
		m_IsPerformance = false;
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK1_CHANGE_MATERIAL_START && m_AnimeData.Frame < NORMAL_ATTACK1_CHANGE_MATERIAL_END) {
		//輪郭線のマテリアルをマテリアル赤に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_RED), FALSE);
	}
	else {
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_BLACK), FALSE);
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK1_COLLISION_START && m_AnimeData.Frame < NORMAL_ATTACK1_COLLISION_END) {
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_ELEPHANT_NORMAL_ATTACK)) {
			SoundManager::Play(SoundManager::TagID::SE_ELEPHANT_NORMAL_ATTACK);
		}
		//ボーンに攻撃判定を生成
		SetFrameDataIsAttackFlg(NOSE004END, NORMAL_ATTACK1_COLLISION_RAD);
	}
	else {
		//ボーン攻撃判定を削除する
		DeleteFrameDataIsAttackFlg(NOSE004END);
	}
	if (!m_IsPerformance && m_AnimeData.Frame > NORMAL_ATTACK1_EFFECT_REQUEST) {
		///演出を実行オン
		m_IsPerformance = true;
		//指定ボーンの座標取得
		VECTOR Pos = GetFramePos(m_Hndl, NOSE004END);
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK01BLOW2, Pos, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
	}
}
//通常攻撃２段目　攻撃終了(牙振り上げ)
void Boss::BreakNormalAttack2() {
	//通常攻撃２段目　攻撃終了(牙振り上げ)アニメーション再生
	RequestEndLoop(ANIME_BREAK_NORMAL_ATTACK2, ANIME_SPEED);
	//通常攻撃２段目共通処理
	NoormalAttack2(BREAK_NORMAL_ATTACK2);
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = IDEL;
	}
}
//通常攻撃２段目　攻撃継続(牙振り上げ)
void Boss::ChainNormalAttack2() {
	//通常攻２段目　攻撃継続(鼻)アニメーション再生
	RequestEndLoop(ANIME_CHAIN_NORMAL_ATTACK2, ANIME_SPEED);
	//通常攻撃２段目共通処理
	NoormalAttack2(CHAIN_NORMAL_ATTACK2);
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//攻撃パターン管理
		AttackPatternManager();
	}
}
//通常攻撃２段目共通処理
void Boss::NoormalAttack2(TagState _State) {
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//攻撃力設定
		m_Power = NORAML_ATTACK2_POWER;
		///演出を実行オフ
		m_IsPerformance = false;
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK2_CHANGE_MATERIAL_START && m_AnimeData.Frame < NORMAL_ATTACK2_CHANGE_MATERIAL_END) {
		//輪郭線のマテリアルをマテリアル赤に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_RED), FALSE);
	}
	else {
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_BLACK), FALSE);
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK2_COLLISION_START && m_AnimeData.Frame < NORMAL_ATTACK2_COLLISION_END) {
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_ELEPHANT_NORMAL_ATTACK)) {
			SoundManager::Play(SoundManager::TagID::SE_ELEPHANT_NORMAL_ATTACK);
		}
		//ボーンに攻撃判定を生成
		SetFrameDataIsAttackFlg(FANG003END_LEFT, NORMAL_ATTACK2_COLLISION_RAD);
		SetFrameDataIsAttackFlg(FANG003END_RIGHT, NORMAL_ATTACK2_COLLISION_RAD);
	}
	else {
		//ボーン攻撃判定を削除する
		DeleteFrameDataIsAttackFlg(FANG003END_LEFT);
		DeleteFrameDataIsAttackFlg(FANG003END_RIGHT);
	}
	if (!m_IsPerformance && m_AnimeData.Frame > NORMAL_ATTACK2_EFFECT_REQUEST) {
		///演出を実行オン
		m_IsPerformance = true;
		//指定ボーンの座標取得
		VECTOR Pos1 = GetFramePos(m_Hndl, FANG003END_LEFT);
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK01BLOW12, Pos1, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
		//指定ボーンの座標取得
		VECTOR Pos2 = GetFramePos(m_Hndl, FANG003END_RIGHT);
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK01BLOW12, Pos2, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
	}
}
//通常攻撃３段目　攻撃終了(踏みつけ)
void Boss::BreakNormalAttack3() {
	//通常攻撃３段目　攻撃終了(踏みつけ)アニメーション再生
	RequestEndLoop(ANIME_BREAK_NORMAL_ATTACK3, ANIME_SPEED);


	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//攻撃力設定
		m_Power = NORAML_ATTACK3_POWER;
		///演出を実行オフ
		m_IsPerformance = false;
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK3_CHANGE_MATERIAL_START && m_AnimeData.Frame < NORMAL_ATTACK3_CHANGE_MATERIAL_END) {
		//輪郭線のマテリアルをマテリアル赤に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_RED), FALSE);
	}
	else {
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_BLACK), FALSE);
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK3_COLLISION_START && m_AnimeData.Frame < NORMAL_ATTACK3_COLLISION_END) {
		//ボーンに攻撃判定を生成
		SetFrameDataIsAttackFlg(PALMEND_LEFT, NORMAL_ATTACK3_COLLISION_RAD);
		SetFrameDataIsAttackFlg(PALMEND_RIGHT, NORMAL_ATTACK3_COLLISION_RAD);
	}
	else {
		//ボーン攻撃判定を削除する
		DeleteFrameDataIsAttackFlg(PALMEND_LEFT);
		DeleteFrameDataIsAttackFlg(PALMEND_RIGHT);
	}
	if (!m_IsPerformance && m_AnimeData.Frame > NORMAL_ATTACK3_EFFECT_REQUEST) {
		///演出を実行オン
		m_IsPerformance = true;
		//指定ボーンの座標取得
		VECTOR Pos1 = GetFramePos(m_Hndl, PALMEND_LEFT);
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK01BLOW3, Pos1, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
		//指定ボーンの座標取得
		VECTOR Pos2 = GetFramePos(m_Hndl, PALMEND_RIGHT);
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK01BLOW3, Pos2, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_ELEPHANT_STRONG_ATTACK)) {
			SoundManager::Play(SoundManager::TagID::SE_ELEPHANT_STRONG_ATTACK);
		}
	}
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = IDEL;
	}
}
//後方攻撃
void Boss::RearAttack() {
	//後方攻撃アニメーション再生
	RequestEndLoop(ANIME_REAR_ATTACK, ANIME_SPEED);
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//攻撃力設定
		m_Power = REAR_ATTACK_POWER;
	}
	if (m_AnimeData.Frame > REAR_ATTACK_CHANGE_MATERIAL_START && m_AnimeData.Frame < REAR_ATTACK_CHANGE_MATERIAL_END) {
		//輪郭線のマテリアルをマテリアル赤に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_RED), FALSE);
	}
	else {
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_BLACK), FALSE);
	}
	if (m_AnimeData.Frame > REAR_ATTACK_COLLISION_START && m_AnimeData.Frame < REAR_ATTACK_COLLISION_END) {
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_ELEPHANT_STRONG_ATTACK)) {
			SoundManager::Play(SoundManager::TagID::SE_ELEPHANT_STRONG_ATTACK);
		}
		//ボーンに攻撃判定を生成
		SetFrameDataIsAttackFlg(TOESEND_LEFT, REAR_ATTACK_COLLISION_RAD);
		SetFrameDataIsAttackFlg(TOESEND_RIGHT, REAR_ATTACK_COLLISION_RAD);
		//正規化された方向ベクトルを取得
		VECTOR DirToPlayer = GetDirectionNotY(m_Pos, GetFrameDataPos(FRONT), TRUE);
		//1フレームで移動する距離を生成
		DirToPlayer = VScale(DirToPlayer, REAR_ATTACK_MULT);
		//座標に加算
		m_Pos = VAdd(m_Pos, DirToPlayer);
		//移動方向を向く
		m_Rot.y = atan2f(DirToPlayer.x, DirToPlayer.z);
	}
	else {
		//ボーン攻撃判定を削除する
		DeleteFrameDataIsAttackFlg(TOESEND_LEFT);
		DeleteFrameDataIsAttackFlg(TOESEND_RIGHT);
	}
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		if (m_NextAttack == CHARGE_ATTACK_START) {
			AttackPatternManager();
		}
		else {
			//待機状態へ
			m_State = IDEL;
		}
	}
}
//回転攻撃
void Boss::SideAttack() {
	//後方攻撃アニメーション再生
	RequestEndLoop(ANIME_SIDE_ATTACK, SIDE_ATTACK_ANIME_SPEED);
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//攻撃力設定
		m_Power = SIDE_ATTACK_POWER;
	}
	if (m_AnimeData.Frame > SIDE_ATTACK_CHANGE_MATERIAL_START && m_AnimeData.Frame < SIDE_ATTACK_CHANGE_MATERIAL_END) {
		//輪郭線のマテリアルをマテリアル赤に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_RED), FALSE);
	}
	else {
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_BLACK), FALSE);
	}
	if (m_AnimeData.Frame > SIDE_ATTACK_COLLISION_START && m_AnimeData.Frame < SIDE_ATTACK_COLLISION_END) {
		//ボーンに攻撃判定を生成
		SetFrameDataIsAttackFlg(FANG003END_LEFT, SIDE_ATTACK_COLLISION_RAD);
		SetFrameDataIsAttackFlg(FANG003END_RIGHT, SIDE_ATTACK_COLLISION_RAD);
	}
	else {
		//ボーン攻撃判定を削除する
		DeleteFrameDataIsAttackFlg(FANG003END_LEFT);
		DeleteFrameDataIsAttackFlg(FANG003END_RIGHT);
	}
	if (m_AnimeData.Frame > SIDE_ATTACK_COLLISION_END) {
		//正規化された方向ベクトルを取得
		VECTOR DirToPlayer = GetDirectionNotY(m_PlayerPos, m_Pos, TRUE);
		//移動方向を向く
		UpdateRotation(DirToPlayer, NORMAL_MOVE_ROTATE_SPEED);
	}
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = IDEL;
	}
}
//突進直前移動
void Boss::Jump() {
	//待機アニメーションループ再生
	RequestLoop(ANIME_IDEL, ANIME_SPEED);

	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//着地予定座標を設定
		m_PredictedLandingPos = { 0.0f,0.0f,-500.0f };
		//現在の座標を保存
		m_BeforJumpPos = m_Pos;
	}
	//現在の座標から着地予定座標への方向ベクトルを取得
	VECTOR DirToPredictedLanding = GetDirectionNotY(m_Pos, m_PredictedLandingPos);
	//サイズ取得
	float Len1 = VSize(DirToPredictedLanding);
	//ジャンプ直前の座標から着地予定座標への方向ベクトル
	VECTOR PosCalc = GetDirectionNotY(m_BeforJumpPos, m_PredictedLandingPos);
	//サイズ取得
	float  Len2 = VSize(PosCalc);
	//値を半分に
	Len2 = Len2 * 0.5f;

	float floatCalc = Len1 - Len2;

	if (Len1 > JUMP_END_SIZE) {
		//正規化したものを取得
		DirToPredictedLanding = VNorm(DirToPredictedLanding);
		//1フレームで移動する距離を生成
		DirToPredictedLanding = VScale(DirToPredictedLanding, JUMP_MULT);
		//座標に加算
		m_Pos = VAdd(m_Pos, DirToPredictedLanding);
		//ジャンプ計算
		m_Pos.y += (floatCalc * 0.05f);
	}
	else {
		//突進チャージ状態へ
		m_State = CHARGE_ATTACK_START;
		//Y軸を地面に設置
		m_Pos.y = 0;
	}
}
//突進チャージ
void Boss::ChargeAttackStart() {
	//突進チャージアニメーション再生
	RequestEndLoop(ANIME_CHARGE_ATTACK_START, CHARGE_START_ANIME_SPEED);
	//正規化された方向ベクトルを取得
	VECTOR DirToPlayer = GetDirectionNotY(m_Pos, m_PlayerPos, TRUE);
	//方向ベクトルを反転
	DirToPlayer = VScale(DirToPlayer, -1.0f);
	//移動方向を向く
	UpdateRotation(DirToPlayer, NORMAL_MOVE_ROTATE_SPEED);

	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		///演出を実行オフ
		m_IsPerformance = false;
	}
	if (!m_IsPerformance) {
		///演出を実行オン
		m_IsPerformance = true;
		//指定ボーンの座標取得
		VECTOR Pos = m_Pos;
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::ANDREWFM01BLUE_LASER, Pos, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
	}
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//突進状態へ
		m_State = CHARGE;
	}
}
//突進
void Boss::Charge() {
	//突進アニメーションループ再生
	RequestLoop(ANIME_CHARGE, ANIME_SPEED);
	//方向ベクトルを取得
	VECTOR DirToPlayer = GetDirectionNotY(m_Pos, m_PlayerPos);
	//サイズ取得
	float Len = VSize(DirToPlayer);

	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		///演出を実行オフ
		m_IsPerformance = false;
	}
	if (!m_IsPerformance) {
		///演出を実行オン
		m_IsPerformance = true;
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_ELEPHANT_LUNGES)) {
			SoundManager::Play(SoundManager::TagID::SE_ELEPHANT_LUNGES);
		}
		//指定ボーンの座標取得
		VECTOR Pos = m_Pos;
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK02BLOW3, Pos, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
	}
	if (Len > CHARGE_END_LEN) {
		//正規化したものを取得
		DirToPlayer = VNorm(DirToPlayer);
		//1フレームで移動する距離を生成
		DirToPlayer = VScale(DirToPlayer, CHARGE_MULT);
		//座標に加算
		m_Pos = VAdd(m_Pos, DirToPlayer);
		//方向ベクトルを反転
		DirToPlayer = VScale(DirToPlayer, -1.0f);
		//移動方向を向く
		UpdateRotation(DirToPlayer, NORMAL_MOVE_ROTATE_SPEED);
	}
	else {
		//突進振り上げ状態へ
		m_State = CHARGE_ATTACK;
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_BLACK), FALSE);
	}
	if (Len < CHARGE_CHANGE_MATERIAL_START_LEN) {
		//輪郭線のマテリアルをマテリアル赤に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_RED), FALSE);
	}
}
//突進振り上げ
void Boss::ChargeAttack() {
	//突進振り上げアニメーション再生
	RequestEndLoop(ANIME_CHARGE_ATTACK, ANIME_SPEED);

	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//攻撃力設定
		m_Power = CHARGE_ATTACK_POWER;
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::GetHndl(LoadMaterial::TagMaterial::MATERIAL_BLACK), FALSE);
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_ELEPHANT_STRONG_ATTACK)) {
			SoundManager::Play(SoundManager::TagID::SE_ELEPHANT_STRONG_ATTACK);
		}
		///演出を実行オフ
		m_IsPerformance = false;
	}
	if (!m_IsPerformance && m_AnimeData.Frame) {
		///演出を実行オン
		m_IsPerformance = true;
		//指定ボーンの座標取得
		VECTOR Pos1 = GetFramePos(m_Hndl, FANG003END_LEFT);
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK01BLOW12, Pos1, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
		//指定ボーンの座標取得
		VECTOR Pos2 = GetFramePos(m_Hndl, FANG003END_RIGHT);
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK01BLOW12, Pos2, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
	}
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = IDEL;
		//ボーン攻撃判定を削除する
		DeleteFrameDataIsAttackFlg(FANG003END_LEFT);
		DeleteFrameDataIsAttackFlg(FANG003END_RIGHT);
	}
	else {
		//ボーンに攻撃判定を生成
		SetFrameDataIsAttackFlg(FANG003END_LEFT, NORMAL_ATTACK2_COLLISION_RAD);
		SetFrameDataIsAttackFlg(FANG003END_RIGHT, NORMAL_ATTACK2_COLLISION_RAD);
	}
}
//必殺開始
void Boss::SpecialStart() {
	//必殺開始アニメーション再生
	RequestEndLoop(ANIME_SPECIAL_START, ANIME_SPEED);

	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
	}
	//方向ベクトルを取得
	VECTOR DirToZero = GetDirectionNotY(m_Pos, VZERO);
	//サイズ取得
	float Len = VSize(DirToZero);

	if (m_AnimeData.EndFlg && Len < SPECIAL_START_END_LEN) {
		//必殺チャージ状態へ
		m_State = SPECIAL_CHARGE;
	}
	else if (Len < SPECIAL_START_END_LEN) {
		//座標を固定
		m_Pos = VZERO;
	}
	else {
		//正規化したものを取得
		DirToZero = VNorm(DirToZero);
		//1フレームで移動する距離を生成
		DirToZero = VScale(DirToZero, SPECIALSTART_MULT);
		//座標に加算
		m_Pos = VAdd(m_Pos, DirToZero);
		//方向ベクトルを反転
		DirToZero = VScale(DirToZero, -1.0f);
		//移動方向を向く
		UpdateRotation(DirToZero, NORMAL_MOVE_ROTATE_SPEED);
	}
}
//必殺チャージ
void Boss::SpecialCharge() {
	//必殺チャージアニメーションループ再生
	RequestLoop(ANIME_SPECIAL_CHAGE, ANIME_SPEED);

	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
	}
	//アニメーションの再生速度を取得
	float AnimeSpeed = m_AnimeData.Speed;
	//加算
	AnimeSpeed += SPECIAL_CHARGE_ANIME_SPEED_CHANGE;
	//セット
	SetAnimeSpeed(AnimeSpeed);
	//必殺チャージ継続時間加算
	m_SpecialChargeTime++;
	if (m_SpecialChargeTime >= SPECIAL_CHARGE_END_TIME) {
		//必殺へ
		m_State = SPECIAL;
		//初期化
		m_SpecialChargeTime = 0;
	}
}
//必殺
void Boss::Special() {
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//座標を変更
		m_Pos = SPECIAL_INIT_VECTOR;
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_ELEPHANT_CRY)) {
			SoundManager::Play(SoundManager::TagID::SE_ELEPHANT_CRY);
		}
	}
	if (m_Pos.y <= 0.0f) {
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK02BLOW2, m_Pos, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
		//初期化
		m_Pos.y = 0.0;
		//待機状態へ
		m_State = IDEL;
	}
	else {
		//Y軸のみ計算をする
		m_Pos.y += SPECIAL_POSY_CALC;
	}
}
//行動管理
void Boss::ActionManager() {
	VECTOR DistanceToPlayer = GetDirectionNotY(m_Pos, m_PlayerPos);
	float ToPlayerLen = VSize(DistanceToPlayer);

	if (ToPlayerLen > ACTION_IDEL_DISTANCE) {
		//待機状態へ
		m_State = IDEL;
		return;
	}
	if (ToPlayerLen <= ACTION_ATTACK_DISTANCE) {
		//攻撃パターン管理
		AttackPatternManager();
		return;
	}
}
//攻撃パターン管理
void Boss::AttackPatternManager() {
	switch (m_DirectNum) {
	case REAR:
	case REAR_END:
		m_State = REAR_ATTACK;
		break;
	case RIGHT:
	case RIGHT_END:
	case LEFT:
	case LEFT_END:
		m_State = SIDE_ATTACK;
		break;
	default:
		//攻撃種配列を１つずらす
		m_AttackIndex++;
		//攻撃種配列の最大格納量より多ければ
		if (m_AttackIndex >= ATTACK_INDEX) {
			//体力を一時保存
			float HitPoints = m_HitPoints;
			//体力の割合を取得
			float HitPointsRate = m_HitPoints / m_MaxHitPoints;
			//体力の割合が一定以下なら
			if (HitPointsRate < ATTACK_PATTERN_HP_RATE_LOW) {
				//攻撃パターン配列を変更
				m_PatternIndex = ATTACK_PATTERN_LOW_HP_START_INDEX + GetRand(ATTACK_PATTERN_LOW_HP_RANDOM_MAX);
			}
			else if (HitPointsRate < ATTACK_PATTERN_HP_RATE_HIGH) {
				//攻撃パターン配列を変更
				m_PatternIndex = GetRand(ATTACK_PATTERN_RANDOM_MAX);
			}
			//先頭にリセット
			m_AttackIndex = 0;
		}
		//二次元配列に合致する位置の数値を取得する
		int Attack = m_AttackPatterns[m_PatternIndex][m_AttackIndex];
		//状態変化
		m_State = (TagState)Attack;

		//次の攻撃は何の予定か調べる
		int NextAttackIndex = m_AttackIndex + 1;
		//最大格納量より多ければ-1を入れておく
		if (NextAttackIndex >= ATTACK_INDEX) {
			m_NextAttack = -1;
		}
		else {
			//二次元配列に合致する位置の数値を取得する
			int NextAttack = m_AttackPatterns[m_PatternIndex][NextAttackIndex];
			//次の攻撃を保存
			m_NextAttack = (TagState)NextAttack;
		}
		//正規化された方向ベクトルを取得
		VECTOR DirToPlayer = GetDirectionNotY(m_Pos, m_PlayerPos, TRUE);
		//方向ベクトルを反転
		DirToPlayer = VScale(DirToPlayer, -1.0f);
		//移動方向を向く
		UpdateRotation(DirToPlayer, NORMAL_MOVE_ROTATE_SPEED);
		break;
	}
}
//ジャンプ時の着地地点管理
void Boss::JumpPosManger() {

}
//状態遷移
void Boss::StateManager() {
	switch (m_State) {
	case IDEL:						//待機
		Idel();
		break;
	case DOWN:						//ダウン
		Down();
		break;
	case DEATH:						//死亡
		Death();
		break;
	case WALK:						//歩き
		Walk();
		break;
	case BREAK_NORMAL_ATTACK1:		//通常攻撃１段目　攻撃終了(鼻)
		BreakNormalAttack1();
		break;
	case CHAIN_NORMAL_ATTACK1:		//通常攻撃１段目　攻撃継続(鼻)
		ChainNormalAttack1();
		break;
	case BREAK_NORMAL_ATTACK2:		//通常攻撃２段目　攻撃終了(牙振り上げ)
		BreakNormalAttack2();
		break;
	case CHAIN_NORMAL_ATTACK2:		//通常攻撃２段目　攻撃継続(牙振り上げ)
		ChainNormalAttack2();
		break;
	case BREAK_NORMAL_ATTACK3:		//通常攻撃３段目　攻撃終了(踏みつけ)
		BreakNormalAttack3();
		break;
	case REAR_ATTACK:				//後方攻撃
		RearAttack();
		break;
	case SIDE_ATTACK:				//回転攻撃
		SideAttack();
		break;
	case JUMP:						//突進直前移動
		Jump();
		break;
	case CHARGE_ATTACK_START:		//突進チャージ
		ChargeAttackStart();
		break;
	case CHARGE:					//突進
		Charge();
		break;
	case CHARGE_ATTACK:				//突進振り上げ
		ChargeAttack();
		break;
	case SPECIAL_START:				//必殺開始
		SpecialStart();
		break;
	case SPECIAL_CHARGE:			//必殺チャージ
		SpecialCharge();
		break;
	case SPECIAL:					//必殺
		Special();

		break;
	}
}
//当たり判定設定
void Boss::SetFrameDataIsCollision(int _FrameNamber, float _Rad) {
	m_FrameData[_FrameNamber].Pos = GetFramePos(m_Hndl, _FrameNamber);
	m_FrameData[_FrameNamber].Rad = _Rad;
	m_FrameData[_FrameNamber].IsCollision = true;
}
//指定のボーン当たり判定を削除する
void Boss::DeleteFrameDataIsCollision(int _FrameNamber) {
	m_FrameData[_FrameNamber].Pos = VZERO;
	m_FrameData[_FrameNamber].Rad = 0.0f;
	m_FrameData[_FrameNamber].IsCollision = false;
}
//全てのボーン当たり判定を削除する
void Boss::AllDeleteFrameDataIsCollision() {
	for (int Index = 0; Index < FRAME_NUM; Index++) {
		//指定のボーン攻撃判定を削除する
		DeleteFrameDataIsCollision(Index);
	}
}
//指定のボーン攻撃判定を設定
void Boss::SetFrameDataIsAttackFlg(int _FrameNamber, float _Rad) {
	m_FrameData[_FrameNamber].Pos = GetFramePos(m_Hndl, _FrameNamber);
	m_FrameData[_FrameNamber].Rad = _Rad;
	m_FrameData[_FrameNamber].IsAttackFlg = true;
}
//指定のボーン攻撃判定を削除する
void Boss::DeleteFrameDataIsAttackFlg(int _FrameNamber) {
	m_FrameData[_FrameNamber].Pos = VZERO;
	m_FrameData[_FrameNamber].Rad = 0.0f;
	m_FrameData[_FrameNamber].IsAttackFlg = false;
}
//全てのボーン攻撃判定を削除する
void Boss::AllDeleteFrameDataIsAttackFlg() {
	for (int Index = 0; Index < FRAME_NUM; Index++) {
		//指定のボーン攻撃判定を削除する
		DeleteFrameDataIsAttackFlg(Index);
	}
}
