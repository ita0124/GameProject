#include "Player.h"
#include "Game/Base/Object/ActorBase/Character/EnemyBase/EnemyBase.h"
#include "Lib/Input/InputManager.h"

namespace {
	constexpr float		RAD = 5.0f;																//半径
	constexpr VECTOR	PLAYER_SIZE = { RAD,20.0f,RAD };										//ボックス当たり判定

	constexpr VECTOR	RESPAWN_POS = { 0.0f,0.0f,0.0f };										//落下後のリスポーン座標

	constexpr float		HIT_POINTS = 100.0f;													//体力
	constexpr float		STAMINA = 75.0f;														//スタミナ
	constexpr int		SKILL_POINTS = 50;														//スキルポイント
	constexpr float		MAX_HITPOINTS = 100.0f;													//最大体力
	constexpr float		MAX_STAMINA = 100.0f;													//最大スタミナ
	constexpr int		MAX_SKILL_POINTS = 100;													//最大スキルポイント

	constexpr float		ADD_STAMINA = 0.01f;													//プレイヤースタミナ回復量(１フレーム毎)

	constexpr float		WALK_MOVE_MULT = 1.75f;													//歩き時の移動乗算値
	constexpr float		ROLLING_MOVE_MULT = 3.0f;												//ローリング時の移動乗算値
	constexpr float		JUMP_MOVE_MULT = 2.0f;													//ジャンプ時の移動乗算値
	constexpr float		GUARD_MOVE_MULT = WALK_MOVE_MULT / 2.5f;								//ガード時の移動乗算値(歩き/指定値)
	constexpr float		SKILL_ATTACK_MOVE_MULT = 15.0f;											//スキル攻撃時の移動乗算値
	constexpr float		NORMAL_ATTACK_MOVE_MULT = 3.0f;											//通常攻撃時の移動乗算値

	constexpr float		NORMAL_MOVE_ROTATE_SPEED = 0.25f;										//通常移動時の回転速度
	constexpr float		NORMAL_ATTACK1_MOVE_ROTATE_SPEED = 1.0f;								//通常攻撃１段目の回転速度
	constexpr float		NORMAL_ATTACK2_MOVE_ROTATE_SPEED = 0.1f;								//通常攻撃２段目の回転速度
	constexpr float		NORMAL_ATTACK3_MOVE_ROTATE_SPEED = 0.25f;								//通常攻撃３段目の回転速度

	constexpr float		ROLLING_SUB_STAMINA = 10.0f;											//ローリング時のスタミナ減算値

	constexpr float		GUARD_MIN_STAMINA = 10.0f;												//ガード状態維持に必要な最低限のスタミナ量
	constexpr float		ROLLING_MIN_STAMINA = 10.0f;											//ローリング発動に必要な最低限のスタミナ量

	constexpr float		SKILL_ATTACK_COLLISION_START = 45.0f;									//スキル攻撃の当たり判定開始フレーム
	constexpr float		SKILL_ATTACK_COLLISION_END = 55.0f;										//スキル攻撃の当たり判定終了フレーム
	constexpr float		NORMAL_ATTACK1_COLLISION_START = 15.0f;									//通常攻撃１段目の当たり判定開始フレーム
	constexpr float		NORMAL_ATTACK1_COLLISION_END = 25.0f;									//通常攻撃１段目の当たり判定終了フレーム
	constexpr float		NORMAL_ATTACK1_TRANSITION = 30.0f;										//通常攻撃１段目から通常攻撃２段目に移行するフレーム
	constexpr float		NORMAL_ATTACK2_COLLISION_START = 0.0f;									//通常攻撃２段目の当たり判定開始フレーム
	constexpr float		NORMAL_ATTACK2_COLLISION_END = 10.0f;									//通常攻撃２段目の当たり判定終了フレーム
	constexpr float		NORMAL_ATTACK2_TRANSITION = 17.0f;										//通常攻撃２段目から通常攻撃３段目に移行するフレーム
	constexpr float		NORMAL_ATTACK3_COLLISION_START = 25.0f;									//通常攻撃３段目の当たり判定開始フレーム
	constexpr float		NORMAL_ATTACK3_COLLISION_END = 30.0f;									//通常攻撃３段目の当たり判定終了フレーム
	constexpr float		NORMAL_ATTACK3_TRANSITION = 40.0f;										//通常攻撃３段目から通常攻撃１段目に移行するフレーム

	constexpr float		SKILL_ATTACKPOWER = 150.0f;												//スキル攻撃時の攻撃力
	constexpr float		NORMAL_ATTACK1_POWER = 15.0f;											//通常攻撃１段目時の攻撃力
	constexpr float		NORAML_ATTACK2_POWER = 10.0f;											//通常攻撃２段目時の攻撃力
	constexpr float		NORAML_ATTACK3_POWER = 30.0f;											//通常攻撃３段目時の攻撃力

