#include "Boss.h"

namespace {
	const VECTOR	INIT_POS = { 0.0f,0.0f,-500.0f };												//初期座標

	const float		RAD = 5.0f;																		//半径
	const VECTOR	BOSS_SIZE = { RAD,RAD,RAD };													//ボックス当たり判定

	const float		HIT_POINTS = 1000.0f;															//体力
	const float		MAX_HITPOINTS = 1000.0f;														//最大体力

	const float		ACTION_WAIT_DISTANCE = 1000.0f;													//WAITに移行するプレイヤーとの距離
	const float		ACTION_ATTACK_DISTANCE = 75.0f;													//攻撃に移行するプレイヤーとの距離

	const float		WALK_MULT = 5.0f;																//歩き時の移動乗算値
	const float		REAR_ATTACK_MULT = -7.5f;														//後方攻撃の移動乗算値
	const float		JUMP_MULT = 10.0f;																//ジャンプの移動乗算値
	const float		CHARGE_MULT = 25.0f;															//突進の移動乗算値

	const float		NORMAL_ATTACK1_CHANGE_MATERIAL_START = 7.0f;									//通常攻撃１段目のマテリアル変更開始フレーム
	const float		NORMAL_ATTACK1_CHANGE_MATERIAL_END = 10.0f;										//通常攻撃１段目のマテリアル変更終了フレーム
	const float		NORMAL_ATTACK1_COLLISION_START = 10.0f;											//通常攻撃１段目の当たり判定開始フレーム
	const float		NORMAL_ATTACK1_COLLISION_END = 15.0f;											//通常攻撃１段目の当たり判定終了フレーム
	const float		NORMAL_ATTACK1_EFFECT_REQUEST = 12.0f;											//通常攻撃１段目のエフェクト呼び出しフレーム

	const float		NORMAL_ATTACK2_CHANGE_MATERIAL_START = 5.0f;									//通常攻撃２段目のマテリアル変更開始フレーム
	const float		NORMAL_ATTACK2_CHANGE_MATERIAL_END = 10.0f;										//通常攻撃２段目のマテリアル変更終了フレーム
	const float		NORMAL_ATTACK2_COLLISION_START = 10.0f;											//通常攻撃２段目の当たり判定開始フレーム
	const float		NORMAL_ATTACK2_COLLISION_END = 20.0f;											//通常攻撃２段目の当たり判定終了フレーム
	const float		NORMAL_ATTACK2_EFFECT_REQUEST = 15.0f;											//通常攻撃２段目のエフェクト呼び出しフレーム

	const float		NORMAL_ATTACK3_CHANGE_MATERIAL_START = 25.0f;									//通常攻撃３段目のマテリアル変更開始フレーム
	const float		NORMAL_ATTACK3_CHANGE_MATERIAL_END = 30.0f;										//通常攻撃３段目のマテリアル変更終了フレーム
	const float		NORMAL_ATTACK3_COLLISION_START = 30.0f;											//通常攻撃３段目の当たり判定開始フレーム
	const float		NORMAL_ATTACK3_COLLISION_END = 40.0f;											//通常攻撃３段目の当たり判定終了フレーム
	const float		NORMAL_ATTACK3_EFFECT_REQUEST = 40.0f;											//通常攻撃３段目のエフェクト呼び出しフレーム

	const float		REAR_ATTACK_CHANGE_MATERIAL_START = 10.0f;										//後方攻撃のマテリアル変更開始フレーム
	const float		REAR_ATTACK_CHANGE_MATERIAL_END = 15.0f;										//後方攻撃のマテリアル変更終了フレーム
	const float		REAR_ATTACK_COLLISION_START = 15.0f;											//後方攻撃の当たり判定開始フレーム
	const float		REAR_ATTACK_COLLISION_END = 25.0f;												//後方攻撃の当たり判定終了フレーム

	const float		JUMP_END_SIZE = 25.0f;															//ジャンプから次の状態に移行する最低距離

