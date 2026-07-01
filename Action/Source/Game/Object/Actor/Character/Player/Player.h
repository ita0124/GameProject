#pragma once
#include "Game/Base/Object/ActorBase/Character/CharacterBase.h"
#include "Lib/Input/InputKey.h"
#include "Lib/Input/InputPad.h"

//プレイヤークラス
class Player :public CharacterBase {
public:
	//プレイヤーの状態を管理するタグ
	enum TagState {
		T_PAUSE,			//Tポーズ
		WAIT,				//待機
		DAMAGE,				//ダメージ
		DEATH,				//死亡
		WALK,				//歩き
		ROLLING,			//ローリング
		JUMP,				//ジャンプ
		GUARD,				//ガード
		PARRY,				//パリィ
		SKILL_ATTACK,		//スキル攻撃
		NORMAL_ATTACK1,		//通常攻撃１段目
		NORMAL_ATTACK2,		//通常攻撃２段目
		NORMAL_ATTACK3,		//通常攻撃３段目

		STATE_NUM
	};

	//プレイヤーのアニメーションを管理するタグ
	enum TagAnime {
		ANIME_T_POSE,			//Tポーズ
		ANIME_IDEL,				//待機
		ANIME_DAMAGE,			//ダメージ
		ANIME_DEATH,			//死亡
		ANIME_RUN,				//走り
		ANIME_ROLLING,			//ローリング
		ANIME_JUMP,
		ANIME_GUARD_START,		//ガード開始
		ANIME_GUARD_IDEL,		//ガード待機
		ANIME_GUARD_EMD,		//ガード終了
		ANIME_PARRY,			//パリィ
		ANIME_SKILL_ATTACK,		//スキル攻撃
		ANIME_NORMAL_ATTACK1,	//通常攻撃1段目
		ANIME_NORMAL_ATTACK2,	//通常攻撃2段目
		ANIME_NORMAL_ATTACK3,	//通常攻撃3段目

		ANIME_NUM
	};

	enum {
		NORMAL_ATTACK1_NUMBER,		//通常攻撃１段目
		NORMAL_ATTACK2_NUMBER,		//通常攻撃２段目
		NORMAL_ATTACK3_NUMBER,		//通常攻撃３段目

		NORMAL_ATTACK_MAX
	};

	//ボーン番号enum
	enum FrameNumber {
		ARMATURE,				// アーマチュア
		//ここからボーン
		HIPS,					// 腰
		SPINE,					// 背骨下
		SPINE1,					// 背骨中
		SPINE2,					// 背骨上
		NECK,					// 首
		HEAD,					// 頭
		HEADTOP,				// 頭頂
		HEADTOP_END,			// 頭頂先端

		LEFT_SHOULDER,			// 左肩
		LEFT_UPPER_ARM,			// 左上腕
		LEFT_LOWER_ARM,			// 左前腕
		LEFT_HAND1,				// 左手ボーン1
		LEFT_HAND2,				// 左手ボーン2
		LEFT_HAND3,				// 左手ボーン3
		LEFT_HAND4,				// 左手ボーン4
		LEFT_HAND_END,			// 左手先

		SHIELD_SOCKET,			// 盾ソケット
		SHIELD_SOCKET_END,		// 盾ソケット先端

		RIGHT_SHOULDER,			// 右肩
		RIGHT_UPPER_ARM,		// 右上腕
		RIGHT_LOWER_ARM,		// 右前腕
		RIGHT_HAND1,			// 右手ボーン1
		RIGHT_HAND2,			// 右手ボーン2
		RIGHT_HAND3,			// 右手ボーン3
		RIGHT_HAND4,			// 右手ボーン4
		RIGHT_HAND5,			// 右手ボーン5
		RIGHT_HAND_END,			// 右手先

		LEFT_UPPER_LEG,			// 左太腿
		LEFT_LOWER_LEG,			// 左脛
		LEFT_FOOT,				// 左足首
		LEFT_TOE,				// 左つま先
		LEFT_TOE1,				// 左足指先
		LEFT_TOE1_END,			// 左足指先端

		RIGHT_UPPER_LEG,		// 右太腿
		RIGHT_LOWER_LEG,		// 右脛
		RIGHT_FOOT,				// 右足首
		RIGHT_TOE,				// 右つま先
		RIGHT_TOE1,				// 右足指先
		RIGHT_TOE1_END,			// 右足指先端
		//これ以下は使うことがないと思うので省略

		FRAME_NUM
	};

private:
	TagState	m_State;									//プレイヤー状態変数
	TagState	m_PrevState;								//１フレーム前の状態

	VECTOR		m_CamraRot;									//カメラの回転率を代入する

	int			m_RollingTime;								//ローリング継続時間

	bool		m_IsActionSuccess[STATE_NUM];				//アクション成功判定フラグ
	int			m_ActionSuccessTime[STATE_NUM];				//アクション成功の継続時間

