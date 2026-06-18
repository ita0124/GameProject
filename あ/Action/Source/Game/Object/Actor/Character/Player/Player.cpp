#include "Player.h"
#include "Game/Object/Actor/Character/Enemy/Boss/Boss.h"

namespace {
	constexpr float		RAD = 5.0f;																//半径
	constexpr VECTOR	PLAYER_SIZE = { RAD,20.0f,RAD };										//ボックス当たり判定

	constexpr VECTOR	RESPAWN_POS = { 0.0f,50.0f,0.0f };										//落下後のリスポーン座標

	constexpr float		HIT_POINTS = 100.0f;													//体力
	constexpr float		STAMINA = 75.0f;														//スタミナ
	constexpr int		SKILL_POINTS = 50;														//スキルポイント
	constexpr float		MAX_HITPOINTS = 100.0f;													//最大体力
	constexpr float		MAX_STAMINA = 100.0f;													//最大スタミナ
	constexpr int		MAX_SKILL_POINTS = 100;													//最大スキルポイント

	constexpr float		ADD_STAMINA = 0.01f;													//プレイヤースタミナ回復量(１フレーム毎)

	constexpr float		WALK_MOVE_MULT = 1.25f;													//歩き時の移動乗算値
	constexpr float		ROLLING_MOVE_MULT = 3.0f;												//ローリング時の移動乗算値
	constexpr float		JUMP_MOVE_MULT = 2.0f;													//ジャンプ時の移動乗算値
	constexpr float		GUARD_MOVE_MULT = WALK_MOVE_MULT / 5.0f;								//ガード時の移動乗算値(歩き/指定値)
	constexpr float		SKILL_ATTACK_MOVE_MULT = 20.0f;											//スキル攻撃時の移動乗算値
	constexpr float		PLAYER_NORMAL_ATTACK_MOVE_MULT = 5.0f;									//通常攻撃時の移動乗算値

	constexpr float		ROLLING_SUB_STAMINA = 10.0f;											//ローリング時のスタミナ減算値

	constexpr float		GUARD_MIN_STAMINA = 10.0f;												//ガード状態維持に必要な最低限のスタミナ量
	constexpr float		ROLLING_MIN_STAMINA = 10.0f;											//ローリング発動に必要な最低限のスタミナ量

	constexpr float		SKILL_ATTACK_COLLISION_START = 15.0f;									//スキル攻撃の当たり判定開始フレーム
	constexpr float		SKILL_ATTACK_COLLISION_END = 35.0f;										//スキル攻撃の当たり判定終了フレーム
	constexpr float		NORMAL_ATTACK1_COLLISION_START = 10.0f;									//通常攻撃１段目の当たり判定開始フレーム
	constexpr float		NORMAL_ATTACK1_COLLISION_END = 15.0f;									//通常攻撃１段目の当たり判定終了フレーム
	constexpr float		NORMAL_ATTACK1_TRANSITION = 17.0f;										//通常攻撃１段目から通常攻撃２段目に移行するフレーム
	constexpr float		NORMAL_ATTACK2_COLLISION_START = 10.0f;									//通常攻撃２段目の当たり判定開始フレーム
	constexpr float		NORMAL_ATTACK2_COLLISION_END = 15.0f;									//通常攻撃２段目の当たり判定終了フレーム
	constexpr float		NORMAL_ATTACK2_TRANSITION = 17.0f;										//通常攻撃２段目から通常攻撃３段目に移行するフレーム
	constexpr float		NORMAL_ATTACK3_COLLISION_START = 10.0f;									//通常攻撃３段目の当たり判定開始フレーム
	constexpr float		NORMAL_ATTACK3_COLLISION_END = 15.0f;									//通常攻撃３段目の当たり判定終了フレーム
	constexpr float		NORMAL_ATTACK3_TRANSITION = 17.0f;										//通常攻撃３段目から通常攻撃１段目に移行するフレーム

	constexpr float		SKILL_ATTACKPOWER = 100.0f;												//スキル攻撃時の攻撃力
	constexpr float		NORMAL_ATTACK1_POWER = 10.0f;											//通常攻撃１段目時の攻撃力
	constexpr float		NORAML_ATTACK2_POWER = 15.0f;											//通常攻撃２段目時の攻撃力
	constexpr float		NORAML_ATTACK3_POWER = 20.0f;											//通常攻撃３段目時の攻撃力