	const float		CHARGE_CHANGE_MATERIAL_START_LEN = 500.0f;										//突進のマテリアル変更最低距離
	const float		CHARGE_END_LEN = 75.0f;															//突進アクションから次の状態に移行する最低距離

	const float		SPECIAL_START_END_LEN = 25.0f;													//必殺開始アクションから次の状態に移行する最低距離

	const float		SPECIAL_CHARGE_ANIME_SPEED_CHANGE = 0.025;										//必殺チャージ中のアニメーション再生速度加算値
	const int		SPECIAL_CHARGE_END_TIME = 1200;													//必殺チャージ終了フレーム

	const VECTOR	SPECIAL_INIT_VECTOR = { FZERO,500.0f,FZERO };									//必殺の初めに座標を変更するときの値

	const float		SPECIAL_POSY_CALC = -50.0f;														//必殺中のY座標計算値

	const float		NORMAL_ATTACK1_COLLISION_RAD = 20.0f;											//通常攻撃１段目の攻撃当たり判定の半径
	const float		NORMAL_ATTACK2_COLLISION_RAD = 30.0f;											//通常攻撃２段目の攻撃当たり判定の半径
	const float		NORMAL_ATTACK3_COLLISION_RAD = 50.0f;											//通常攻撃３段目の攻撃当たり判定の半径
	const float		REAR_ATTACK_COLLISION_RAD = 50.0f;												//後方攻撃の攻撃当たり判定の半径

	const float		ANIME_SPEED = 0.4f;																//アニメーション再生スピード

	const float		NORMAL_ATTACK1_POWER = 10.0f;													//通常攻撃１段目時の攻撃力
	const float		NORAML_ATTACK2_POWER = 15.0f;													//通常攻撃２段目時の攻撃力
	const float		NORAML_ATTACK3_POWER = 20.0f;													//通常攻撃３段目時の攻撃力
	const float		REAR_ATTACK_POWER = 20.0f;														//後方攻撃時の攻撃力

	const char		MODEL_FILE_PATH[] = ("Data/Model/Enemy/Boss/MainBody/Boss.mv1");				//モデルファイルパス
	const char		ATTACK_CSV_FILE_PATH[] = ("Data/CSV/Boss/AttackPatterns/AttackPatterns.csv");	//攻撃パターンCSVのファイルパス
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

	m_Pos = INIT_POS;									//座標
	m_Rad = RAD;										//半径
	m_Size = BOSS_SIZE;									//ボックス当たり判定

	m_HitPoints = HIT_POINTS;							//体力

	for (int Index = 0; Index < STATE_NUM; Index++) {
		m_IsAction[Index] = false;						//アクションフラグ
		m_IsActionSuccess[Index] = false;				//アクション成功判定フラグ
	}

	m_State = WAIT;										//ボス状態変数
	m_IsDamage = false;									//ダメージ処理中か
	m_DamageTime = 0;									//ダメージ処理の継続時間
	m_BeforJumpPos = VZERO;								//ジャンプ直前の座標を保存
	m_PredictedLandingPos = VZERO;						//着地予定座標
	m_SpecialChargeTime = 0;								//必殺チャージの継続時間

