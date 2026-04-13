#pragma once
#include "Game/Base/Object/ActorBase/Character/EnemyBase/EnemyBase.h"

namespace {
	const int PATTERN_INDEX = 5;
	const int ATTACK_INDEX = 7;
}

//ボスクラス
class Boss :public EnemyBase {
public:
	//ボス１の状態を管理するタグ
	enum TagState {
		WAIT,						//待機
		DOWN,						//ダウン
		DEATH,						//死亡
		WALK,						//歩き
		BREAK_NORMAL_ATTACK1,		//通常攻撃１段目　攻撃終了(鼻)
		CHAIN_NORMAL_ATTACK1,		//通常攻撃１段目　攻撃継続(鼻)
		BREAK_NORMAL_ATTACK2,		//通常攻撃２段目　攻撃終了(牙振り上げ)
		CHAIN_NORMAL_ATTACK2,		//通常攻撃２段目　攻撃継続(牙振り上げ)
		BREAK_NORMAL_ATTACK3,		//通常攻撃３段目　攻撃終了(踏みつけ)
		REAR_ATTACK,				//後方攻撃
		JUMP,						//突進直前移動
		CHARGE_ATTACK_START,		//突進チャージ
		CHARGE,						//突進
		CHARGE_ATTACK,				//突進振り上げ
		SPECIAL_START,				//必殺開始
		SPECIAL_CHARGE,				//必殺チャージ
		SPECIAL,					//必殺

		STATE_NUM
	};

	//ボスのアニメーションを管理するタグ
	enum TagAnime {
		ANIME_WAIT,						//待機
		ANIME_DOWN,						//ダウン
		ANIME_DEATH,					//死亡
		ANIME_WALK,						//歩き
		ANIME_BREAK_NORMAL_ATTACK1,		//通常攻撃１段目　攻撃終了(鼻)
		ANIME_CHAIN_NORMAL_ATTACK1,		//通常攻撃１段目　攻撃継続(鼻)
		ANIME_BREAK_NORMAL_ATTACK2,		//通常攻撃２段目　攻撃終了(牙振り上げ)
		ANIME_CHAIN_NORMAL_ATTACK2,		//通常攻撃２段目　攻撃継続(牙振り上げ)
		ANIME_BREAK_NORMAL_ATTACK3,		//通常攻撃３段目　攻撃終了(踏みつけ)
		ANIME_REAR_ATTACK,				//後方攻撃
		ANIME_CHARGE_ATTACK_START,		//突進チャージ
		ANIME_CHARGE,					//突進
		ANIME_CHARGE_ATTACK,			//突進振り上げ
		ANIME_SPECIAL_START,			//必殺開始
		ANIME_SPECIAL_CHAGE,			//必殺チャージ

		ANIME_NUM
	};

	//ボーン番号enum
	enum FrameNamber {
		ARMATURE,				//アーマチュア
		//ここから骨
		HIP,					//お尻
		SPINE,					//脊髄
		CHEST,					//胸骨
		NECK,					//首
		HEAD,					//頭蓋骨
		NOSE,					//鼻１
		NOSE001,				//鼻２
		NOSE002,				//鼻３
		NOSE003,				//鼻４
		NOSE004,				//鼻５
		NOSE004END,				//鼻先
		FANG_LEFT,				//左牙の根本
		FANG001_LEFT,			//左牙１
		FANG002_LEFT,			//左牙２
		FANG003_LEFT,			//左牙３
		FANG003END_LEFT,		//左牙先
		FANG_RIGHT,				//左牙の根本
		FANG001_RIGHT,			//左牙１
		FANG002_RIGHT,			//左牙２
		FANG003_RIGHT,			//左牙３
		FANG003END_RIGHT,		//左牙先
		SHOULDER_LEFT,			//左鎖骨
		UPPERARM_LEFT,			//左上腕骨
		LOWERARM_LEFT,			//左尺骨
		PALM_LEFT,				//左中手骨
		PALMEND_LEFT,			//左指骨
		SHOULDER_RIGHT,			//右鎖骨
		UPPERARM_RIGHT,			//右上腕骨
		LOWERARM_RIGHT,			//右尺骨
		PALM_RIGHT,				//右中手骨
		PALMEND_RIGHT,			//右指骨
		PELVIS_LEFT,			//左骨盤
		UPPERLEG_LEFT,			//左大腿骨
		LOWERLRG_LEFT,			//左脛骨
		TOES_LEFT,				//左中足骨
		TOSEEND_LEFT,			//左指骨
		PELVIS_RIGHT,			//右骨盤
		UPPERLEG_RIGHT,			//右大腿骨
		LOWERLRG_RIGHT,			//右脛骨
		TOES_RIGHT,				//右足骨
		TOSEEND_RIGHT,			//右指骨
		FRONT,					//前方
		FRONT_END,				//前方-終-
		REAR,					//後方
		REAR_END,				//後方-終-
		RIGHT,					//右方
		RIGHT_END,				//右方-終-
		LEFT,					//左方
		LEFT_END,				//左方-終-
		//これ以下は使うことがないと思うので省略