	constexpr float		GUARD_SUCCESS_TIME = 30;												//ガードアクション成功の継続時間

	constexpr float		PARRY_WINDOW_TIME = 10;													//ガードアクション実行後のパリィに移行できる許容時間

	constexpr int		ROLLING_TIME = 20;														//ローリング持続時間
	constexpr float		ROLLING_ONEFRAM = 180.0f * DX_PI_F / 2600.0f;							//１フレーム中に回転するX軸の値

	constexpr float		FIRST_JUMP_POWER = 5.5f;												//初回ジャンプ力

	constexpr float		GUARD_DAMAGE_TAKEN_MULT = 0.2f;											//ガード時の被ダメ軽減量

	constexpr char		FILE_PATH[] = ("Data/Model/Player/MainBody/MainBody.mv1");				//モデルファイルパス
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

	m_Kinds = PLAYER;

	m_IsPush = true;							//押し出し判定を行う

	m_Rad = RAD;								//半径
	m_Size = PLAYER_SIZE;						//ボックス当たり判定

	m_HitPoints = HIT_POINTS;					//体力
	m_Stamina = STAMINA;						//スタミナ
	m_SkillPoints = SKILL_POINTS;				//スキルポイント

	m_State = WAIT;								//プレイヤー状態変数
	m_RollingTime = 0;							//ローリング継続時間
	for (int Index = 0; Index < STATE_NUM; Index++) {
		m_IsAction[Index] = false;				//アクションフラグ
		m_IsActionSuccess[Index] = false;		//アクション成功判定フラグ
		m_ActionSuccessTime[Index] = 0;			//アクション成功の継続時間
	}
	m_IsGuardCollision = false;					//ガードの当たり判定を発生させてよいか
	m_ParryWindoeTime = 0;						//ガードアクション実行後のパリィに移行できる許容時間
	m_IsParryWindo = true;						//パリィ許容フラグ
	m_JumpCalc = 0.0f;							//ジャンプ力計算

	m_KnockBackStartPos = VZERO;				//ノックバック開始時の敵座標
	m_KnockBackDistance = 0.0f;				//現在のノックバック量
	m_KnockBackMaxDistance = 0.0f;				//最大ノックバック量
	m_KnockBackSub = 0.0f;						//1フレーム毎のノックバック力減衰量
	m_KnockBackDuration = 0;					//ノックバック継続時間	
	m_IsKnockBackCalcStart = true;				//ノックバック計算を始めるフラグ
	m_IsKnockBack = false;						//ノックバック中フラグ

