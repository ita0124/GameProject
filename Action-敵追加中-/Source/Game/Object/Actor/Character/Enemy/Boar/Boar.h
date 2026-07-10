#pragma once
#include "Game/Base/Object/ActorBase/Character/EnemyBase/EnemyBase.h"

namespace {
	constexpr int PATTERN_INDEX = 5;
	constexpr int ATTACK_INDEX = 7;
}

//ボスクラス
class Boar :public EnemyBase {
public:
	//ボス１の状態を管理するタグ
	enum TagState {
		IDEL,						//待機
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
		ANIME_IDEL,						//待機
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
	enum FrameNumber {
		ARMATURE,				//アーマチュア
		//ここから骨
		ROOT,					//ルート
		HITPS,					//お尻
		SPINE,					//背骨
		CHEST,					//胸骨
		NECK,					//首
		HEAD001,				//頭１
		HEAD002,				//頭２
		HEAD003,				//頭３
		HEAD004,				//頭４
		HEAD004_END,			//鼻先
		FANG_BASE_LEFT,			//
		FANG001_LEFT,			//
		FANG002_LEFT,			//
		FANG002_END_LEFT,		//
		FANG_BASE_RIGHT,		//
		FANG001_RIGHT,			//
		FANG002_RIGHT,			//
		FANG002_END_RIGHT,		//
		FRONT_UPPER_LEG_LEFT,	//
		FRONT_LOWER_LEG_LEFT,	//
		FRONT_PAW_LEFT,			//
		FRONT_PAW_END_LEFT,		//
		FRONT_UPPER_LEG_RIGHT,	//
		FRONT_LOWER_LEG_RIGHT,	//
		FRONT_PAW_RIGHT,		//
		FRONT_PAW_END_RIGHT,	//
		PELVIS_LEFT,			//
		BACK_UPPER_LEG_LEFT,	//
		BACK_LOWER_LEG_LEFT,	//
		BACK_PAW_LEFT,			//
		BACK_PAW_END_LEFT,		//
		PELVIS_RIGHT,			//
		BACK_UPPER_LEG_RIGHT,	//
		BACK_LOWER_LEG_RIGHT,	//
		BACK_PAW_RIGHT,			//
		BACK_PAW_END_RIGHT,		//
		TAIL_BASE,				//
		TAIL001,				//
		TAIL002,				//
		TAIL003,				//
		TAIL004,				//
		TAIL005,				//
		TAIL005_END,			//
		//これ以下は使うことがないと思うので省略

		FRAME_NUM
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
	TagState	m_PrevState;										//１フレーム前の状態

	FRAME_DATA	m_FrameData[FRAME_NUM];								//ボスのボーン分の情報を格納

	int			m_DamageTime;										//ダメージ処理の継続時間

	VECTOR		m_BeforJumpPos;										//ジャンプ直前の座標を保存
	VECTOR		m_PredictedLandingPos;								//着地予定座標

	int			m_SpecialChargeTime;								//必殺チャージの継続時間

	int			m_DirectNum;										//どの方向判定ボーンと当たったかを保存する

	int			m_AttackPatterns[PATTERN_INDEX][ATTACK_INDEX];		//攻撃パターンの配列
	int			m_PatternIndex;										//攻撃パターンの種類
	int			m_AttackIndex;										//パターン内の攻撃順
	int			m_NextAttack;										//次に行う予定の攻撃

	//待機
	void Idel();
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
	void SetFrameDataIsCollision(int _FrameNamber, float _Rad);
	//指定のボーン当たり判定を削除する
	void DeleteFrameDataIsCollision(int _FrameNamber);
	//全てのボーン当たり判定を削除する
	void AllDeleteFrameDataIsCollision();
	//指定のボーン攻撃判定を設定
	void SetFrameDataIsAttackFlg(int _FrameNamber, float _Rad);
	//指定のボーン攻撃判定を削除する
	void DeleteFrameDataIsAttackFlg(int _FrameNamber);
	//全てのボーン攻撃判定を削除する
	void AllDeleteFrameDataIsAttackFlg();
public:
	//コンストラクタ
	Boar();
	//デストラクタ
	~Boar();
	//初期化処理
	void Init();
	//データ読み込み処理
	void Load();
	//毎フレーム呼び出す処理
	void Step();
	//当たり判定後の処理(当たっている場合)
	void HitCalc(ObjectBase* _Object);
	//方向判定ボーンと当たった場合
	void HitFrame(int _FrameNum);

	//Get
	//プレイヤータグ
	TagState	GetState() const { return m_State; }
	//指定のボーン座標を取得する
	VECTOR	GetFrameDataPos(int _Num) { return m_FrameData[_Num].Pos; }
	//指定のボーン半径を取得する
	float	GetFrameDataRad(int _Num) { return m_FrameData[_Num].Rad; }
	//指定のボーン当たり判定を取得する
	bool	GetFrameDataIsHitFlg(int _Num) { return m_FrameData[_Num].IsCollision; }
	//指定のボーン攻撃判定を取得する
	bool	GetFrameDataIsAttackFlg(int _Num) { return m_FrameData[_Num].IsAttackFlg; }
};