	for (int Index = 0; Index < FARAM_NUM; Index++) {
		m_FrameData[Index].Pos = VZERO;					//ボーン座標
		m_FrameData[Index].Rad = FZERO;					//ボーン半径
		m_FrameData[Index].IsHitFlg = false;			//ボーン当たり判定
		m_FrameData[Index].IsAttackFlg = false;			//ボーン攻撃判定
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
	for (int Column = 0; Column < ATTACK_INDEX; Column++) {
		for (int Row = 0; Row < PATTERN_INDEX; Row++) {
			//データ一つ分取得
			fscanf_s(AttackPatternsFile, "%d", &m_AttackPatterns[Row][Column]);
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
		return;
	}
	//状態遷移
	StateManager();
}
//待機
void Boss::Wait() {
	//待機アニメーションループ再生
	RequestLoop(ANIME_WAIT);

	if (!m_IsAction[WAIT]) {
		//ダウンアクション中に変更
		m_IsAction[WAIT] = true;
		//全てのボーン攻撃判定を削除する
		AllDeleteFrameDataIsHitFlg();
	}
	//設定した時間分待機を続けたら
	if (m_NextActionTime >= 0) {
		//歩き状態へ
		m_State = WALK;
		//待機アクション終了
		m_IsAction[WAIT] = false;
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

	if (!m_IsAction[DOWN]) {
		//ダウンアクション中に変更
		m_IsAction[DOWN] = true;
		//輪郭線のマテリアルをマテリアル青に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::MATERIAL_BLUE, FALSE);
		//全てのボーン攻撃判定を削除する
		AllDeleteFrameDataIsHitFlg();
	}
	if (m_DownTime <= 0) {
		//待機状態へ
		m_State = WAIT;
		//ダウンアクション終了
		m_IsAction[DOWN] = false;
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::MATERIAL_BLACK, FALSE);
	}
	else {
		m_DownTime--;
	}
}
//死亡
void Boss::Death() {
	//死亡アニメーション再生
	RequestEndLoop(ANIME_DEATH);

	if (!m_IsAction[DEATH]) {
		//死亡アクション中に変更
		m_IsAction[DEATH] = true;
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::MATERIAL_BLUE, FALSE);
		//全てのボーン攻撃判定を削除する
		AllDeleteFrameDataIsHitFlg();
	}
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//死亡アクション終了
		m_IsAction[DEATH] = false;
		//生存フラグをオフ
		m_IsActive = false;
	}
}
//歩き
void Boss::Walk() {
	//歩きアニメーションループ再生
	RequestLoop(ANIME_WALK);

	if (!m_IsAction[WALK]) {
		//歩きアクション中に変更
		m_IsAction[WALK] = true;
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
		//角度を計算
		float RotY = atan2f(-DirToPlayer.x, -DirToPlayer.z);
		//Y軸回転値に代入
		m_Rot.y = RotY;
	}
	else {
		//歩きアクション終了
		m_IsAction[WALK] = false;
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
		m_State = WAIT;
		//通常攻撃１段目アクション終了
		m_IsAction[BREAK_NORMAL_ATTACK1] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
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
		//通常攻撃１段目アクション終了
		m_IsAction[CHAIN_NORMAL_ATTACK1] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
	}
}
//通常攻撃１段目共通処理
void Boss::NoormalAttack1(TagState _State) {
	if (!m_IsAction[_State]) {
		//通常攻撃１段目アクション中に変更
		m_IsAction[_State] = true;
		//攻撃力設定
		m_Power = NORMAL_ATTACK1_POWER;
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK1_CHANGE_MATERIAL_START && m_AnimeData.Frame < NORMAL_ATTACK1_CHANGE_MATERIAL_END) {
		//輪郭線のマテリアルをマテリアル赤に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::MATERIAL_RED, FALSE);
	}
	else {
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::MATERIAL_BLACK, FALSE);
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK1_COLLISION_START && m_AnimeData.Frame < NORMAL_ATTACK1_COLLISION_END) {
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_WEAKATK)) {
			SoundManager::Play(SoundManager::TagID::SE_WEAKATK);
		}
		//ボーンに攻撃判定を生成
		SetFrameDataIsAttackFlg(NOSE004END, NORMAL_ATTACK1_COLLISION_RAD);
	}
	else {
		//ボーン攻撃判定を削除する
		DeleteFrameDataIsHitFlg(NOSE004END);
	}
	if (!m_IsEffect && m_AnimeData.Frame > NORMAL_ATTACK1_EFFECT_REQUEST) {
		//エフェクト発生判定オン
		m_IsEffect = true;
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
		m_State = WAIT;
		//通常攻撃２段目アクション終了
		m_IsAction[BREAK_NORMAL_ATTACK2] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
	}
}
//通常攻撃２段目　攻撃継続(牙振り上げ)
void Boss::ChainNormalAttack2() {
	//通常攻２段目　攻撃継続(鼻)アニメーション再生
	RequestEndLoop(ANIME_CHAIN_NORMAL_ATTACK2, ANIME_SPEED);
	//通常攻撃２段目共通処理
	NoormalAttack1(CHAIN_NORMAL_ATTACK2);
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//攻撃パターン管理
		AttackPatternManager();
		//通常攻撃２段目アクション終了
		m_IsAction[CHAIN_NORMAL_ATTACK2] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
	}
}
//通常攻撃２段目共通処理
void Boss::NoormalAttack2(TagState _State) {
	if (!m_IsAction[_State]) {
		//通常攻撃２段目アクション中に変更
		m_IsAction[_State] = true;
		//攻撃力設定
		m_Power = NORAML_ATTACK2_POWER;
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK2_CHANGE_MATERIAL_START && m_AnimeData.Frame < NORMAL_ATTACK2_CHANGE_MATERIAL_END) {
		//輪郭線のマテリアルをマテリアル赤に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::MATERIAL_RED, FALSE);
	}
	else {
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::MATERIAL_BLACK, FALSE);
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK2_COLLISION_START && m_AnimeData.Frame < NORMAL_ATTACK2_COLLISION_END) {
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_MEDIUMATK)) {
			SoundManager::Play(SoundManager::TagID::SE_MEDIUMATK);
		}
		//ボーンに攻撃判定を生成
		SetFrameDataIsAttackFlg(FANG003END_LEFT, NORMAL_ATTACK2_COLLISION_RAD);
		SetFrameDataIsAttackFlg(FANG003END_RIGHT, NORMAL_ATTACK2_COLLISION_RAD);
	}
	else {
		//ボーン攻撃判定を削除する
		DeleteFrameDataIsHitFlg(FANG003END_LEFT);
		DeleteFrameDataIsHitFlg(FANG003END_RIGHT);
	}
	if (!m_IsEffect && m_AnimeData.Frame > NORMAL_ATTACK2_EFFECT_REQUEST) {
		//エフェクト発生判定オン
		m_IsEffect = true;
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

	if (!m_IsAction[BREAK_NORMAL_ATTACK3]) {
		//通常攻撃３段目アクション中に変更
		m_IsAction[BREAK_NORMAL_ATTACK3] = true;
		//攻撃力設定
		m_Power = NORAML_ATTACK3_POWER;
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK3_CHANGE_MATERIAL_START && m_AnimeData.Frame < NORMAL_ATTACK3_CHANGE_MATERIAL_END) {
		//輪郭線のマテリアルをマテリアル赤に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::MATERIAL_RED, FALSE);
	}
	else {
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::MATERIAL_BLACK, FALSE);
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK3_COLLISION_START && m_AnimeData.Frame < NORMAL_ATTACK3_COLLISION_END) {
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_STRONGATK)) {
			SoundManager::Play(SoundManager::TagID::SE_STRONGATK);
		}
		//ボーンに攻撃判定を生成
		SetFrameDataIsAttackFlg(PALMEND_LEFT, NORMAL_ATTACK3_COLLISION_RAD);
		SetFrameDataIsAttackFlg(PALMEND_RIGHT, NORMAL_ATTACK3_COLLISION_RAD);
	}
	else {
		//ボーン攻撃判定を削除する
		DeleteFrameDataIsHitFlg(PALMEND_LEFT);
		DeleteFrameDataIsHitFlg(PALMEND_RIGHT);
	}
	if (!m_IsEffect && m_AnimeData.Frame > NORMAL_ATTACK3_EFFECT_REQUEST) {
		//エフェクト発生判定オン
		m_IsEffect = true;
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
	}
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = WAIT;
		//通常攻撃３段目アクション終了
		m_IsAction[BREAK_NORMAL_ATTACK3] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
	}
}
//後方攻撃
void Boss::RearAttack() {
	//後方攻撃アニメーション再生
	RequestEndLoop(ANIME_REAR_ATTACK, ANIME_SPEED);

	if (!m_IsAction[REAR_ATTACK]) {
		//後方攻撃アクション中に変更
		m_IsAction[REAR_ATTACK] = true;
		//攻撃力設定
		m_Power = REAR_ATTACK_POWER;
	}
	if (m_AnimeData.Frame > REAR_ATTACK_CHANGE_MATERIAL_START && m_AnimeData.Frame < REAR_ATTACK_CHANGE_MATERIAL_END) {
		//輪郭線のマテリアルをマテリアル赤に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::MATERIAL_RED, FALSE);
	}
	else {
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::MATERIAL_BLACK, FALSE);
	}
	if (m_AnimeData.Frame > REAR_ATTACK_COLLISION_START && m_AnimeData.Frame < REAR_ATTACK_COLLISION_END) {
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_STRONGATK)) {
			SoundManager::Play(SoundManager::TagID::SE_STRONGATK);
		}
		//ボーンに攻撃判定を生成
		SetFrameDataIsAttackFlg(TOSEEND_LEFT, REAR_ATTACK_COLLISION_RAD);
		SetFrameDataIsAttackFlg(TOSEEND_RIGHT, REAR_ATTACK_COLLISION_RAD);
		//正規化された方向ベクトルを取得
		VECTOR DirToPlayer = GetDirectionNotY(m_Pos, m_PlayerPos, TRUE);
		//1フレームで移動する距離を生成
		DirToPlayer = VScale(DirToPlayer, REAR_ATTACK_MULT);
		//座標に加算
		m_Pos = VAdd(m_Pos, DirToPlayer);
		//角度を計算
		float RotY = atan2f(DirToPlayer.x, DirToPlayer.z);
		//Y軸回転値に代入
		m_Rot.y = RotY;
	}
	else {
		//ボーン攻撃判定を削除する
		DeleteFrameDataIsHitFlg(TOSEEND_LEFT);
		DeleteFrameDataIsHitFlg(TOSEEND_RIGHT);
	}
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = WAIT;
		//後方攻撃アクション終了
		m_IsAction[REAR_ATTACK] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
	}

}
//突進直前移動
void Boss::Jump() {
	//待機アニメーションループ再生
	RequestLoop(ANIME_WAIT, ANIME_SPEED);

	if (!m_IsAction[JUMP]) {
		//ジャンプアクション中に変更
		m_IsAction[JUMP] = true;
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
		//ジャンプアクション終了
		m_IsAction[JUMP] = false;
		//Y軸を地面に設置
		m_Pos.y = 0;
	}
}
//突進チャージ
void Boss::ChargeAttackStart() {
	//突進チャージアニメーション再生
	RequestEndLoop(ANIME_CHARGE_ATTACK_START, ANIME_SPEED);
	//正規化された方向ベクトルを取得
	VECTOR DirToPlayer = GetDirectionNotY(m_Pos, m_PlayerPos, TRUE);
	//角度を計算
	float RotY = atan2f(-DirToPlayer.x, -DirToPlayer.z);
	//Y軸回転値に代入
	m_Rot.y = RotY;

	if (!m_IsAction[CHARGE_ATTACK_START]) {
		//突進チャージアクション中に変更
		m_IsAction[CHARGE_ATTACK_START] = true;
	}
	if (!m_IsEffect) {
		//エフェクト発生判定オン
		m_IsEffect = true;
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
		//突進チャージアクション終了
		m_IsAction[CHARGE_ATTACK_START] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
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

	if (!m_IsAction[CHARGE]) {
		//突進アクション中に変更
		m_IsAction[CHARGE] = true;
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_STRONGATK)) {
			SoundManager::Play(SoundManager::TagID::SE_STRONGATK);
		}
		//エフェクト発生判定オン
		m_IsEffect = true;
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
		//角度を計算
		float RotY = atan2f(-DirToPlayer.x, -DirToPlayer.z);
		//Y軸回転値に代入
		m_Rot.y = RotY;
	}
	else {
		//突進振り上げ状態へ
		m_State = CHARGE_ATTACK;
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::MATERIAL_BLACK, FALSE);
		//エフェクト発生判定オフ
		m_IsEffect = false;
	}
	if (Len < CHARGE_CHANGE_MATERIAL_START_LEN) {
		//輪郭線のマテリアルをマテリアル赤に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::MATERIAL_RED, FALSE);
	}
}
//突進振り上げ
void Boss::ChargeAttack() {
	//突進振り上げアニメーション再生
	RequestEndLoop(ANIME_CHARGE_ATTACK, ANIME_SPEED);

	if (!m_IsAction[CHARGE_ATTACK]) {
		//突進振り上げアクション中に変更
		m_IsAction[CHARGE_ATTACK] = true;
		//攻撃力設定
		m_Power = NORAML_ATTACK2_POWER;
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_MEDIUMATK)) {
			SoundManager::Play(SoundManager::TagID::SE_MEDIUMATK);
		}
		//ボーンに攻撃判定を生成
		SetFrameDataIsAttackFlg(FANG003END_LEFT, NORMAL_ATTACK2_COLLISION_RAD);
		SetFrameDataIsAttackFlg(FANG003END_RIGHT, NORMAL_ATTACK2_COLLISION_RAD);
	}
	if (!m_IsEffect && m_AnimeData.Frame) {
		//エフェクト発生判定オン
		m_IsEffect = true;
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
		m_State = WAIT;
		//突進振り上げアクション終了
		m_IsAction[BREAK_NORMAL_ATTACK2] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
		//ボーン攻撃判定を削除する
		DeleteFrameDataIsHitFlg(FANG003END_LEFT);
		DeleteFrameDataIsHitFlg(FANG003END_RIGHT);
	}
}
//必殺開始
void Boss::SpecialStart() {
	//必殺開始アニメーション再生
	RequestEndLoop(ANIME_SPECIAL_START, ANIME_SPEED);

	if (!m_IsAction[SPECIAL_START]) {
		//必殺開始アクション中に変更
		m_IsAction[SPECIAL_START] = true;
	}
	//方向ベクトルを取得
	VECTOR DirToZero = GetDirectionNotY(m_Pos, VZERO);
	//サイズ取得
	float Len = VSize(DirToZero);

	if (m_AnimeData.EndFlg && Len < SPECIAL_START_END_LEN) {
		//必殺チャージ状態へ
		m_State = SPECIAL_CHARGE;
		//必殺開始アクション終了
		m_IsAction[SPECIAL_START] = false;
	}
	else if (Len < SPECIAL_START_END_LEN) {
		//座標を固定
		m_Pos = VZERO;
	}
	else {
		//正規化したものを取得
		DirToZero = VNorm(DirToZero);
		//1フレームで移動する距離を生成
		DirToZero = VScale(DirToZero, CHARGE_MULT);
		//座標に加算
		m_Pos = VAdd(m_Pos, DirToZero);
		//角度を計算
		float RotY = atan2f(-DirToZero.x, -DirToZero.z);
		//Y軸回転値に代入
		m_Rot.y = RotY;
	}
}
//必殺チャージ
void Boss::SpecialCharge() {
	//必殺チャージアニメーションループ再生
	RequestLoop(ANIME_SPECIAL_CHAGE, ANIME_SPEED);

	if (!m_IsAction[SPECIAL_CHARGE]) {
		//必殺チャージアクション中に変更
		m_IsAction[SPECIAL_CHARGE] = true;
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
		//必殺チャージアクション終了
		m_IsAction[SPECIAL_START] = false;
		//初期化
		m_SpecialChargeTime = 0;
	}
}
//必殺
void Boss::Special() {
	if (!m_IsAction[SPECIAL]) {
		//必殺チャージアクション中に変更
		m_IsAction[SPECIAL] = true;
		//座標を変更
		m_Pos = SPECIAL_INIT_VECTOR;
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_ELEPHANTCRY)) {
			SoundManager::Play(SoundManager::TagID::SE_ELEPHANTCRY);
		}
	}
	if (m_Pos.y<=0.0f) {
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK02BLOW2, m_Pos, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_SPECIALMOVE)) {
			SoundManager::Play(SoundManager::TagID::SE_SPECIALMOVE);
		}
		//初期化
		m_Pos.y = 0.0;
		//待機状態へ
		m_State = WAIT;
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

	if (ToPlayerLen > ACTION_WAIT_DISTANCE) {
		//待機状態へ
		m_State = WAIT;
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
	//パターン内の攻撃順を1進める
	m_AttackIndex++;
	//パターン内の攻撃順の最大格納量より多いなら
	if (m_AttackIndex >= ATTACK_INDEX) {
		//パターン内の攻撃順を0に
		m_AttackIndex = 0;
		//攻撃パターンの種類の最大格納量より多いなら
		if (m_PatternIndex >= PATTERN_INDEX) {
			//攻撃パターンを0に
			m_PatternIndex = 0;
		}
		else {
			//攻撃パターンを1進める
			m_PatternIndex++;
		}
	}
	//二つの配列番号に合致する位置の数値を格納
	int Attack = (int)m_AttackPatterns[m_PatternIndex][m_AttackIndex];
	//状態を変化
	m_State = (TagState)Attack;

	//次の攻撃は何の予定か調べる
	int NextAttackIndex = m_AttackIndex + 1;
	//最大格納量を超過していたら無視
	if (NextAttackIndex >= ATTACK_INDEX) {
		m_NextAttack = 0;
	}
	else {
		//二つの配列番号に合致する位置の数値を格納
		int NextAttack = (int)m_AttackPatterns[m_PatternIndex][NextAttackIndex];
		//次の攻撃を保存
		m_NextAttack = (TagState)NextAttack;
	}
}
//ジャンプ時の着地地点管理
void Boss::JumpPosManger() {

}
//状態遷移
void Boss::StateManager() {
	switch (m_State) {
	case WAIT:						//待機
		Wait();
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
	case SPECIAL:
		Special();					//必殺
		break;
	}
}
//当たり判定設定
void Boss::SetFrameData(int _FrameNamber, float _Rad) {
	m_FrameData[_FrameNamber].Pos = GetFramePos(m_Hndl, _FrameNamber);
	m_FrameData[_FrameNamber].Rad = _Rad;
	m_FrameData[_FrameNamber].IsHitFlg = true;
}
//指定のボーン当たり判定を削除する
void Boss::DeleteFrameDataIsHitFlgFalse(int _FrameNamber) {
	m_FrameData[_FrameNamber].Pos = VZERO;
	m_FrameData[_FrameNamber].Rad = FZERO;
	m_FrameData[_FrameNamber].IsHitFlg = false;
}
//全てのボーン当たり判定を削除する
void Boss::AllDeleteFrameDataIsHitFlgFalse() {
	for (int Index = 0; Index < FARAM_NUM; Index++) {
		if (m_FrameData[Index].IsHitFlg) {
			//指定のボーン攻撃判定を削除する
			DeleteFrameDataIsHitFlgFalse(Index);
		}
	}
}
//指定のボーン攻撃判定を設定
void Boss::SetFrameDataIsAttackFlg(int _FrameNamber, float _Rad) {
	m_FrameData[_FrameNamber].Pos = GetFramePos(m_Hndl, _FrameNamber);
	m_FrameData[_FrameNamber].Rad = _Rad;
	m_FrameData[_FrameNamber].IsAttackFlg = true;
}
//指定のボーン攻撃判定を削除する
void Boss::DeleteFrameDataIsHitFlg(int _FrameNamber) {
	m_FrameData[_FrameNamber].Pos = VZERO;
	m_FrameData[_FrameNamber].Rad = FZERO;
	m_FrameData[_FrameNamber].IsAttackFlg = false;
}
//全てのボーン攻撃判定を削除する
void Boss::AllDeleteFrameDataIsHitFlg() {
	for (int Index = 0; Index < FARAM_NUM; Index++) {
		if (m_FrameData[Index].IsAttackFlg) {
			//指定のボーン攻撃判定を削除する
			DeleteFrameDataIsHitFlg(Index);
		}
	}
}
