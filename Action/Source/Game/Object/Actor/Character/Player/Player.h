#pragma once
#include "Game/Base/Object/ActorBase/Character/CharacterBase.h"
#include "Lib/Input/InputKey.h"
#include "Lib/Input/InputPad.h"

//プレイヤークラス
class Player :public CharacterBase {
public:
	//プレイヤーの状態を管理するタグ
	enum TagState {
		T_PAUSE,		//Tポーズ
		WAIT,			//待機
		DAMAGE,			//ダメージ
		DEATH,			//死亡
		WALK,			//歩き
		ROLLING,		//ローリング
		JUMP,			//ジャンプ
		GUARD_STARA,	//ガード
		GUARD_WAIT,		//ガード
		GUARD_END,		//ガード
		PARRY,			//パリィ
		SKILL_ATTACK,	//スキル攻撃
		NORMAL_ATTACK1,	//通常攻撃１段目
		NORMAL_ATTACK2,	//通常攻撃２段目
		NORMAL_ATTACK3,	//通常攻撃３段目

		STATE_NUM
	};

	enum {
		NORMAL_ATTACK1_NUMBER,		//通常攻撃１段目
		NORMAL_ATTACK2_NUMBER,		//通常攻撃２段目
		NORMAL_ATTACK3_NUMBER,		//通常攻撃３段目

		NORMAL_ATTACK_MAX
	};

	//ボーン番号enum
	enum FrameNamber {
		ARMATURE,		//アーマチュア
		//ここからボーン
		HIP,				//お尻
		SPINE,				//脊髄
		CHEST,				//胸骨
		HEAD,				//頭蓋骨
		HEADEND,			//頭の頂点
		SHOULDER_LEFT,		//左鎖骨
		UPPERARM_LEFT,		//左上腕骨
		LOWERARM_LEFT,		//左尺骨
		HAND_LEFT,			//左中手骨
		HANDEND_LEFT,		//左指骨
		SHOULDER_RIGHT,		//右鎖骨
		UPPERARM_RIGHT,		//右上腕骨
		LOWERARM_RIGHT,		//右尺骨
		HAND_RIGHT,			//右中手骨
		HANDEND_RIGHT,		//右指骨
		PELVIS_LEFT,		//左骨盤
		UPPERLEG_LEFT,		//左大腿骨
		LOWERLRG_LEFT,		//左脛骨
		TOES_LEFT,			//左中足骨
		TOSEEND_LEFT,		//左指骨
		PELVIS_RIGHT,		//右骨盤
		UPPERLEG_RIGHT,		//右大腿骨
		LOWERLRG_RIGHT,		//右脛骨
		TOES_RIGHT,			//右足骨
		TOSEEND_RIGHT,		//右指骨
		//これ以下は使うことがないと思うので省略

		FARAM_NUM
	};

private:
	TagState	m_State;									//プレイヤー状態変数

	VECTOR		m_CamraRot;									//カメラの回転率を代入する

	int			m_RollingTime;								//ローリング継続時間

	bool		m_IsAction[STATE_NUM];						//アクションフラグ
	bool		m_IsActionSuccess[STATE_NUM];				//アクション成功判定フラグ
	int			m_ActionSuccessTime[STATE_NUM];				//アクション成功の継続時間

	bool		m_IsGuardCollision;							//ガードの当たり判定を発生させてよいか
	int			m_ParryWindoeTime;							//ガードアクション実行後のパリィに移行できる許容時間
	bool		m_IsParryWindo;								//パリィ許容フラグ

	float		m_JumpCalc;									//ジャンプ力計算

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
	float		m_TargetAngle;								//攻撃対象との角度差


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
	//通常移動方向更新
	bool UpdateNormalMoveVec();
	//通常移動計算
	void NormalMoveCalc();
	//攻撃移動方向更新
	bool UpdateAttackMoveVec(int _Index);
	//攻撃移動計算
	void AttackMoveCalc(int _Index);
	//スタミナ処理
	void StaminaManager();
	//状態遷移
	void StateManager();
	//動作管理
	void ActionManager();
	//重力処理
	void GravityManager();
	//アクションフラグをリセット
	void ResetIsAction();
	//アクション成功フラグ管理
	void ActionSuccessManager();
	//ノックバック
	void KnockBackManager();
	//ノックバックデータ数値代入
	void SetKnockBackData(float _Power,VECTOR _Pos);
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

	//Get
	//プレイヤータグ
	TagState	GetState() const { return m_State; }
	//アクションフラグ
	bool		GetIsAction(int _State)const { return m_IsAction[_State]; }
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

	//Set
	//カメラ回転率
	void	SetCameraRot(VECTOR _CameraRot) { m_CamraRot = _CameraRot; }
	//攻撃サーチを行う物体の座標
	void	SetAttackTargetPos(VECTOR _AttackTargetPos) { m_AttackTargetPos = _AttackTargetPos; }
};