	m_AttackTargetPos = VZERO;					//攻撃サーチを行う物体の座標
	m_TargetAngle = 0.0f;						// 攻撃対象との角度差
	for (int Index = 0; Index < NORMAL_ATTACK_MAX; Index++) {
		m_IsNextNormalAttack[Index] = false;	//通常攻撃の次の段数に移行するか
		m_AttackMoveVec[Index] = VZERO;			//攻撃進行方向
	}
	m_IsAttackCollision = false;				//攻撃の当たり判定を発生させてよいか
}
//データ読み込み処理
void Player::Load() {
	ObjectBase::Load(FILE_PATH);
}
//毎フレーム呼び出す処理
void Player::Step() {
	m_PrevPos = m_Pos;

	if (m_PlatformVec.x != 0.0f || m_PlatformVec.y != 0.0f || m_PlatformVec.z != 0.0f) {
		m_Pos = VAdd(m_Pos, m_PlatformVec);
	}
	m_PlatformVec = VZERO;

	if (m_HitPoints <= 0) {
		m_HitPoints = 0;
		m_State = DEATH;
	}

#ifdef _DEBUG
	m_HitPoints = HIT_POINTS;
#endif // DEBUG

	if (m_Pos.y <= -500) {
		m_Pos = RESPAWN_POS;
		GravityReset();
	}

	//スタミナ処理
	StaminaManager();
	//状態遷移
	StateManager();
	//重力処理
	GravityManager();
	//アクション成功フラグ管理
	ActionSuccessManager();

	MV1SetPosition(m_Hndl, m_Pos);		//座標情報
	MV1SetRotationXYZ(m_Hndl, m_Rot);	//回転角度情報
	MV1SetScale(m_Hndl, m_Scale);		//スケール情報

	DrawFormatStringToHandle(50, 400, RED, DxLibFont::FONTHNDL_N20, "プレイヤーX軸：%2f", m_Pos.x);
	DrawFormatStringToHandle(50, 430, RED, DxLibFont::FONTHNDL_N20, "プレイヤーY軸：%2f", m_Pos.y);
	DrawFormatStringToHandle(50, 460, RED, DxLibFont::FONTHNDL_N20, "プレイヤーZ軸：%2f", m_Pos.z);

	/*VECTOR Pos = GetFramePos(m_Hndl,15);*/
	VECTOR Pos = GetFramePos(m_Hndl, 24);

	DrawFormatStringToHandle(50, 500, RED, DxLibFont::FONTHNDL_N20, "ボーンX軸：%2f", Pos.x);
	DrawFormatStringToHandle(50, 530, RED, DxLibFont::FONTHNDL_N20, "ボーンY軸：%2f", Pos.y);
	DrawFormatStringToHandle(50, 560, RED, DxLibFont::FONTHNDL_N20, "ボーンZ軸：%2f", Pos.z);
}
//当たり判定後の処理(当たっている場合)
void Player::HitCalc(ObjectBase* _Object) {
	//ボスクラスデータを保存する変数
	Boss* PointerBoss = nullptr;
	//ボスクラスをダウンキャスト
	PointerBoss = dynamic_cast<Boss*>(_Object);
	//ガード状態の時
	if (m_State == GUARD) {
		//パリィ許容フラグがオンなら
		if (m_IsParryWindo) {
			//パリィ状態に変更
			m_State = PARRY;
			//スタミナを回復
			m_Stamina += PointerBoss->GetPower() / 2;
			//パリィアクション成功
			m_IsActionSuccess[PARRY] = true;
		}
		else {
			//スタミナを消費
			m_Stamina -= PointerBoss->GetPower();
			//敵のの攻撃力に被ダメ率を乗算後HPを消費
			m_HitPoints = m_HitPoints - (PointerBoss->GetPower() * GUARD_DAMAGE_TAKEN_MULT);
			//ガードアクション成功
			m_IsActionSuccess[GUARD] = true;
			//ノックバックの力を計算
			float KnockBackPower = (PointerBoss->GetPower() * 0.5f) * GUARD_DAMAGE_TAKEN_MULT;
			//ノックバックデータ数値代入
			SetKnockBackData(KnockBackPower, PointerBoss->GetPos());
		}
	}
	else {
		//HPを消費
		m_HitPoints = m_HitPoints - PointerBoss->GetPower();
		//当たり判定オフ
		m_IsCollision = false;
		//ダメージ状態へ
		m_State = DAMAGE;
		//ノックバックの力を計算
		float KnockBackPower = PointerBoss->GetPower() * 0.5f;
		//ノックバックデータ数値代入
		SetKnockBackData(KnockBackPower, PointerBoss->GetPos());
	}
}
//待機
void Player::Wait() {
	//待機アニメーションループ再生
	RequestLoop(WAIT);
	//スタミナを回復する
	m_IsStaminaRecover = true;
	//通常移動方向設定
	if (UpdateNormalMoveVec()) {
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
	//当たり判定オフ
	m_IsCollision = false;
	//攻撃の当たり判定消滅
	m_IsAttackCollision = false;
	//ガードの当たり判定消失
	m_IsGuardCollision = false;
	//X軸回転率をリセット
	m_Rot.x = 0.0f;
	//アクションフラグをリセット
	ResetIsAction();
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = WAIT;
		//当たり判定オン
		m_IsCollision = true;
	}
	//ノックバック
	KnockBackManager();
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
	RequestLoop(2);
	//スタミナを回復する
	m_IsStaminaRecover = true;
	//通常移動方向設定
	if (UpdateNormalMoveVec()) {
		//移動計算
		NormalMoveCalc();
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
	if (UpdateNormalMoveVec()) {
		//移動計算
		NormalMoveCalc();
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
	if (UpdateNormalMoveVec()) {
		//移動計算
		NormalMoveCalc();
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
	if (UpdateNormalMoveVec()) {
		//移動計算
		NormalMoveCalc();
	}
	//スタミナを回復しない
	m_IsStaminaRecover = false;
	//ノックバック
	KnockBackManager();

	if (!m_IsAction[GUARD]) {
		//ガードアクション中に変更
		m_IsAction[GUARD] = true;
		//当たり判定オフ
		m_IsCollision = false;
		//ガードの当たり判定発生
		m_IsGuardCollision = true;
	}
	//ガードボタンを離したら
	if (!InputPad::IsPushPadRep(XINPUT_BUTTON_RIGHT_SHOULDER) && !InputKey::IsPushKeyRep(KEY_INPUT_F)) {
		//待機状態へ
		m_State = WAIT;
		//当たり判定オン
		m_IsCollision = true;
		//ガードアクション終了
		m_IsAction[GUARD] = false;
		//ガードの当たり判定消失
		m_IsGuardCollision = false;
		//ガードアクション実行後のパリィに移行できる許容時間をリセット
		m_ParryWindoeTime = 0;
		//パリィに移行してもよい
		m_IsParryWindo = true;
	}
	//スタミナが一定値を下回れば
	if (m_Stamina <= GUARD_MIN_STAMINA) {
		//待機状態へ
		m_State = WAIT;
		//当たり判定オン
		m_IsCollision = true;
		//ガードアクション終了
		m_IsAction[GUARD] = false;
		//ガードの当たり判定消失
		m_IsGuardCollision = false;
		//ガードアクション実行後のパリィに移行できる許容時間をリセット
		m_ParryWindoeTime = 0;
		//パリィに移行してもよい
		m_IsParryWindo = true;
	}
	//ガードアクション実行後のパリィに移行できる許容時間が一定の値を超えていれば
	if (m_ParryWindoeTime >= PARRY_WINDOW_TIME) {
		//パリィに移行することを許可しない
		m_IsParryWindo = false;
	}
	else {
		//ガードアクション実行後のパリィに移行できる許容時間を加算
		m_ParryWindoeTime++;
		//パリィに移行してもよい
		m_IsParryWindo = true;
	}
}
//パリィ
void Player::Parry() {
	//パリィアニメーション再生
	RequestEndLoop(PARRY);
	//スタミナを回復しない
	m_IsStaminaRecover = false;

	if (!m_IsAction[PARRY]) {
		//ガードアクション終了
		m_IsAction[GUARD] = false;
		//パリィアクション中に変更
		m_IsAction[PARRY] = true;
		//当たり判定オフ
		m_IsCollision = false;
		//ガードアクション実行後のパリィに移行できる許容時間をリセット
		m_ParryWindoeTime = 0;
		//パリィに移行してもよい
		m_IsParryWindo = true;
	}

	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = GUARD;
		//当たり判定オン
		m_IsCollision = true;
		//パリィアクション終了
		m_IsAction[PARRY] = false;
		//パリィアクション成功フラグをオフに
		m_IsActionSuccess[PARRY] = false;
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
		//押し出し判定オフ
		m_IsPush = false;
		//攻撃力設定
		m_Power = SKILL_ATTACKPOWER;
		//スキルゲージ減少
		m_SkillPoints += -10;
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
		NormalMoveCalc();
		if (!m_IsEffect) {
			//エフェクト発生判定オン
			m_IsEffect = true;
			//指定ボーンの座標取得
			VECTOR Pos = GetFramePos(m_Hndl, UPPERARM_RIGHT);
		}
		//指定ボーンの座標取得
		VECTOR Pos = GetFramePos(m_Hndl, UPPERARM_RIGHT);
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
		//押し出し判定オン
		m_IsPush = true;
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
		////正規化された方向ベクトルを取得
		//VECTOR DirToAttaxkTarget = GetDirectionNotY(m_Pos, m_AttackTargetPos, TRUE);
		////攻撃対象との角度差を設定
		//m_TargetAngle = atan2f(-DirToAttaxkTarget.x, -DirToAttaxkTarget.y);
	}
	//指定フレームから指定フレームまでの間
	if (m_AnimeData.Frame > NORMAL_ATTACK1_COLLISION_START && m_AnimeData.Frame < NORMAL_ATTACK1_COLLISION_END) {
		if (!m_IsAttackCollision) {
			//攻撃の当たり判定発生
			m_IsAttackCollision = true;
		}
		//攻撃移動計算
		AttackMoveCalc(NORMAL_ATTACK1_NUMBER);
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
	}
	//通常攻撃ボタンが押されたら
	if (InputPad::IsPushPadTrg(XINPUT_BUTTON_B) || InputKey::IsPushKeyTrg(KEY_INPUT_SPACE)) {
		m_IsNextNormalAttack[NORMAL_ATTACK2_NUMBER] = true;
		//攻撃移動方向更新
		if (!UpdateAttackMoveVec(NORMAL_ATTACK2_NUMBER)) {
			//何も入力されていなけらば-Z軸方向に進む
			m_AttackMoveVec[NORMAL_ATTACK2_NUMBER].z = -1.0f;
		}
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
	}
	//指定フレームから指定フレームまでの間
	if (m_AnimeData.Frame > NORMAL_ATTACK2_COLLISION_START && m_AnimeData.Frame < NORMAL_ATTACK2_COLLISION_END) {
		if (!m_IsAttackCollision) {
			//攻撃の当たり判定発生
			m_IsAttackCollision = true;
		}
		//攻撃移動計算
		AttackMoveCalc(NORMAL_ATTACK2_NUMBER);
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
	}
	//通常攻撃ボタンが押されたら
	if (InputPad::IsPushPadTrg(XINPUT_BUTTON_B) || InputKey::IsPushKeyTrg(KEY_INPUT_SPACE)) {
		m_IsNextNormalAttack[NORMAL_ATTACK3_NUMBER] = true;
		//攻撃移動方向更新
		if (!UpdateAttackMoveVec(NORMAL_ATTACK3_NUMBER)) {
			//何も入力されていなけらば-Z軸方向に進む
			m_AttackMoveVec[NORMAL_ATTACK3_NUMBER].z = -1.0f;
		}
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
	}
	//指定フレームから指定フレームまでの間
	if (m_AnimeData.Frame > NORMAL_ATTACK3_COLLISION_START && m_AnimeData.Frame < NORMAL_ATTACK3_COLLISION_END) {
		if (!m_IsAttackCollision) {
			//攻撃の当たり判定発生
			m_IsAttackCollision = true;
		}
		//攻撃移動計算
		AttackMoveCalc(NORMAL_ATTACK3_NUMBER);
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
	}
	//通常攻撃ボタンが押されたら
	if (InputPad::IsPushPadTrg(XINPUT_BUTTON_B) || InputKey::IsPushKeyTrg(KEY_INPUT_SPACE)) {
		m_IsNextNormalAttack[NORMAL_ATTACK1_NUMBER] = true;
		//攻撃移動方向更新
		if (!UpdateAttackMoveVec(NORMAL_ATTACK1_NUMBER)) {
			//何も入力されていなけらば-Z軸方向に進む
			m_AttackMoveVec[NORMAL_ATTACK1_NUMBER].z = -1.0f;
		}
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
//移動方向設定
bool Player::UpdateNormalMoveVec() {
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
void Player::NormalMoveCalc() {
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
//攻撃移動方向更新
bool Player::UpdateAttackMoveVec(int _Index) {
	bool IsMove = false;

	m_AttackMoveVec[_Index] = VZERO;
	//奥方向
	if (InputKey::IsPushKeyRep(KEY_INPUT_W)) {
		m_AttackMoveVec[_Index].z = -1.0f;
		IsMove = true;
	}
	//手前方向
	if (InputKey::IsPushKeyRep(KEY_INPUT_S)) {
		m_AttackMoveVec[_Index].z = 1.0f;
		IsMove = true;
	}
	//左方向
	if (InputKey::IsPushKeyRep(KEY_INPUT_A)) {
		m_AttackMoveVec[_Index].x = 1.0f;
		IsMove = true;
	}
	//右方向
	if (InputKey::IsPushKeyRep(KEY_INPUT_D)) {
		m_AttackMoveVec[_Index].x = -1.0f;
		IsMove = true;
	}

	if (InputPad::GetLAnalogYInput() > 0) {
		m_AttackMoveVec[_Index].z = -InputPad::GetLAnalogYInput();
		IsMove = true;
	}

	if (InputPad::GetLAnalogYInput() < 0) {
		m_AttackMoveVec[_Index].z = -InputPad::GetLAnalogYInput();
		IsMove = true;
	}

	if (InputPad::GetLAnalogXInput() < 0) {
		m_AttackMoveVec[_Index].x = -InputPad::GetLAnalogXInput();
		IsMove = true;
	}

	if (InputPad::GetLAnalogXInput() > 0) {
		m_AttackMoveVec[_Index].x = -InputPad::GetLAnalogXInput();
		IsMove = true;
	}

	return IsMove;
}
//攻撃移動計算
void Player::AttackMoveCalc(int _Index) {
	VECTOR AttackMoveVec = m_AttackMoveVec[_Index];
	AttackMoveVec = VNorm(AttackMoveVec);
	//回転行列
	MATRIX Mat1, Mat2;

	Mat2 = MGetRotY(m_Rot.y);
	Mat1 = MGetTranslate(AttackMoveVec);
	Mat1 = MMult(Mat1, Mat2);
	AttackMoveVec = VGet(Mat1.m[3][0], 0.0f, Mat1.m[3][2]);

	AttackMoveVec.x = AttackMoveVec.x * (PLAYER_NORMAL_ATTACK_MOVE_MULT);
	AttackMoveVec.z = AttackMoveVec.z * (PLAYER_NORMAL_ATTACK_MOVE_MULT);
	m_Pos = VAdd(m_Pos, AttackMoveVec);
	m_Rot.y = atan2f(-AttackMoveVec.x, -AttackMoveVec.z);
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
		DrawFormatStringToHandle(50, 300, RED, DxLibFont::FONTHNDL_N20, "WAIT");
		break;
	case DAMAGE:			//ダメージ
		Damage();
		DrawFormatStringToHandle(50, 300, RED, DxLibFont::FONTHNDL_N20, "DAMAGE");
		break;
	case DEATH:				//死亡
		Death();
		DrawFormatStringToHandle(50, 300, RED, DxLibFont::FONTHNDL_N20, "DEATH");
		break;
	case WALK:				//歩き
		Walk();
		DrawFormatStringToHandle(50, 300, RED, DxLibFont::FONTHNDL_N20, "WALK");
		break;
	case ROLLING:			//ローリング
		Rolling();
		DrawFormatStringToHandle(50, 300, RED, DxLibFont::FONTHNDL_N20, "ROLLING");
		break;
	case JUMP:				//ジャンプ
		Jump();
		DrawFormatStringToHandle(50, 300, RED, DxLibFont::FONTHNDL_N20, "JUMP");
		break;
	case GUARD:				//ガード
		Guard();
		DrawFormatStringToHandle(50, 300, RED, DxLibFont::FONTHNDL_N20, "GUARD");
		break;
	case PARRY:				//パリィ
		Parry();
		DrawFormatStringToHandle(50, 300, RED, DxLibFont::FONTHNDL_N20, "PARRY");
		break;
	case SKILL_ATTACK:		//スキル攻撃
		SkillAttack();
		DrawFormatStringToHandle(50, 300, RED, DxLibFont::FONTHNDL_N20, "SKILL_ATTACK");
		break;
	case NORMAL_ATTACK1:	//通常攻撃１段目
		NormalAttack1();
		DrawFormatStringToHandle(50, 300, RED, DxLibFont::FONTHNDL_N20, "NORMAL_ATTACK1");
		break;
	case NORMAL_ATTACK2:	//通常攻撃２段目
		NormalAttack2();
		DrawFormatStringToHandle(50, 300, RED, DxLibFont::FONTHNDL_N20, "NORMAL_ATTACK2");
		break;
	case NORMAL_ATTACK3:	//通常攻撃３段目
		NormalAttack3();
		DrawFormatStringToHandle(50, 300, RED, DxLibFont::FONTHNDL_N20, "NORMAL_ATTACK3");
		break;
	}
}
//動作管理
void Player::ActionManager() {
	//スタミナが一定値を上回っていれば
	if (m_Stamina > ROLLING_MIN_STAMINA) {
		//ローリング
		if (InputPad::IsPushPadTrg(XINPUT_BUTTON_X) || InputKey::IsPushKeyTrg(KEY_INPUT_R)) {
			m_State = ROLLING;
		}
	}
	//ジャンプ
	if ((InputPad::IsPushPadTrg(XINPUT_BUTTON_A) || InputKey::IsPushKeyTrg(KEY_INPUT_Z))) {
		m_State = JUMP;
	}
	//ガード
	if (InputPad::IsPushPadTrg(XINPUT_BUTTON_RIGHT_SHOULDER) || InputKey::IsPushKeyTrg(KEY_INPUT_F)) {
		m_State = GUARD;
	}
	//スキル攻撃
	if ((InputPad::IsPushPadTrg(XINPUT_BUTTON_Y) || InputKey::IsPushKeyTrg(KEY_INPUT_Q)) && m_SkillPoints > 0) {
		m_State = SKILL_ATTACK;
	}
	//通常攻撃１段目
	if (InputPad::IsPushPadTrg(XINPUT_BUTTON_B) || InputKey::IsPushKeyTrg(KEY_INPUT_SPACE)) {
		m_State = NORMAL_ATTACK1;
		m_IsNextNormalAttack[NORMAL_ATTACK1_NUMBER] = true;
		//攻撃移動方向更新
		if (!UpdateAttackMoveVec(NORMAL_ATTACK1_NUMBER)) {
			//何も入力されていなけらば-Z軸方向に進む
			m_AttackMoveVec[NORMAL_ATTACK1_NUMBER].z = -1.0f;
		}
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
//アクションフラグをリセット
void Player::ResetIsAction() {
	for (int State = 0; State < STATE_NUM; State++) {
		m_IsAction[State] = false;
	}
}
//アクション成功フラグ管理
void Player::ActionSuccessManager() {
	for (int State = 0; State < STATE_NUM; State++) {
		//アクション成功フラグがオンになっていなければ次の配列へ
		if (!m_IsActionSuccess[State])continue;
		//アクション成功フラグがオンになっているものはなにか
		switch (State) {
			//ガード
		case GUARD:
			//ガードアクション成功継続時間が一定の値以上になっていれば
			if (m_ActionSuccessTime[GUARD] >= GUARD_SUCCESS_TIME) {
				//ガードアクション成功継続時間をリセット
				m_ActionSuccessTime[GUARD] = 0;
				//ガードアクション成功フラグをオフに
				m_IsActionSuccess[GUARD] = false;
			}
			else {
				//ガードアクション成功継続時間を加算
				m_ActionSuccessTime[GUARD]++;
			}
			break;
		}
	}
}
//ノックバック
void Player::KnockBackManager() {
	if (!m_IsKnockBackCalcStart) {
		//ノックバック計算を開始
		m_IsKnockBackCalcStart = true;
		//ノックバック中フラグをオン
		m_IsKnockBack = true;
		//最大ノックバック量を保存
		m_KnockBackMaxDistance = m_KnockBackDistance;
		//１フレーム当たりの移動量を設定
		m_KnockBackSub = m_KnockBackMaxDistance * 0.1f;
	}
	//ノックバック中なら
	if (m_IsKnockBack) {
		//ノックバック継続時間を加算
		m_KnockBackDuration++;
		//方向ベクトルを取得(正規化済み)
		VECTOR DirToPlayerPos = GetDirectionNotY(m_KnockBackStartPos, m_Pos, true);
		if (m_KnockBackDuration <= 10) {
			//移動量を計算
			VECTOR KnockBackSpeed = VScale(DirToPlayerPos, m_KnockBackDistance);
			//座標に加算
			m_Pos = VAdd(m_Pos, KnockBackSpeed);
			//ノックバック量を1フレーム毎の減衰量分減らす
			m_KnockBackDistance -= m_KnockBackSub;
		}
		else {
			//ノックバック中フラグをオフ
			m_IsKnockBack = false;
			//ノックバック継続時間をリセット
			m_KnockBackDuration = 0;
		}
	}
}
//ノックバックデータ数値代入
void Player::SetKnockBackData(float _Power, VECTOR _Pos) {
	//スタミナ残量が少ないほどノックバック量が大きくる
	float Calc = (MAX_STAMINA - m_Stamina) * 0.01f;
	m_KnockBackDistance = _Power + Calc;
	//ノックバック計算を始める
	m_IsKnockBackCalcStart = false;
	//ノックバック開始時の敵座標
	m_KnockBackStartPos = _Pos;
}
