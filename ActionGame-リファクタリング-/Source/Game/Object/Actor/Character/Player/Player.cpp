#include "Player.h"

namespace {
	const float		RAD = 10.0f;															//半径
	const VECTOR	PLAYER_SIZE = { RAD,RAD,RAD };											//ボックス当たり判定

	const float		HIT_POINTS = 100.0f;													//体力
	const float		STAMINA = 75.0f;														//スタミナ
	const int		SKILL_POINTS = 5;														//スキルポイント
	const float		MAX_HITPOINTS = 100.0f;													//最大体力
	const float		MAX_STAMINA = 100.0f;													//最大スタミナ
	const int		MAX_SKILL_POINTS = 10;													//最大スキルポイント

	const float		ADD_STAMINA = 0.01f;													//プレイヤースタミナ回復量(１フレーム毎)

	const float		WALK_MOVE_MULT = 1.25f;													//歩き時の移動乗算値
	const float		ROLLING_MOVE_MULT = 3.0f;												//ローリング時の移動乗算値
	const float		JUMP_MOVE_MULT = 2.0f;													//ジャンプ時の移動乗算値
	const float		GUARD_MOVE_MULT = WALK_MOVE_MULT / 5.0f;								//ガード時の移動乗算値(歩き/指定値)
	const float		SKILL_ATTACK_MOVE_MULT = 20.0f;											//スキル攻撃時の移動乗算値

	const float		ROLLING_SUB_STAMINA = 10.0f;											//ローリング時のスタミナ減算値

	const float		GUARD_MIN_STAMINA = 10.0f;												//ガード状態維持に必要な最低限のスタミナ量

	const float		SKILL_ATTACK_COLLISION_START = 15.0f;									//スキル攻撃の当たり判定開始フレーム
	const float		SKILL_ATTACK_COLLISION_END = 35.0f;										//スキル攻撃の当たり判定終了フレーム
	const float		NORMAL_ATTACK1_COLLISION_START = 10.0f;									//通常攻撃１段目の当たり判定開始フレーム
	const float		NORMAL_ATTACK1_COLLISION_END = 15.0f;									//通常攻撃１段目の当たり判定終了フレーム
	const float		NORMAL_ATTACK1_TRANSITION = 17.0f;										//通常攻撃１段目から通常攻撃２段目に移行するフレーム
	const float		NORMAL_ATTACK2_COLLISION_START = 10.0f;									//通常攻撃２段目の当たり判定開始フレーム
	const float		NORMAL_ATTACK2_COLLISION_END = 15.0f;									//通常攻撃２段目の当たり判定終了フレーム
	const float		NORMAL_ATTACK2_TRANSITION = 17.0f;										//通常攻撃２段目から通常攻撃３段目に移行するフレーム
	const float		NORMAL_ATTACK3_COLLISION_START = 10.0f;									//通常攻撃３段目の当たり判定開始フレーム
	const float		NORMAL_ATTACK3_COLLISION_END = 15.0f;									//通常攻撃３段目の当たり判定終了フレーム
	const float		NORMAL_ATTACK3_TRANSITION = 17.0f;										//通常攻撃３段目から通常攻撃１段目に移行するフレーム

	const float		SKILL_ATTACKPOWER = 100.0f;												//スキル攻撃時の攻撃力
	const float		NORMAL_ATTACK1_POWER = 10.0f;											//通常攻撃１段目時の攻撃力
	const float		NORAML_ATTACK2_POWER = 15.0f;											//通常攻撃２段目時の攻撃力
	const float		NORAML_ATTACK3_POWER = 20.0f;											//通常攻撃３段目時の攻撃力

	const int		ROLLING_TIME = 20;														//ローリング持続時間
	const float		ROLLING_ONEFRAM = 180.0f * DX_PI_F / 2600.0f;							//１フレーム中に回転するX軸の値

	const float		FIRST_JUMP_POWER = 5.0f;												//初回ジャンプ力

	const char		FILE_PATH[] = ("Data/Model/Player/MainBody/MainBody.mv1");				//モデルファイルパス
}