	bool		m_IsGuardCollision;							//ガードの当たり判定を発生させてよいか
	int			m_ParryWindoeTime;							//ガードアクション実行後のパリィに移行できる許容時間
	bool		m_IsParryWindo;								//パリィ許容フラグ

	float		m_JumpPower;								//ジャンプ力計算

	VECTOR		m_KnockBackStartPos;						//ノックバック開始時の敵座標
	float		m_KnockBackDistance;						//現在のノックバック量
	float		m_KnockBackMaxDistance;						//最大ノックバック量
	float		m_KnockBackSub;								//1フレーム毎のノックバック力減衰量
	int			m_KnockBackDuration;						//ノックバック継続時間
	bool		m_IsKnockBackCalcStart;						//ノックバック計算を始めるフラグ
	bool		m_IsKnockBack;								//ノックバック中フラグ

	bool		m_IsNextNormalAttack[NORMAL_ATTACK_MAX];	//通常攻撃の次の段数に移行するか
	bool		m_IsAttackCollision;						//攻撃の当たり判定を発生させてよいか
	VECTOR		m_AttackTargetPos;							//攻撃対象の座標
	VECTOR		m_AttackMoveVec[NORMAL_ATTACK_MAX];			//攻撃進行方向
	float		m_AttackRot[NORMAL_ATTACK_MAX];				//攻撃方向角度
	bool		m_IsSetAttackMoveVec[NORMAL_ATTACK_MAX];	//攻撃信仰方向を設定したか
	float		m_TargetAngle;								//攻撃対象との角度差

	bool		m_IsRespawn;								//リスポーン中か

	int			m_RecoveryTime;								//行動可能になるまでの硬直時間
	bool		m_IsRecovery;								//硬直中かどうか
	bool		m_IsSetRecovery;							//硬直を開始したか

	//待機
	void Wait();
	//ダメージ食らい
	void Damage();
	//死亡
	void Death();
	//歩き
	void Walk();
	//ローリング
	void Rolling();
	//ジャンプ
	void Jump();
	//ガード
	void Guard();
	//パリィ
	void Parry();
	//スキル攻撃
	void SkillAttack();
	//通常攻撃１段目
	void NormalAttack1();
	//通常攻撃２段目
	void NormalAttack2();
	//通常攻撃３段目
	void NormalAttack3();
	//通常移動方向設定
	bool SetNormalMoveVec();
	//通常移動計算
	void NormalMoveCalc();
	//攻撃移動方向設定
	bool SetAttackMoveVec(int _Index);
	//攻撃移動方向更新
	bool UpdateAttackMoveVec(int _Index);
	//攻撃移動計算
	void AttackMoveCalc(int _Index);
	//スタミナ処理
	void StaminaManager();
	//状態遷移
	void StateManager();
	//動作管理
	bool ActionManager();
	//重力処理
	void GravityManager();
	//アクション成功フラグ管理
	void ActionSuccessManager();
	//ノックバック
	void KnockBackManager();
	//ノックバックデータ数値代入
	void SetKnockBackData(float _Power, VECTOR _Pos);
	//アニメーションの硬直設定
	void SetAnimeRecoveryManager(int _RecoveryTime);
	//アニメーションの硬直更新
	bool UpdateAnimeRecoveryManager(float _AnimeSpeed);
public:
	//コンストラクタ
	Player();
	//デストラクタ
	~Player();
	//初期化処理
	void Init();
	//データ読み込み処理
	void Load();
	//毎フレーム呼び出す処理
	void Step();
	//当たり判定後の処理(当たっている場合)
	void HitCalc(ObjectBase* _Object);
	//リスポーン処理
	void Respawn();

	//Get
	//プレイヤータグ
	TagState	GetState() const { return m_State; }
	//アクション成功判定フラグ
	bool		GetIsActionSuccess(int _State)const { return m_IsActionSuccess[_State]; }
	//攻撃の当たり判定を発生させてよいか
	bool		GetIsAttackCollision() { return m_IsAttackCollision; }
	//ガードの当たり判定を発生させてよいか
	bool		GetIsGuardCollision() { return m_IsGuardCollision; }
	//パリィ許容フラグ
	bool		GetIsParryWindo() { return m_IsParryWindo; }
	//攻撃サーチを行う物体の座標
	VECTOR		GetAttackTargetPos() { return m_AttackTargetPos; }
	//リスポーン中か
	bool		GetIsRespawn() { return m_IsRespawn; }

	//Set
	//カメラ回転率
	void	SetCameraRot(VECTOR _CameraRot) { m_CamraRot = _CameraRot; }
	//攻撃サーチを行う物体の座標
	void	SetAttackTargetPos(VECTOR _AttackTargetPos) { m_AttackTargetPos = _AttackTargetPos; }
	//リスポーン中か
	void	SetIsRespawn(bool _IsRespawn) { m_IsRespawn = _IsRespawn; }
};