	constexpr float		SKILL_KNOCK_BACkK_POWER = 50.0;											//スキル攻撃時の敵へ与えるノックバックの強さ
	constexpr float		NORMAL_ATTACK1_BACkK_POWER = 5.0;										//通常攻撃１段目時の敵へ与えるノックバックの強さ
	constexpr float		NORMAL_ATTACK2_BACkK_POWER = 5.0;										//通常攻撃２段目時の敵へ与えるノックバックの強さ
	constexpr float		NORMAL_ATTACK3_BACkK_POWER = 25.0;										//通常攻撃３段目時の敵へ与えるノックバックの強さ

	constexpr float ATTACK_TARGET_APPROACH_DISTANCE = 100.0f;									//攻撃対象へ向かう距離の閾値

	constexpr float		GUARD_SUCCESS_TIME = 30;												//ガードアクション成功の継続時間
	constexpr float		PARRY_SUCCESS_TIME = 30;												//パリィアクション成功の継続時間

	constexpr int		GUARD_HOLD_TIME = 13;													//ガードアクションを最低限継続する時間

	constexpr float		GUARD_DAMAGE_TAKEN_MULT = 0.2f;											//ガード時の被ダメ軽減量

	constexpr float		ANIME_WALK_SPEED = 0.75f;												//歩きアニメーションの再生速度
	constexpr float		ANIME_GUARD_START_SPEED = 2.0f;											//ガード開始アニメーションの再生速度
	constexpr float		ANIME_GUARD_END_SPEED = 1.25f;											//ガード終了アニメーションの再生速度
	constexpr float		ANIME_NORMAL_ATTACK1_SPEED = 1.0f;										//通常攻撃１段目アニメーションの再生速度
	constexpr float		ANIME_NORMAL_ATTACK2_SPEED = 1.0f;										//通常攻撃２段目アニメーションの再生速度
	constexpr float		ANIME_NORMAL_ATTACK3_SPEED = 1.25f;										//通常攻撃３段目アニメーションの再生速度

	constexpr int		DAMAGE_RECOVERY_TIME = 0;												//ダメージの硬直フレーム数
	constexpr int		NORMAL_ATTACK1_RECOVERY_TIME = 5;										//通常攻撃1段目の硬直フレーム数
	constexpr int		NORMAL_ATTACK2_RECOVERY_TIME = 0;										//通常攻撃2段目の硬直フレーム数
	constexpr int		NORMAL_ATTACK3_RECOVERY_TIME = 5;										//通常攻撃３段目の硬直フレーム数

	constexpr float		FIRST_JUMP_POWER = 3.7f;												//初回ジャンプ力
	constexpr float		JUMP_POWER_MAX = -7.5f;													//ジャンプ速度の下限
	constexpr float		GRAVITY = -0.025f;														//重力
	constexpr float		GRAVITY_MAX = -0.25f;													//最大重力

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

	m_Pos = RESPAWN_POS;
	m_RespawnPos = RESPAWN_POS;					//リスポーン座標
	m_IsGoal = false;							//ゴールしたか

	m_IsPush = true;							//押し出し判定を行う

	m_Rad = RAD;								//半径
	m_Size = PLAYER_SIZE;						//ボックス当たり判定

	m_HitPoints = HIT_POINTS;					//体力
	m_MaxHitPoints = MAX_HITPOINTS;				//最大体力
	m_Stamina = STAMINA;						//スタミナ
	m_MaxStamina = MAX_STAMINA;					//最大スタミナ
	m_SkillPoints = SKILL_POINTS;				//スキルポイント

	m_State = IDEL;								//プレイヤー状態変数
	m_PrevState = m_State;						//１フレーム前の状態

	m_IsHit = false;							//何かに当たった

	m_KnockBackPower = 0.0f;					//攻撃時に敵へ与えるノックバックの強さ

	m_IsGuardCollision = false;					//ガードの当たり判定を発生させてよいか
	m_IsParryCollision = false;					//パリィ許容フラグ
	m_IsGuardSuccess = false;					//ガードに成功したか
	m_GuardSuccessTime = 0;						//ガードに成功してどれだけ経ったか
	m_ParrySuccessTime = 0;						//パリィに成功してどれだけ経ったか
	m_GuardTimer = 0;							//ガード入力を継続している時間

	m_IsParrySucess = false;					//パリィが成功したか

	m_JumpPower = 0.0f;							//ジャンプ力計算

	m_AttackTarget = nullptr;					//攻撃対象
	m_AttackTargetPos = VZERO;					//攻撃サーチを行う物体の座標
	m_TargetAngle = 0.0f;						// 攻撃対象との角度差
	for (int Index = 0; Index < NORMAL_ATTACK_MAX; Index++) {
		m_IsNextNormalAttack[Index] = false;	//通常攻撃の次の段数に移行するか
		m_AttackMoveVec[Index] = VZERO;			//攻撃進行方向
		m_IsSetAttackMoveVec[Index] = true;		//攻撃進行方向を設定したか
	}
	m_IsAttackCollision = false;				//攻撃の当たり判定を発生させてよいか

	m_IsRespawn = false;						//リスポーン中か