//コンストラクタ
Player::Player() {
	Init();
}
//デストラクタ
Player::~Player() {
	Exit();
}
//初期化処理
void Player::Init() {
	CharacterBase::Init();

	m_IsPush = true;							//押し出し判定を行う

	m_Rad = RAD;								//半径
	m_Size = PLAYER_SIZE;						//ボックス当たり判定

	m_HitPoints = HIT_POINTS;					//体力
	m_Stamina = STAMINA;						//スタミナ
	m_SkillPoints = SKILL_POINTS;				//スキルポイント

	m_State = WAIT;								//プレイヤー状態変数
	m_AttackTargetPos = VZERO;					//攻撃サーチを行う物体の座標
	m_RollingTime = 0;							//ローリング継続時間
	for (int Index = 0; Index < STATE_NUM; Index++) {
		m_IsAction[Index] = false;				//アクションフラグ
		m_IsActionSuccess[Index] = false;		//アクション成功判定フラグ
	}
	for (int Index = 0; Index < NORMAL_ATTACK_MAX; Index++) {
		m_IsNextNormalAttack[Index] = false;	//通常攻撃の次の段数に移行するか
	}
	m_IsAttackCollision = false;				//攻撃の当たり判定を発生させてよいか
	m_JumpCalc = 0.0f;							//ジャンプ力計算
}
//データ読み込み処理
void Player::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void Player::Step() {
	if (m_HitPoints <= 0) {
		m_HitPoints = 0;
		m_State = DEATH;
		return;
	}

#ifdef _DEBUG
	m_HitPoints = 100;
#endif // DEBUG

	//スタミナ処理
	StaminaManager();
	//状態遷移
	StateManager();
	//重力処理
	GravityManager();
}

