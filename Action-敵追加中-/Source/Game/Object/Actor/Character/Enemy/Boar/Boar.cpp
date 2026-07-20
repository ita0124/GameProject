#include "Boar.h"
#include "Game/Object/Actor/Character/Player/Player.h"

namespace {
	constexpr VECTOR	INIT_POS = { -20.0f,0.0f,-80.0f };												//初期座標

	constexpr float		RAD = 5.0f;																		//半径
	constexpr VECTOR	BOSS_SIZE = { RAD,RAD,RAD };													//ボックス当たり判定

	constexpr float		HIT_POINTS = 50.0f;																//体力
	constexpr float		MAX_HITPOINTS = 50.0f;															//最大体力

	constexpr float		ACTION_IDEL_DISTANCE = 300.0f;													//IDELに移行するプレイヤーとの距離
	constexpr float		ACTION_ATTACK_DISTANCE = 200.0f;												//攻撃に移行するプレイヤーとの距離

	constexpr float		WALK_MULT = 2.0f;																//歩き時の移動乗算値
	constexpr float		CHARGE_MULT = 3.0f;																//突進の移動乗算値

	constexpr float		CHARGE_END_DISTANCE = 50.0f;													//突進の追尾を終了する距離

	constexpr int		CHARGE_END_TIME = 60;															//接近後に突進を終了するまでの時間

	constexpr int		NEXT_ACTION_WAIT_TIME = 60;														//次の行動までの待機時間

	constexpr float		NORMAL_MOVE_ROTATE_SPEED = 0.25f;												//通常移動時の回転速度

	constexpr float		CHARGE_CHANGE_MATERIAL_START_LEN = 500.0f;										//突進のマテリアル変更最低距離

	constexpr float		NORMAL_ATTACK1_COLLISION_RAD = 15.0f;											//通常攻撃１段目の攻撃当たり判定の半径

	constexpr float		ANIME_SPEED = 0.35f;															//アニメーション再生スピード

	constexpr float		NORMAL_ATTACK1_POWER = 10.0f;													//通常攻撃１段目時の攻撃力

	constexpr int		DAMAGE_TIME = 20;																//ダメージ状態の継続時間

	constexpr float		DOWN_DAMAGE_TAKEN_MULT = 1.5f;													//ダウン時の被ダメ増加量

	constexpr float		PARRY_DOWN_POWER_THRESHOLD = 5.0f;												//パリィされたときにダウンへ移行する攻撃力
	constexpr float		PARRY_DOWN_TIME_MULT = 3.0f;													//パリィされたときに攻撃力に乗算してダウン時間を設定する

	constexpr char		MODEL_FILE_PATH[] = ("Data/Model/Enemy/Boar/Boar.mv1");							//モデルファイルパス
}