	m_RecoveryTime = 0;							//行動可能になるまでの硬直時間
	m_IsRecovery = true;						//硬直中かどうか
	m_IsSetRecovery = false;					//硬直を開始したか
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

	if (m_AttackTarget != nullptr) {
		if (m_AttackTarget->GetIsActive()) {
			m_AttackTargetPos = m_AttackTarget->GetPos();
		}
		else {
			m_AttackTarget = nullptr;
		}
	}

	if (m_HitPoints <= 0) {
		m_HitPoints = 0;
		m_State = DEATH;
	}

#ifdef _DEBUG
	m_HitPoints = HIT_POINTS;
#endif // DEBUG

	if (m_Pos.y <= -500) {
		m_IsRespawn = true;
	}
	if (!m_IsRespawn) {
		//スタミナ処理
		StaminaManager();
		//状態遷移
		StateManager();
		//重力処理
		GravityManager();
		//ガードの成功判定を管理する
		IsGuardSuccessManager();
		//パリィの成功判定を管理する
		IsParrySucessManager();
	}
	else {
		GravityReset();
	}
	MV1SetPosition(m_Hndl, m_Pos);		//座標情報
	MV1SetRotationXYZ(m_Hndl, m_Rot);	//回転角度情報
	MV1SetScale(m_Hndl, m_Scale);		//スケール情報
#ifdef _DEBUG
	DrawFormatStringToHandle(10, 600, RED, DxLibFont::FONTHNDL_N20, "プレイヤー座標X:%.1f", m_Pos.x);
	DrawFormatStringToHandle(10, 620, RED, DxLibFont::FONTHNDL_N20, "プレイヤー座標Y:%.1f", m_Pos.y);
	DrawFormatStringToHandle(10, 640, RED, DxLibFont::FONTHNDL_N20, "プレイヤー座標Z:%.1f", m_Pos.z);
#endif // DEBUG
}
//当たり判定後の処理(当たっている場合)
void Player::HitCalc(ObjectBase* _Object) {
	//エネミーベースクラスデータを保存する変数
	EnemyBase* PointerEnemy = nullptr;
	//エネミーベースクラスをダウンキャスト
	PointerEnemy = dynamic_cast<EnemyBase*>(_Object);
	//パリィの当たりがオン
	if (m_IsParryCollision) {
		//パリィ成功
		m_IsParrySucess = true;
		//スタミナを回復
		m_Stamina += PointerEnemy->GetPower() / 2;
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_SHIELD)) {
			SoundManager::Play(SoundManager::TagID::SE_SHIELD);
		}
	}
	//ガードの当たりがオン
	else if (m_IsGuardCollision) {
		//スタミナを消費
		m_Stamina -= PointerEnemy->GetPower();
		//敵のの攻撃力に被ダメ率を乗算後HPを消費
		m_HitPoints = m_HitPoints - (PointerEnemy->GetPower() * GUARD_DAMAGE_TAKEN_MULT);
		//ガード成功
		m_IsGuardSuccess = true;
		//ノックバックの力を計算
		float KnockBackPower = PointerEnemy->GetPower() * GUARD_DAMAGE_TAKEN_MULT;
		//ノックバックデータ数値代入
		SetKnockBackData(PointerEnemy->GetPos());
		//指定ボーンの座標取得
		VECTOR Pos = m_Pos;
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::SIMPLE_SPAWNMETHOD, Pos, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_HIT)) {
			SoundManager::Play(SoundManager::TagID::SE_HIT);
		}
	}
	else {
		//HPを消費
		m_HitPoints = m_HitPoints - PointerEnemy->GetPower();
		//当たり判定オフ
		m_IsCollision = false;
		//ダメージ状態へ
		m_State = DAMAGE;
		//ノックバックの力を計算
		float KnockBackPower = PointerEnemy->GetPower();
		//ノックバックデータ数値代入
		SetKnockBackData(PointerEnemy->GetPos());
		//指定ボーンの座標取得
		VECTOR Pos = m_Pos;
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::SIMPLE_SPAWNMETHOD, Pos, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
		//サウンドリクエスト
		if (!SoundManager::IsPlay(SoundManager::TagID::SE_HIT)) {
			SoundManager::Play(SoundManager::TagID::SE_HIT);
		}
	}
	//当たった
	m_IsHit = true;
}
//リスポーン処理
void Player::Respawn() {
	m_Pos = m_RespawnPos;
	//ジャンプ力リセット
	m_JumpPower = 0.0f;
	//ダメージ状態へ
	m_State = IDEL;
	//状態遷移
	StateManager();
}
//待機
void Player::Idel() {
	//待機アニメーションループ再生
	RequestLoop(ANIME_IDEL);
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//スタミナを回復する
		m_IsStaminaRecover = true;
		//攻撃力設定
		m_Power = 0.0f;
		//当たり判定をオン
		m_IsCollision = true;
	}
	//通常移動方向設定
	if (SetNormalMoveVec()) {
		//歩き状態へ
		m_State = WALK;
	}
	//動作管理
	ActionManager();
}
//ダメージ
void Player::Damage() {
	//ダメージアニメーション再生
	RequestEndLoop(ANIME_DAMAGE);
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//スタミナを回復しない
		m_IsStaminaRecover = false;
		//当たり判定オフ
		m_IsCollision = false;
		//攻撃の当たり判定消滅
		m_IsAttackCollision = false;
		//ガードの当たり判定消失
		m_IsGuardCollision = false;
		//押し出し判定オフ
		m_IsPush = false;
	}
	//ノックバック
	KnockBackManager();
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = IDEL;
		//当たり判定オン
		m_IsCollision = true;
		//押し出し判定オン
		m_IsPush = true;
		//当たってない
		m_IsHit = false;
	}
}
//死亡
void Player::Death() {
	//死亡アニメーション再生
	RequestEndLoop(ANIME_DEATH);
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
	RequestLoop(ANIME_RUN, ANIME_WALK_SPEED);
	//スタミナを回復する
	m_IsStaminaRecover = true;
	//通常移動方向設定
	if (SetNormalMoveVec()) {
		//移動計算
		NormalMoveCalc();
	}
	else {
		//待機状態へ
		m_State = IDEL;
	}
	//動作管理
	ActionManager();
}
//ローリング
void Player::Rolling() {
	//ローリングアニメーション再生
	RequestEndLoop(ANIME_ROLLING);
	//通常移動方向設定
	if (!SetNormalMoveVec()) {
		m_MoveVec.z = -1.0f;
	}
	//移動計算
	NormalMoveCalc();
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//スタミナを回復しない
		m_IsStaminaRecover = false;
		//スタミナを減らす
		m_Stamina -= ROLLING_SUB_STAMINA;
	}
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = IDEL;
	}
}
//ジャンプ
void Player::Jump() {
	//ジャンプアニメーション再生
	RequestEndLoop(ANIME_JUMP);
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//重力処理オン
		m_IsGravity = true;
		//初回ジャンプ力設定
		m_JumpPower = FIRST_JUMP_POWER;
	}
	//通常移動方向設定
	if (SetNormalMoveVec()) {
		//移動計算
		NormalMoveCalc();
	}
	//重力処理がオフになったら
	if (!m_IsGravity) {
		//待機状態へ
		m_State = IDEL;
	}
}
//落下
void Player::Falling() {
	//落下開始アニメーションループ再生
	RequestLoop(ANIME_FALLING);
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//ガード当たり判定消失
		m_IsGuardCollision = false;
		//当たり判定をオン
		m_IsCollision = true;
	}
	//重力処理がオフになったら
	if (!m_IsGravity) {
		//待機状態へ
		m_State = IDEL;
	}
}
//ガード開始
void Player::GuardStart() {
	//ガード開始アニメーション再生
	RequestEndLoop(ANIME_GUARD_START, ANIME_GUARD_START_SPEED);
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//スタミナを回復しない
		m_IsStaminaRecover = false;
		//パリィの当たり判定をオン
		m_IsParryCollision = true;
		//当たり判定をオフ
		m_IsCollision = false;
	}
	//アニメーションが終わったら
	else if (m_AnimeData.EndFlg) {
		//パリィが成功していたら
		if (m_IsParrySucess) {
			//ガード終了状態へ
			m_State = GUARD_END;
			//パリィの当たり判定をオフ
			m_IsParryCollision = false;
		}
		else {
			//ガード待機状態へ
			m_State = GUARD_IDEL;
			//パリィの当たり判定をオフ
			m_IsParryCollision = false;
		}
	}
}
//ガード待機
void Player::GuardIdel() {
	//ガード開始アニメーションループ再生
	RequestLoop(ANIME_GUARD_START);
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//ガード当たり判定をオン
		m_IsGuardCollision = true;
	}
	//ノックバック
	KnockBackManager();
	//ガード入力を継続している時間を加算
	m_GuardTimer++;
	//ガードボタンを離したら
	if (!InputManager::IsPushRep(InputManager::TagInput::INPUT_GUARD)) {
		if (m_GuardTimer >= GUARD_HOLD_TIME) {
			//ガード終了状態へ
			m_State = GUARD_END;
			//ガード当たり判定消失
			m_IsGuardCollision = false;
			//当たり判定をオン
			m_IsCollision = true;
			//ガード入力を継続している時間をリセット
			m_GuardTimer = 0;
		}
	}
}
//ガード終了
void Player::GuardEnd() {
	//ガード開始アニメーションループ再生
	RequestEndLoop(ANIME_GUARD_END, ANIME_GUARD_END_SPEED);
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
	}
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = IDEL;
		//当たってない
		m_IsHit = false;
	}
}
//スキル攻撃
void Player::SkillAttack() {
	//スキル攻撃アニメーション再生
	RequestEndLoop(ANIME_SKILL_ATTACK);

	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//当たり判定オフ
		m_IsCollision = false;
		//押し出し判定オフ
		m_IsPush = false;
		//攻撃力設定
		m_Power = SKILL_ATTACKPOWER;
		//敵へ与えるノックバックの強さ設定
		m_KnockBackPower = SKILL_KNOCK_BACkK_POWER;
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
			VECTOR Pos = GetFramePos(m_Hndl, RIGHT_HAND2);
			//エフェクトリクエスト
			m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK01SWORD21, Pos, false);
			//エフェクトの回転角度を設定
			MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
		}
		//指定ボーンの座標取得
		VECTOR Pos = GetFramePos(m_Hndl, RIGHT_HAND2);
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
		m_State = IDEL;
		//当たり判定オン
		m_IsCollision = true;
		//エフェクト発生判定オフ
		m_IsEffect = false;
		//押し出し判定オン
		m_IsPush = true;
	}
}
//通常攻撃１段目
void Player::NormalAttack1() {
	//通常攻撃_Index段目アニメーション再生
	RequestEndLoop(ANIME_NORMAL_ATTACK1, ANIME_NORMAL_ATTACK1_SPEED);
	//攻撃移動方向更新
	if (UpdateAttackMoveVec(NORMAL_ATTACK1_NUMBER)) {
		m_IsSetAttackMoveVec[NORMAL_ATTACK1_NUMBER] = true;
	}

	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//攻撃力設定
		m_Power = NORMAL_ATTACK1_POWER;
		//敵へ与えるノックバックの強さ設定
		m_KnockBackPower = NORMAL_ATTACK1_BACkK_POWER;
		//攻撃回転補間速度を設定
		m_AttackRotationSpeed = NORMAL_ATTACK1_MOVE_ROTATE_SPEED;
		//サウンドリクエスト
		SoundManager::Play(SoundManager::TagID::SE_ATK);
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
		VECTOR Pos = GetFramePos(m_Hndl, RIGHT_HAND2);
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK01SWORD1_1, Pos, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
	}
	//通常攻撃ボタンが押されたら
	if (m_AnimeData.Frame < NORMAL_ATTACK1_TRANSITION) {
		if (InputManager::IsPushTrg(InputManager::TagInput::INPUT_NORMAL_ATTACK)) {
			m_IsNextNormalAttack[NORMAL_ATTACK2_NUMBER] = true;
			//攻撃移動方向更新
			if (!SetAttackMoveVec(NORMAL_ATTACK2_NUMBER)) {
				//何も入力されなかった
				m_IsSetAttackMoveVec[NORMAL_ATTACK2_NUMBER] = false;
			}
		}
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK1_TRANSITION) {
		m_IsSetAttackMoveVec[NORMAL_ATTACK1_NUMBER] = true;
		//硬直を設定していなければ
		if (!m_IsSetRecovery) {
			//硬直設定済みにする
			m_IsSetRecovery = true;
			//アニメーションの硬直を設定
			SetAnimeRecoveryManager(NORMAL_ATTACK1_RECOVERY_TIME);
		}
		//アニメーションの硬直を更新
		if (UpdateAnimeRecoveryManager(ANIME_NORMAL_ATTACK1_SPEED)) {
			if (m_IsNextNormalAttack[NORMAL_ATTACK2_NUMBER]) {
				//次の攻撃状態へ
				m_State = NORMAL_ATTACK2;
				//通常攻撃の段数判定をオフ
				m_IsNextNormalAttack[NORMAL_ATTACK1_NUMBER] = false;
				//エフェクト発生判定オフ
				m_IsEffect = false;
				//硬直設定をリセット
				m_IsSetRecovery = false;
			}
			else if (SetNormalMoveVec()) {
				//歩き状態へ
				m_State = WALK;
				//通常攻撃の段数判定をオフ
				m_IsNextNormalAttack[NORMAL_ATTACK1_NUMBER] = false;
				//エフェクト発生判定オフ
				m_IsEffect = false;
				//硬直設定をリセット
				m_IsSetRecovery = false;
			}
			else if (ActionManager()) {
				//通常攻撃の段数判定をオフ
				m_IsNextNormalAttack[NORMAL_ATTACK1_NUMBER] = false;
				//エフェクト発生判定オフ
				m_IsEffect = false;
				//硬直設定をリセット
				m_IsSetRecovery = false;
			}
		}
	}
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = IDEL;
		//通常攻撃の段数判定をオフ
		m_IsNextNormalAttack[NORMAL_ATTACK1_NUMBER] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
		//硬直設定をリセット
		m_IsSetRecovery = false;
	}
}
//通常攻撃２段目
void Player::NormalAttack2() {
	//通常攻撃_Index段目アニメーション再生
	RequestEndLoop(ANIME_NORMAL_ATTACK2, ANIME_NORMAL_ATTACK2_SPEED);
	//攻撃移動方向更新
	if (UpdateAttackMoveVec(NORMAL_ATTACK2_NUMBER)) {
		m_IsSetAttackMoveVec[NORMAL_ATTACK2_NUMBER] = true;
	}

	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//攻撃力設定
		m_Power = NORAML_ATTACK2_POWER;
		//敵へ与えるノックバックの強さ設定
		m_KnockBackPower = NORMAL_ATTACK2_BACkK_POWER;
		//攻撃回転補間速度を設定
		m_AttackRotationSpeed = NORMAL_ATTACK2_MOVE_ROTATE_SPEED;
		//サウンドリクエスト
		SoundManager::Play(SoundManager::TagID::SE_ATK);
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
		VECTOR Pos = GetFramePos(m_Hndl, RIGHT_HAND2);
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK01SWORD1_2, Pos, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
	}
	//通常攻撃ボタンが押されたら
	if (m_AnimeData.Frame < NORMAL_ATTACK2_TRANSITION) {
		if (InputManager::IsPushTrg(InputManager::TagInput::INPUT_NORMAL_ATTACK)) {
			m_IsNextNormalAttack[NORMAL_ATTACK3_NUMBER] = true;
			//攻撃移動方向更新
			if (!SetAttackMoveVec(NORMAL_ATTACK3_NUMBER)) {
				//何も入力されなかった
				m_IsSetAttackMoveVec[NORMAL_ATTACK3_NUMBER] = false;
			}
		}
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK2_TRANSITION) {
		//何も入力されなかった
		m_IsSetAttackMoveVec[NORMAL_ATTACK2_NUMBER] = true;
		//硬直を設定していなければ
		if (!m_IsSetRecovery) {
			//硬直設定済みにする
			m_IsSetRecovery = true;
			//アニメーションの硬直を設定
			SetAnimeRecoveryManager(NORMAL_ATTACK2_RECOVERY_TIME);
		}
		//アニメーションの硬直を更新
		if (UpdateAnimeRecoveryManager(ANIME_NORMAL_ATTACK2_SPEED)) {
			if (m_IsNextNormalAttack[NORMAL_ATTACK3_NUMBER]) {
				//次の攻撃状態へ
				m_State = NORMAL_ATTACK3;
				//通常攻撃の段数判定をオフ
				m_IsNextNormalAttack[NORMAL_ATTACK2_NUMBER] = false;
				//エフェクト発生判定オフ
				m_IsEffect = false;
				//硬直設定をリセット
				m_IsSetRecovery = false;
			}
			else if (SetNormalMoveVec()) {
				//歩き状態へ
				m_State = WALK;
				//通常攻撃の段数判定をオフ
				m_IsNextNormalAttack[NORMAL_ATTACK2_NUMBER] = false;
				//エフェクト発生判定オフ
				m_IsEffect = false;
				//硬直設定をリセット
				m_IsSetRecovery = false;
			}
			else if (ActionManager()) {
				//通常攻撃の段数判定をオフ
				m_IsNextNormalAttack[NORMAL_ATTACK2_NUMBER] = false;
				//エフェクト発生判定オフ
				m_IsEffect = false;
				//硬直設定をリセット
				m_IsSetRecovery = false;
			}
		}
	}
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = IDEL;
		//通常攻撃の段数判定をオフ
		m_IsNextNormalAttack[NORMAL_ATTACK2_NUMBER] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
		//硬直設定をリセット
		m_IsSetRecovery = false;
	}
}
//通常攻撃３段目
void Player::NormalAttack3() {
	//通常攻撃_Index段目アニメーション再生
	RequestEndLoop(ANIME_NORMAL_ATTACK3, ANIME_NORMAL_ATTACK3_SPEED);
	//攻撃移動方向更新
	//攻撃移動方向更新
	if (UpdateAttackMoveVec(NORMAL_ATTACK3_NUMBER)) {
		m_IsSetAttackMoveVec[NORMAL_ATTACK3_NUMBER] = true;
	}

	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//攻撃力設定
		m_Power = NORAML_ATTACK3_POWER;
		//敵へ与えるノックバックの強さ設定
		m_KnockBackPower = NORMAL_ATTACK3_BACkK_POWER;
		//攻撃回転補間速度を設定
		m_AttackRotationSpeed = NORMAL_ATTACK3_MOVE_ROTATE_SPEED;
		//サウンドリクエスト
		SoundManager::Play(SoundManager::TagID::SE_ATK);
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
		VECTOR Pos = GetFramePos(m_Hndl, RIGHT_HAND2);
		//エフェクトリクエスト
		m_EffectHndl = MyEffeckseer::Request(MyEffeckseer::EFFECTID::TKTK01SWORD1_1, Pos, false);
		//エフェクトの回転角度を設定
		MyEffeckseer::SetRot(m_EffectHndl, m_Rot);
	}
	if (m_AnimeData.Frame > NORMAL_ATTACK3_TRANSITION) {
		//何も入力されなかった
		m_IsSetAttackMoveVec[NORMAL_ATTACK3_NUMBER] = true;
		//硬直を設定していなければ
		if (!m_IsSetRecovery) {
			//硬直設定済みにする
			m_IsSetRecovery = true;
			//アニメーションの硬直を設定
			SetAnimeRecoveryManager(NORMAL_ATTACK3_RECOVERY_TIME);
		}
		//アニメーションの硬直を更新
		if (UpdateAnimeRecoveryManager(ANIME_NORMAL_ATTACK3_SPEED)) {
			if (SetNormalMoveVec()) {
				//歩き状態へ
				m_State = WALK;
				//通常攻撃の段数判定をオフ
				m_IsNextNormalAttack[NORMAL_ATTACK3_NUMBER] = false;
				//エフェクト発生判定オフ
				m_IsEffect = false;
				//硬直設定をリセット
				m_IsSetRecovery = false;
			}
			else if (ActionManager()) {
				//通常攻撃の段数判定をオフ
				m_IsNextNormalAttack[NORMAL_ATTACK3_NUMBER] = false;
				//エフェクト発生判定オフ
				m_IsEffect = false;
				//硬直設定をリセット
				m_IsSetRecovery = false;
			}
		}
	}
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//待機状態へ
		m_State = IDEL;
		//通常攻撃の段数判定をオフ
		m_IsNextNormalAttack[NORMAL_ATTACK3_NUMBER] = false;
		//エフェクト発生判定オフ
		m_IsEffect = false;
		//硬直設定をリセット
		m_IsSetRecovery = false;
	}
}
//通常移動方向設定
bool Player::SetNormalMoveVec() {
	bool IsMove = false;

	m_MoveVec = VZERO;
	if (InputManager::GetLAnalogYInput() != 0.0f) {
		m_MoveVec.z = -InputManager::GetLAnalogYInput();
		IsMove = true;
	}
	if (InputManager::GetLAnalogXInput() != 0.0f) {
		m_MoveVec.x = -InputManager::GetLAnalogXInput();
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
	case JUMP:			//ジャンプ
		MoveVec = VScale(MoveVec, JUMP_MOVE_MULT);
		break;
	case SKILL_ATTACK:	//スキル攻撃
		MoveVec = VScale(MoveVec, SKILL_ATTACK_MOVE_MULT);
		break;
	}
	//移動
	m_Pos = VAdd(m_Pos, MoveVec);
	//方向ベクトルを反転
	MoveVec = VScale(MoveVec, -1.0f);
	//移動方向を向く
	UpdateRotation(MoveVec, NORMAL_MOVE_ROTATE_SPEED);
}
//攻撃移動方向設定
bool Player::SetAttackMoveVec(int _Index) {
	bool IsMove = false;

	m_AttackMoveVec[_Index] = VZERO;

	IsMove = UpdateAttackMoveVec(_Index);

	return IsMove;
}
//攻撃移動方向更新
bool Player::UpdateAttackMoveVec(int _Index) {
	bool IsMove = false;
	if (InputManager::GetLAnalogYInput() != 0.0f) {
		m_AttackMoveVec[_Index].z = -InputManager::GetLAnalogYInput();
		IsMove = true;
	}
	if (InputManager::GetLAnalogXInput() != 0.0f) {
		m_AttackMoveVec[_Index].x = -InputManager::GetLAnalogXInput();
		IsMove = true;
	}
	return IsMove;
}
//攻撃移動計算
void Player::AttackMoveCalc(int _Index) {
	VECTOR AttackMoveVec = m_AttackMoveVec[_Index];
	//入力がある場合はカメラの向き、入力がない場合はプレイヤーの向きを使用する
	float RotY = m_CamraRot.y;
	if (!m_IsSetAttackMoveVec[_Index]) {
		//攻撃対象への移動方向を決定
		VECTOR TargetDirection = GetDirectionNotY(m_Pos, m_AttackTargetPos);
		float Lenght = VSize(TargetDirection);
		//攻撃対象が近い場合は、対象へ向かって移動
		if (Lenght < ATTACK_TARGET_APPROACH_DISTANCE) {
			//代入
			AttackMoveVec = TargetDirection;
		}
		else {
			RotY = m_Rot.y;
			//プレイヤー前方向に移動する
			AttackMoveVec.z = -1.0f;
		}
	}
	//MoveVecを正規化
	AttackMoveVec = VNorm(AttackMoveVec);
	//MoveVecを行列化
	MATRIX MatMoveVec = MGetTranslate(AttackMoveVec);
	//カメラのY軸回転値を行列化
	MATRIX MatRotY = MGetRotY(RotY);
	//行列合成
	MATRIX MatComposition = MMult(MatMoveVec, MatRotY);
	//合成した行列から移動方向を取得
	AttackMoveVec = VGet(MatComposition.m[3][0], 0.0f, MatComposition.m[3][2]);
	//攻撃移動速度を適用
	AttackMoveVec = VScale(AttackMoveVec, NORMAL_ATTACK_MOVE_MULT);
	//攻撃移動
	m_Pos = VAdd(m_Pos, AttackMoveVec);
	//方向ベクトルを反転
	AttackMoveVec = VScale(AttackMoveVec, -1.0f);
	//移動方向を向く
	UpdateRotation(AttackMoveVec, m_AttackRotationSpeed);
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
	case IDEL:				//待機
		Idel();
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
	case FALLING:
		Falling();
		break;
	case GUARD_START:		//ガード開始
		GuardStart();
		break;
	case GUARD_IDEL:		//ガード待機
		GuardIdel();
		break;
	case GUARD_END:			//ガード終了
		GuardEnd();
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
bool Player::ActionManager() {
	bool IsAction = false;

	//スタミナが一定値を上回っていれば
	if (m_Stamina > ROLLING_MIN_STAMINA) {
		//ローリング
		if (InputManager::IsPushTrg(InputManager::TagInput::INPUT_ROLLING)) {
			m_State = ROLLING;
			IsAction = true;
		}
	}
	//ジャンプ
	if (InputManager::IsPushTrg(InputManager::TagInput::INPUT_JUMP)) {
		m_State = JUMP;
		IsAction = true;
	}
	//ガード
	if (InputManager::IsPushTrg(InputManager::TagInput::INPUT_GUARD)) {
		m_State = GUARD_START;
		IsAction = true;
	}
	//スキル攻撃
	if (InputManager::IsPushTrg(InputManager::TagInput::INPUT_SKILL_ATTACK) && m_SkillPoints > 0) {
		m_State = SKILL_ATTACK;
		IsAction = true;
	}
	//通常攻撃１段目
	if (InputManager::IsPushTrg(InputManager::TagInput::INPUT_NORMAL_ATTACK)) {
		m_State = NORMAL_ATTACK1;
		m_IsNextNormalAttack[NORMAL_ATTACK1_NUMBER] = true;
		//攻撃移動方向更新
		if (!SetAttackMoveVec(NORMAL_ATTACK1_NUMBER)) {
			//何も入力されなかった
			m_IsSetAttackMoveVec[NORMAL_ATTACK1_NUMBER] = false;
		}
		IsAction = true;
	}

	return IsAction;
}
//重力処理
void Player::GravityManager() {
	if (m_IsGravity) {
		//現在のY座標にジャンプ力を加算
		m_Pos.y += m_JumpPower;
		//重力方向に加算
		m_Gravity += GRAVITY;
		//重力速度を制限
		if (m_Gravity <= GRAVITY_MAX) {
			m_Gravity = GRAVITY_MAX;
		}
		if (m_JumpPower <= JUMP_POWER_MAX) {
			m_JumpPower = JUMP_POWER_MAX;
			//落下状態へ
			m_State = FALLING;
		}
		//ジャンプ力減衰
		m_JumpPower += m_Gravity;
	}
	else {
		//ジャンプ力リセット
		m_JumpPower = 0.0f;
	}
}
//ノックバックデータ数値代入
void Player::SetKnockBackData(VECTOR _Pos) {
	//スタミナ残量が少ないほどノックバック量が大きくる
	float Calc = (m_MaxHitPoints - m_Stamina) * 0.1f;
	m_KnockBackMaxDistance = 5.0f + Calc;
	if (m_IsGuardCollision) {
		m_KnockBackMaxDistance = m_KnockBackMaxDistance * GUARD_DAMAGE_TAKEN_MULT;
	}
	//ノックバック開始時の敵座標
	m_KnockBackStartPos = _Pos;
	//
	m_KnockBackState = KNOCKBACK_START;
	//重力処理オン
	m_IsGravity = true;
}
//アニメーションの硬直設定
void Player::SetAnimeRecoveryManager(int _RecoveryTime) {
	//硬直開始
	if (m_IsRecovery) {
		//硬直中に変更
		m_IsRecovery = false;
		//硬直時間を設定
		m_RecoveryTime = _RecoveryTime;
		//アニメーションを停止
		m_AnimeData.Speed = 0.0f;
	}
}
//アニメーションの硬直更新
bool Player::UpdateAnimeRecoveryManager(float _AnimeSpeed) {
	//硬直時間をカウントダウン
	m_RecoveryTime--;
	//硬直終了
	if (m_RecoveryTime <= 0) {
		//アニメーション速度を元に戻す
		m_AnimeData.Speed = _AnimeSpeed;
		//硬直終了
		m_IsRecovery = true;
	}
	return m_IsRecovery;
}
//ガードの成功判定を管理する
void Player::IsGuardSuccessManager() {
	//ガード成功判定がオフなら処理を行わない
	if (!m_IsGuardSuccess) return;
	//ガードに成功後の時間を加算
	m_GuardSuccessTime++;
	if (m_GuardSuccessTime > GUARD_SUCCESS_TIME) {
		//ガード成功判定をオフ
		m_IsGuardSuccess = false;
		//ガードに成功後の時間をリセット
		m_GuardSuccessTime = 0;
	}
}
//パリィの成功判定を管理する
void Player::IsParrySucessManager() {
	//パリィ成功判定がオフなら処理を行わない
	if (!m_IsParrySucess)return;
	//パリィに成功後の時間を加算
	m_ParrySuccessTime++;
	if (m_ParrySuccessTime > PARRY_SUCCESS_TIME) {
		//パリィ成功判定をオフ
		m_IsParrySucess = false;
		//パリィに成功後の時間をリセット
		m_ParrySuccessTime = 0;
		//当たり判定をオン
		m_IsCollision = true;
	}

}