//待機
void Player::Wait() {
	//待機アニメーションループ再生
	RequestLoop(WAIT);
	//スタミナを回復する
	m_IsStaminaRecover = true;
	//通常移動方向設定
	if (NormalMoveVec()) {
		//歩き状態へ
		m_State = WALK;
	}
	//動作管理
	ActionManager();
}
//ダメージ
void Player::Damage() {
	//ダメージアニメーション再生
	RequestEndLoop(DAMAGE);
	//スタミナを回復しない
	m_IsStaminaRecover = false;
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = WAIT;
	}
}
//死亡
void Player::Death() {
	//死亡アニメーション再生
	RequestEndLoop(DEATH);
	//スタミナを回復しない
	m_IsStaminaRecover = false;
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//生存フラグをオフ
		m_IsActive = false;
	}
}
//歩き
void Player::Walk() {
	//歩きアニメーションループ再生
	RequestLoop(WALK);
	//スタミナを回復する
	m_IsStaminaRecover = true;
	//通常移動方向設定
	if (NormalMoveVec()) {
		//移動計算
		MoveCalc();
	}
	else {
		//待機状態へ
		m_State = WAIT;
	}
	//動作管理
	ActionManager();
}
//ローリング
void Player::Rolling() {
	//ローリングアニメーションループ再生
	RequestLoop(ROLLING);
	//通常移動方向設定
	if (NormalMoveVec()) {
		//移動計算
		MoveCalc();
	}
	//スタミナを回復しない
	m_IsStaminaRecover = false;

	if (!m_IsAction[ROLLING]) {
		//ローリングアクション中に変更
		m_IsAction[ROLLING] = true;
		//スタミナを減らす
		m_Stamina -= ROLLING_SUB_STAMINA;
	}

	if (m_RollingTime > ROLLING_TIME) {
		m_RollingTime = 0;
		//待機状態へ
		m_State = WAIT;
		//X軸回転値を0に
		m_Rot.x = 0.0f;
		//ローリングアクション終了
		m_IsAction[ROLLING] = false;
	}
	else {
		//X軸回転値を計算
		m_Rot.x -= ROLLING_ONEFRAM;
		//ローリング継続時間を加算
		m_RollingTime++;
	}
}
//ジャンプ
void Player::Jump() {
	//ジャンプアニメーションループ再生
	RequestLoop(JUMP);

	if (!m_IsAction[JUMP]) {
		//ジャンプアクション中に変更
		m_IsAction[JUMP] = true;
		//重力処理オン
		m_IsGravity = true;
		//初回ジャンプ力設定
		m_JumpCalc = FIRST_JUMP_POWER;
	}
	//通常移動方向設定
	if (NormalMoveVec()) {
		//移動計算
		MoveCalc();
	}
	//重力処理がオフになったら
	if (!m_IsGravity) {
		//待機状態へ
		m_State = WAIT;
		//ジャンプアクション終了
		m_IsAction[JUMP] = false;
	}
}
//ガード
void Player::Guard() {
	//ガードアニメーションループ再生
	RequestLoop(GUARD);
	//通常移動方向設定
	if (NormalMoveVec()) {
		//移動計算
		MoveCalc();
	}
	//スタミナを回復しない
	m_IsStaminaRecover = false;

	if (!m_IsAction[GUARD]) {
		//ガードアクション中に変更
		m_IsAction[GUARD] = true;
		//当たり判定オフ
		m_IsCollision = false;
	}
	//ガードボタンを離したら
	if (!InputPad::IsPushPadRep(XINPUT_BUTTON_RIGHT_SHOULDER) && !InputKey::IsPushKeyRep(KEY_INPUT_F)) {
		//待機状態へ
		m_State = WAIT;
		//当たり判定オン
		m_IsCollision = true;
		//ガードアクション終了
		m_IsAction[ROLLING] = false;
	}
	//スタミナが一定値を下回れば
	if (m_Stamina <= GUARD_MIN_STAMINA) {
		//待機状態へ
		m_State = WAIT;
		//当たり判定オン
		m_IsCollision = true;
		//ガードアクション終了
		m_IsAction[ROLLING] = false;
	}
}
//パリィ
void Player::Parry() {
	//パリィアニメーション再生
	RequestEndLoop(PARRY);
	//スタミナを回復しない
	m_IsStaminaRecover = false;

	if (!m_IsAction[PARRY]) {
		//パリィアクション中に変更
		m_IsAction[PARRY] = true;
		//当たり判定オフ
		m_IsCollision = false;
	}

	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = WAIT;
		//当たり判定オン
		m_IsCollision = true;
		//パリィアクション終了
		m_IsAction[PARRY] = false;
	}
}
//スキル攻撃
void Player::SkillAttack() {
	//スキル攻撃アニメーション再生
	RequestEndLoop(SKILL_ATTACK);

	if (!m_IsAction[SKILL_ATTACK]) {
		//通常攻撃１段目アクション中に変更
		m_IsAction[SKILL_ATTACK] = true;
		//当たり判定オフ
		m_IsCollision = false;
		//攻撃力設定
		m_Power = SKILL_ATTACKPOWER;
		//スキルゲージ減少
		m_SkillPoints += -1;
	}
	//指定フレームから指定フレームまでの間
	if (m_AnimeData.Frame > SKILL_ATTACK_COLLISION_START && m_AnimeData.Frame < SKILL_ATTACK_COLLISION_END) {
		if (!m_IsAttackCollision) {
			//攻撃の当たり判定発生
			m_IsAttackCollision = true;
			//移動方向設定
			m_MoveVec.z = -1.0f;
		}
		//移動計算
		MoveCalc();
		if (!m_IsEffect) {
			//エフェクト発生判定オン
			m_IsEffect = true;
			//指定ボーンの座標取得
			VECTOR Pos = GetFramePos(m_Hndl, UPPERARM_RIGHT);
			//エフェクトリクエスト
			m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK01SWORD21, Pos, false);
			//エフェクトの回転角度を設定
			MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
		}
		//指定ボーンの座標取得
		VECTOR Pos = GetFramePos(m_Hndl, UPPERARM_RIGHT);
		//エフェクトの座標設定
		MyEffeckseer::SetPosition(m_EffectHndl, Pos);
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_FLASHLIGHT)) {
			SoundManager::Play(SoundManager::TagID::SE_FLASHLIGHT);
		}
	}
	else if (m_IsAttackCollision) {
		//攻撃の当たり判定消滅
		m_IsAttackCollision = false;
	}
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = WAIT;
		//当たり判定オン
		m_IsCollision = true;
		//スキル攻撃アクション終了
		m_IsAction[SKILL_ATTACK] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
	}
}
//通常攻撃１段目
void Player::NormalAttack1() {
	//通常攻撃１段目アニメーション再生
	RequestEndLoop(NORMAL_ATTACK1);

	if (!m_IsAction[NORMAL_ATTACK1]) {
		//通常攻撃１段目アクション中に変更
		m_IsAction[NORMAL_ATTACK1] = true;
		//攻撃力設定
		m_Power = NORMAL_ATTACK1_POWER;
		//サウンドリクエスト
		SoundManager::Play(SoundManager::TagID::SE_ATK);
	}
	//指定フレームから指定フレームまでの間
	if (m_AnimeData.Frame > NORMAL_ATTACK1_COLLISION_START && m_AnimeData.Frame < NORMAL_ATTACK1_COLLISION_END) {
		if (!m_IsAttackCollision) {
			//攻撃の当たり判定発生
			m_IsAttackCollision = true;
		}
	}
	else if (m_IsAttackCollision) {
		//攻撃の当たり判定消滅
		m_IsAttackCollision = false;
	}
	if (!m_IsEffect) {
		//エフェクト発生判定オン
		m_IsEffect = true;
		//指定ボーンの座標取得
		VECTOR Pos = GetFramePos(m_Hndl, UPPERARM_RIGHT);
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK01SWORD1_1, Pos, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
	}
	//通常攻撃ボタンが押されたら
	if (InputPad::IsPushPadTrg(XINPUT_BUTTON_B) || InputKey::IsPushKeyTrg(KEY_INPUT_SPACE)) {
		m_IsNextNormalAttack[NORMAL_ATTACK2_NUMBER] = true;
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK1_TRANSITION && m_IsNextNormalAttack[NORMAL_ATTACK2_NUMBER]) {
		//通常攻撃２段目へ
		m_State = NORMAL_ATTACK2;
		//通常攻撃１段目アクション終了
		m_IsAction[NORMAL_ATTACK1] = false;
		//通常攻撃の段数判定をオフ
		m_IsNextNormalAttack[NORMAL_ATTACK1_NUMBER] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
	}
	//アニメーションが終わったら
	else if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = WAIT;
		//通常攻撃１段目アクション終了
		m_IsAction[NORMAL_ATTACK1] = false;
		//通常攻撃の段数判定をオフ
		m_IsNextNormalAttack[NORMAL_ATTACK1_NUMBER] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
	}
}
//通常攻撃２段目
void Player::NormalAttack2() {
	//通常攻撃２段目アニメーション再生
	RequestEndLoop(NORMAL_ATTACK2);

	if (!m_IsAction[NORMAL_ATTACK2]) {
		//通常攻撃２段目アクション中に変更
		m_IsAction[NORMAL_ATTACK2] = true;
		//攻撃力設定
		m_Power = NORAML_ATTACK2_POWER;
		//サウンドリクエスト
		SoundManager::Play(SoundManager::TagID::SE_ATK);
	}
	//指定フレームから指定フレームまでの間
	if (m_AnimeData.Frame > NORMAL_ATTACK2_COLLISION_START && m_AnimeData.Frame < NORMAL_ATTACK2_COLLISION_END) {
		if (!m_IsAttackCollision) {
			//攻撃の当たり判定発生
			m_IsAttackCollision = true;
		}
	}
	else if (m_IsAttackCollision) {
		//攻撃の当たり判定消滅
		m_IsAttackCollision = false;
	}
	if (!m_IsEffect) {
		//エフェクト発生判定オン
		m_IsEffect = true;
		//指定ボーンの座標取得
		VECTOR Pos = GetFramePos(m_Hndl, UPPERARM_RIGHT);
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK01SWORD1_2, Pos, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
	}
	//通常攻撃ボタンが押されたら
	if (InputPad::IsPushPadTrg(XINPUT_BUTTON_B) || InputKey::IsPushKeyTrg(KEY_INPUT_SPACE)) {
		m_IsNextNormalAttack[NORMAL_ATTACK3_NUMBER] = true;
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK2_TRANSITION && m_IsNextNormalAttack[NORMAL_ATTACK3_NUMBER]) {
		//待機状態へ
		m_State = NORMAL_ATTACK3;
		//通常攻撃２段目アクション終了
		m_IsAction[NORMAL_ATTACK2] = false;
		//通常攻撃の段数判定をオフ
		m_IsNextNormalAttack[NORMAL_ATTACK2_NUMBER] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
	}
	//アニメーションが終わったら
	else if (m_AnimeData.EndFlg) {
		//通常攻撃３段目へ
		m_State = WAIT;
		//通常攻撃２段目アクション終了
		m_IsAction[NORMAL_ATTACK2] = false;
		//通常攻撃の段数判定をオフ
		m_IsNextNormalAttack[NORMAL_ATTACK2_NUMBER] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
	}
}
//通常攻撃３段目
void Player::NormalAttack3() {
	//通常攻撃３段目アニメーション再生
	RequestEndLoop(NORMAL_ATTACK3);

	if (!m_IsAction[NORMAL_ATTACK3]) {
		//通常攻撃３段目アクション中に変更
		m_IsAction[NORMAL_ATTACK3] = true;
		//攻撃力設定
		m_Power = NORAML_ATTACK3_POWER;
		//サウンドリクエスト
		SoundManager::Play(SoundManager::TagID::SE_ATK);
	}
	//指定フレームから指定フレームまでの間
	if (m_AnimeData.Frame > NORMAL_ATTACK3_COLLISION_START && m_AnimeData.Frame < NORMAL_ATTACK3_COLLISION_END) {
		if (!m_IsAttackCollision) {
			//攻撃の当たり判定発生
			m_IsAttackCollision = true;
		}
	}
	else if (m_IsAttackCollision) {
		//攻撃の当たり判定消滅
		m_IsAttackCollision = false;
	}
	if (!m_IsEffect) {
		//エフェクト発生判定オン
		m_IsEffect = true;
		//指定ボーンの座標取得
		VECTOR Pos = GetFramePos(m_Hndl, UPPERARM_RIGHT);
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK01SWORD1_1, Pos, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
	}
	//通常攻撃ボタンが押されたら
	if (InputPad::IsPushPadTrg(XINPUT_BUTTON_B) || InputKey::IsPushKeyTrg(KEY_INPUT_SPACE)) {
		m_IsNextNormalAttack[NORMAL_ATTACK1_NUMBER] = true;
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK2_TRANSITION && m_IsNextNormalAttack[NORMAL_ATTACK1_NUMBER]) {
		//通常攻撃１段目へ
		m_State = NORMAL_ATTACK1;
		//通常攻撃３段目アクション終了
		m_IsAction[NORMAL_ATTACK3] = false;
		//通常攻撃の段数判定をオフ
		m_IsNextNormalAttack[NORMAL_ATTACK3_NUMBER] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
	}
	//アニメーションが終わったら
	else if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = WAIT;
		//通常攻撃３段目アクション終了
		m_IsAction[NORMAL_ATTACK3] = false;
		//通常攻撃の段数判定をオフ
		m_IsNextNormalAttack[NORMAL_ATTACK3_NUMBER] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
	}
}
//通常移動方向設定
bool Player::NormalMoveVec() {
	bool IsMove = false;

	m_MoveVec = VZERO;
	//奥方向
	if (InputKey::IsPushKeyRep(KEY_INPUT_W)) {
		m_MoveVec.z = -1.0f;
		IsMove = true;
	}
	//手前方向
	if (InputKey::IsPushKeyRep(KEY_INPUT_S)) {
		m_MoveVec.z = 1.0f;
		IsMove = true;
	}
	//左方向
	if (InputKey::IsPushKeyRep(KEY_INPUT_A)) {
		m_MoveVec.x = 1.0f;
		IsMove = true;
	}
	//右方向
	if (InputKey::IsPushKeyRep(KEY_INPUT_D)) {
		m_MoveVec.x = -1.0f;
		IsMove = true;
	}

	if (InputPad::GetLAnalogYInput() > 0) {
		m_MoveVec.z = -InputPad::GetLAnalogYInput();
		IsMove = true;
	}

	if (InputPad::GetLAnalogYInput() < 0) {
		m_MoveVec.z = -InputPad::GetLAnalogYInput();
		IsMove = true;
	}

	if (InputPad::GetLAnalogXInput() < 0) {
		m_MoveVec.x = -InputPad::GetLAnalogXInput();
		IsMove = true;
	}

	if (InputPad::GetLAnalogXInput() > 0) {
		m_MoveVec.x = -InputPad::GetLAnalogXInput();
		IsMove = true;
	}

	return IsMove;
}
//移動計算
void Player::MoveCalc() {
	VECTOR MoveVec = m_MoveVec;
	//MoveVecを正規化
	MoveVec = VNorm(MoveVec);
	//MoveVecを行列化
	MATRIX MatMoveVec = MGetTranslate(MoveVec);
	//カメラのY軸回転値を行列化
	MATRIX MatRotY = MGetRotY(m_CamraRot.y);
	//行列合成
	MATRIX MatComposition = MMult(MatMoveVec, MatRotY);
	//行列の座標情報部分を抜き取り
	MoveVec = VGet(MatComposition.m[3][0], 0.0f, MatComposition.m[3][2]);

	switch (m_State) {
	case WALK:			//歩き
		MoveVec = VScale(MoveVec, WALK_MOVE_MULT);
		break;
	case ROLLING:		//ローリング
		MoveVec = VScale(MoveVec, ROLLING_MOVE_MULT);
		break;
	case GUARD:			//ガード
		MoveVec = VScale(MoveVec, GUARD_MOVE_MULT);
		break;
	case JUMP:			//ジャンプ
		MoveVec = VScale(MoveVec, JUMP_MOVE_MULT);
		break;
	case SKILL_ATTACK:	//スキル攻撃
		MoveVec = VScale(MoveVec, SKILL_ATTACK_MOVE_MULT);
		break;
	}
	m_Pos = VAdd(m_Pos, MoveVec);
	m_Rot.y = atan2f(-MoveVec.x, -MoveVec.z);
}
//スタミナ処理
void Player::StaminaManager() {
	if (m_Stamina >= MAX_STAMINA) {
		m_Stamina = MAX_STAMINA;
	}
	else if (m_IsStaminaRecover) {
		m_Stamina += ADD_STAMINA;
	}

	if (m_Stamina <= 0.0f) {
		m_Stamina = 0.0f;
	}
}
//状態遷移
void Player::StateManager() {
	switch (m_State) {
	case WAIT:				//待機
		Wait();
		break;
	case DAMAGE:			//ダメージ
		Damage();
		break;
	case DEATH:				//死亡
		Death();
		break;
	case WALK:				//歩き
		Walk();
		break;
	case ROLLING:			//ローリング
		Rolling();
		break;
	case JUMP:				//ジャンプ
		Jump();
		break;
	case GUARD:				//ガード
		Guard();
		break;
	case PARRY:				//パリィ
		Parry();
		break;
	case SKILL_ATTACK:		//スキル攻撃
		SkillAttack();
		break;
	case NORMAL_ATTACK1:	//通常攻撃１段目
		NormalAttack1();
		break;
	case NORMAL_ATTACK2:	//通常攻撃２段目
		NormalAttack2();
		break;
	case NORMAL_ATTACK3:	//通常攻撃３段目
		NormalAttack3();
		break;
	}
}
//動作管理
void Player::ActionManager() {
	//ローリング
	if (InputPad::IsPushPadTrg(XINPUT_BUTTON_X) || InputKey::IsPushKeyTrg(KEY_INPUT_R)) {
		m_State = ROLLING;
	}
	//ジャンプ
	if ((InputPad::IsPushPadTrg(XINPUT_BUTTON_A) || InputKey::IsPushKeyTrg(KEY_INPUT_Z))) {
		m_State = JUMP;
	}
	//スタミナが一定値を上回っていれば
	if (m_Stamina > GUARD_MIN_STAMINA) {
		//ガード
		if (InputPad::IsPushPadTrg(XINPUT_BUTTON_RIGHT_SHOULDER) || InputKey::IsPushKeyTrg(KEY_INPUT_F)) {
			m_State = GUARD;
		}
	}
	//スキル攻撃
	if (InputPad::IsPushPadTrg(XINPUT_BUTTON_Y) || InputKey::IsPushKeyTrg(KEY_INPUT_Q)) {
		m_State = SKILL_ATTACK;
	}
	//通常攻撃１段目
	if (InputPad::IsPushPadTrg(XINPUT_BUTTON_B) || InputKey::IsPushKeyTrg(KEY_INPUT_SPACE)) {
		m_State = NORMAL_ATTACK1;
		m_IsNextNormalAttack[NORMAL_ATTACK1_NUMBER] = true;
	}
}
//重力処理
void Player::GravityManager() {
	if (m_IsGravity) {
		//現在のY座標にジャンプ力を加算
		m_Pos.y += m_JumpCalc;
		//重力処理
		ObjectBase::GravityManager();
		//ジャンプ力減衰
		m_JumpCalc += m_Gravity;
	}
	else {
		//ジャンプ力リセット
		m_JumpCalc = 0.0f;
	}
}