//コンストラクタ
Boar::Boar() {
	Init();
}
//デストラクタ
Boar::~Boar() {
	Exit();
}
//初期化処理
void Boar::Init() {
	EnemyBase::Init();

	m_Kinds = ENEMY;

	m_Pos = INIT_POS;									//座標
	m_Rad = RAD;										//半径
	m_Size = BOSS_SIZE;									//ボックス当たり判定

	m_HitPoints = HIT_POINTS;							//体力

	m_State = IDEL;										//ボス状態変数
	m_PrevState = m_State;								//１フレーム前の状態
	m_DamageTime = 0;									//ダメージ処理の継続時間

	m_IsClose = false;									//近いかどうか
	m_CloseTime = 0;									//近い状態になってからの経過時間

	for (int Index = 0; Index < FRAME_NUM; Index++) {
		m_FrameData[Index].Pos = VZERO;					//ボーン座標
		m_FrameData[Index].Rad = 0.0f;					//ボーン半径
		m_FrameData[Index].IsCollision = false;			//ボーン当たり判定
		m_FrameData[Index].IsAttackFlg = false;			//ボーン攻撃判定
	}
}
//データ読み込み処理
void Boar::Load() {
	ObjectBase::Load(MODEL_FILE_PATH);
}
//毎フレーム呼び出す処理
void Boar::Step() {
	m_PrevPos = m_Pos;

	if (m_PlatformVec.x != 0.0f || m_PlatformVec.y != 0.0f || m_PlatformVec.z != 0.0f) {
		m_Pos = VAdd(m_Pos, m_PlatformVec);
	}
	m_PlatformVec = VZERO;

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
	//状態遷移
	StateManager();
	//重力処理
	GravityManager();

#ifdef _DEBUG
	DrawFormatStringToHandle(50, 420, RED, DxLibFont::FONTHNDL_N20, "攻撃力:%.0f", m_Power);
	DrawFormatStringToHandle(50, 500, RED, DxLibFont::FONTHNDL_N20, "今の攻撃種:%d", m_State);
	DrawFormatStringToHandle(50, 540, RED, DxLibFont::FONTHNDL_N20, "体力:%.0f", m_HitPoints);
	DrawFormatStringToHandle(50, 560, RED, DxLibFont::FONTHNDL_N20, "%d", m_IsCollision);
#endif // DEBUG
}
//当たり判定後の処理(当たっている場合)
void Boar::HitCalc(ObjectBase* _Object) {
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
		//ダウン状態の時
		if (m_State == DOWN) {
			//プレイヤーの攻撃力に被ダメ率を乗算後HPを消費
			m_HitPoints = m_HitPoints - (PointerPlayer->GetPower() * DOWN_DAMAGE_TAKEN_MULT);
		}
		else {
			//HPを消費
			m_HitPoints = m_HitPoints - PointerPlayer->GetPower();
		}
		//ダメージ処理の継続時間セット
		m_DamageTime = DAMAGE_TIME;
		//当たり判定オフ
		m_IsCollision = false;
	}
}
//待機
void Boar::Idel() {
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
//歩き
void Boar::Walk() {
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
		//移動方向を向く
		UpdateRotation(DirToPlayer, NORMAL_MOVE_ROTATE_SPEED);
	}
}
//攻撃
void Boar::Attack() {
	//突進アニメーションループ再生
	RequestLoop(ANIME_ATTACK);
	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//接近判定をオフ
		m_IsClose = false;
		//接近後の経過フレームを初期化
		m_CloseTime = 0;
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
	if (!m_IsClose)
	{
		//方向ベクトルを取得
		VECTOR DirToPlayer = GetDirectionNotY(m_Pos, m_PlayerPos);
		DirToPlayer.y = 0.0f;
		//サイズ取得
		float Len = VSize(DirToPlayer);

		// 十分近づいたら追尾終了
		if (Len < CHARGE_END_DISTANCE)
		{
			m_IsClose = true;
			// 最後の方向を保存
			m_MoveVec = VNorm(DirToPlayer);
		}
		else
		{
			m_MoveVec = VNorm(DirToPlayer);
		}
		//1フレームで移動する距離を生成
		m_MoveVec = VScale(m_MoveVec, CHARGE_MULT);
	}
	else {
		m_CloseTime++;
	}
	//座標に加算
	m_Pos = VAdd(m_Pos, m_MoveVec);
	//移動方向を向く
	UpdateRotation(m_MoveVec, NORMAL_MOVE_ROTATE_SPEED);
	//一定時間経過したら突進終了
	if (m_CloseTime > CHARGE_END_TIME) {
		//待機状態へ
		m_State = IDEL;
		//次の行動までの待機時間を設定
		m_NextActionTime = NEXT_ACTION_WAIT_TIME;
	}
}
//ダウン
void Boar::Down() {
	//ダウンアニメーションループ再生
	RequestLoop(ANIME_DOWN);

	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//輪郭線のマテリアルをマテリアル青に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::MATERIAL_BLUE, FALSE);
		//全てのボーン攻撃判定を削除する
		AllDeleteFrameDataIsAttackFlg();
	}
	if (m_DownTime <= 0) {
		//待機状態へ
		m_State = IDEL;
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::MATERIAL_BLACK, FALSE);
	}
	else {
		//ダウン状態継続時間を減算
		m_DownTime--;
	}
}
//死亡
void Boar::Death() {
	//死亡アニメーション再生
	RequestEndLoop(ANIME_DEATH, ANIME_SPEED);

	//１フレーム前の状態と今のフレームの状態を比較
	if (m_State != m_PrevState) {
		//変更があった
		m_PrevState = m_State;
		//輪郭線のマテリアルをマテリアル黒に変更
		MV1SetTextureGraphHandle(m_Hndl, OUTLINE, LoadMaterial::MATERIAL_BLACK, FALSE);
		//全てのボーン攻撃判定を削除する
		AllDeleteFrameDataIsAttackFlg();
	}
	//アニメーションが終わったら
	if (m_AnimeData.EndFlg) {
		//生存フラグをオフ
		m_IsActive = false;
	}
}
//行動管理
void Boar::ActionManager() {
	VECTOR DistanceToPlayer = GetDirectionNotY(m_Pos, m_PlayerPos);
	float ToPlayerLen = VSize(DistanceToPlayer);

	if (ToPlayerLen > ACTION_IDEL_DISTANCE) {
		//待機状態へ
		m_State = IDEL;
		return;
	}
	if (ToPlayerLen <= ACTION_ATTACK_DISTANCE) {
		//攻撃へ
		m_State = ATTACK;
		return;
	}
}
//状態遷移
void Boar::StateManager() {
	switch (m_State) {
	case IDEL:						//待機
		Idel();
		break;
		break;
	case WALK:						//歩き
		Walk();
		break;
	case ATTACK:					//攻撃
		Attack();
		break;
	case DOWN:						//ダウン
		Down();
		break;
	case DEATH:						//死亡
		Death();
		break;
	}
#ifdef _DEBUG
	DrawFormatStringToHandle(50, 400, RED, DxLibFont::FONTHNDL_N20, "%d", (int)m_State);
#endif // DEBUG
}
//当たり判定設定
void Boar::SetFrameDataIsCollision(int _FrameNamber, float _Rad) {
	m_FrameData[_FrameNamber].Pos = GetFramePos(m_Hndl, _FrameNamber);
	m_FrameData[_FrameNamber].Rad = _Rad;
	m_FrameData[_FrameNamber].IsCollision = true;
}
//指定のボーン当たり判定を削除する
void Boar::DeleteFrameDataIsCollision(int _FrameNamber) {
	m_FrameData[_FrameNamber].Pos = VZERO;
	m_FrameData[_FrameNamber].Rad = 0.0f;
	m_FrameData[_FrameNamber].IsCollision = false;
}
//全てのボーン当たり判定を削除する
void Boar::AllDeleteFrameDataIsCollision() {
	for (int Index = 0; Index < FRAME_NUM; Index++) {
		//指定のボーン攻撃判定を削除する
		DeleteFrameDataIsCollision(Index);
	}
}
//指定のボーン攻撃判定を設定
void Boar::SetFrameDataIsAttackFlg(int _FrameNamber, float _Rad) {
	m_FrameData[_FrameNamber].Pos = GetFramePos(m_Hndl, _FrameNamber);
	m_FrameData[_FrameNamber].Rad = _Rad;
	m_FrameData[_FrameNamber].IsAttackFlg = true;
}
//指定のボーン攻撃判定を削除する
void Boar::DeleteFrameDataIsAttackFlg(int _FrameNamber) {
	m_FrameData[_FrameNamber].Pos = VZERO;
	m_FrameData[_FrameNamber].Rad = 0.0f;
	m_FrameData[_FrameNamber].IsAttackFlg = false;
}
//全てのボーン攻撃判定を削除する
void Boar::AllDeleteFrameDataIsAttackFlg() {
	for (int Index = 0; Index < FRAME_NUM; Index++) {
		//指定のボーン攻撃判定を削除する
		DeleteFrameDataIsAttackFlg(Index);
	}
}
