#pragma once
#include "Game/Base/Object/ActorBase/Character/CharacterBase.h"
#include "Lib/Input/InputKey.h"
#include "Lib/Input/InputPad.h"

//プレイヤークラス
class Player :public CharacterBase {
public:
	//プレイヤーの状態を管理するタグ
	enum TagState {
		WAIT,			//待機
		DAMAGE,			//ダメージ
		DEATH,			//死亡
		WALK,			//歩き
		ROLLING,		//ローリング
		JUMP,			//ジャンプ
		GUARD,			//ガード
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
		LOWERARM_RIGHT,		//左尺骨
		HAND_RIGHT,			//右指骨
		HANDEND_RIGHT,		//右手の先
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

	VECTOR		m_AttackTargetPos;							//攻撃サーチを行う物体の座標

	int			m_RollingTime;								//ローリング継続時間

	bool		m_IsAction[STATE_NUM];						//アクションフラグ
	bool		m_IsActionSuccess[STATE_NUM];				//アクション成功判定フラグ

	bool		m_IsNextNormalAttack[NORMAL_ATTACK_MAX];	//通常攻撃の次の段数に移行するか
	bool		m_IsAttackCollision;						//攻撃の当たり判定を発生させてよいか

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
	bool NormalMoveVec();
	//移動計算
	void MoveCalc();
	//スタミナ処理
	void StaminaManager();
	//状態遷移
	void StateManager();
	//動作管理
	void ActionManager();

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

	//Get
	//プレイヤータグ
	TagState	GetState() const { return m_State; }
	//アクションフラグ
	bool		GetIsAction(int _State)const { return m_IsAction[_State]; }
	//アクション成功判定フラグ
	bool		GetIsActionSuccess(int _State)const { return m_IsActionSuccess[_State]; }

	//Set
	// //カメラ回転率
	void	SetCameraRot(VECTOR _CameraRot) { m_CamraRot = _CameraRot; }
	//攻撃サーチを行う物体の座標
	void	SetAttackTargetPos(ActorBase& _ActorBase) { m_AttackTargetPos = _ActorBase.GetPos(); }
};