		FARAM_NUM
	};

	//マテリアルenum
	enum Material {
		BODY,					//体
		OUTLINE,				//輪郭線
		FANG,					//牙

		MATERIAL_NUM
	};
private:
	TagState	m_State;											//ボス状態変数

	FRAME_DATA	m_FrameData[FARAM_NUM];								//ボスのボーン分の情報を格納

	bool		m_IsAction[STATE_NUM];								//アクションフラグ
	bool		m_IsActionSuccess[STATE_NUM];						//アクション成功判定フラグ

	bool		m_IsDamage;											//ダメージ処理中か
	int			m_DamageTime;										//ダメージ処理の継続時間

	VECTOR		m_BeforJumpPos;										//ジャンプ直前の座標を保存
	VECTOR		m_PredictedLandingPos;								//着地予定座標

	int			m_SpecialChargeTime;									//必殺チャージの継続時間

	int			m_AttackPatterns[PATTERN_INDEX][ATTACK_INDEX];		//攻撃パターンの配列
	int			m_PatternIndex;										//攻撃パターンの種類
	int			m_AttackIndex;										//パターン内の攻撃順
	int			m_NextAttack;										//次に行う予定の攻撃

	//待機
	void Wait();
	//ダメージ
	void Down();
	//死亡
	void Death();
	//歩き
	void Walk();
	//通常攻撃１段目　攻撃終了(鼻)
	void BreakNormalAttack1();
	//通常攻撃１段目　攻撃継続(鼻)
	void ChainNormalAttack1();
	//通常攻撃１段目共通処理
	void NoormalAttack1(TagState _State);
	//通常攻撃２段目　攻撃終了(牙振り上げ)
	void BreakNormalAttack2();
	//通常攻撃２段目　攻撃継続(牙振り上げ)
	void ChainNormalAttack2();
	//通常攻撃２段目共通処理
	void NoormalAttack2(TagState _State);
	//通常攻撃３段目　攻撃終了(踏みつけ)
	void BreakNormalAttack3();
	//後方攻撃
	void RearAttack();
	//突進直前移動
	void Jump();
	//突進チャージ
	void ChargeAttackStart();
	//突進
	void Charge();
	//突進振り上げ
	void ChargeAttack();
	//必殺開始
	void SpecialStart();
	//必殺チャージ
	void SpecialCharge();
	//必殺チャージ
	void Special();
	//行動管理
	void ActionManager();
	//攻撃パターン管理
	void AttackPatternManager();
	//ジャンプ時の着地地点管理
	void JumpPosManger();
	//状態遷移
	void StateManager();
	//指定のボーン当たり判定を設定設定
	void SetFrameData(int _FrameNamber, float _Rad);
	//指定のボーン当たり判定を削除する
	void DeleteFrameDataIsHitFlgFalse(int _FrameNamber);
	//全てのボーン当たり判定を削除する
	void AllDeleteFrameDataIsHitFlgFalse();
	//指定のボーン攻撃判定を設定
	void SetFrameDataIsAttackFlg(int _FrameNamber, float _Rad);
	//指定のボーン攻撃判定を削除する
	void DeleteFrameDataIsHitFlg(int _FrameNamber);
	//全てのボーン攻撃判定を削除する
	void AllDeleteFrameDataIsHitFlg();
public:
	//コンストラクタ
	Boss();
	//デストラクタ
	~Boss();
	//初期化処理
	void Init();
	//データ読み込み処理
	void Load();
	//毎フレーム呼び出す処理
	void Step();

	//Get
	//プレイヤータグ
	TagState	GetState() const { return m_State; }
};
