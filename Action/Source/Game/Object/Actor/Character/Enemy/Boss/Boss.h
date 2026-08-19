#pragma once
#include "Game/Base/Object/ActorBase/Character/EnemyBase/EnemyBase.h"

namespace {
	constexpr int PATTERN_INDEX = 5;
	constexpr int ATTACK_INDEX = 7;
}

//ボスクラス
class Boss :public EnemyBase {
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
		SIDE_ATTACK,				//回転攻撃
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
		ANIME_SIDE_ATTACK,				//回転攻撃
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
		FANG_RIGHT,				//右牙の根本
		FANG001_RIGHT,			//右牙１
		FANG002_RIGHT,			//右牙２
		FANG003_RIGHT,			//右牙３
		FANG003END_RIGHT,		//右牙先
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
		LOWERLEG_LEFT,			//左脛骨
		TOES_LEFT,				//左中足骨
		TOESEND_LEFT,			//左指骨
		PELVIS_RIGHT,			//右骨盤
		UPPERLEG_RIGHT,			//右大腿骨
		LOWERLEG_RIGHT,			//右脛骨
		TOES_RIGHT,				//右足骨
		TOESEND_RIGHT,			//右指骨
		//方向判定ボーン
		FRONT,					//前方
		FRONT_END,				//前方-終-
		REAR,					//後方
		REAR_END,				//後方-終-
		RIGHT,					//右方
		RIGHT_END,				//右方-終-
		LEFT,					//左方
		LEFT_END,				//左方-終-
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

	int			m_DamageTime;										//ダメージ処理の継続時間
	bool		m_IsDamageTimeCalc;									//ダメージ処理の継続時間の計算をしてよいか

	VECTOR		m_BeforJumpPos;										//ジャンプ直前の座標を保存
	VECTOR		m_PredictedLandingPos;								//着地予定座標

	int			m_SpecialChargeTime;								//必殺チャージの継続時間

	int			m_DirectNum;										//どの方向判定ボーンと当たったかを保存する

	int			m_AttackPatterns[PATTERN_INDEX][ATTACK_INDEX];		//攻撃パターンの配列
	int			m_PatternIndex;										//攻撃パターンの種類
	int			m_AttackIndex;										//パターン内の攻撃順
	int			m_NextAttack;										//次に行う予定の攻撃

	bool		m_WasSpecialMove;									//必殺技を一度でも実行したか
	bool		m_IsCrystalRequest;									//クリスタルの出現を要求しているか
	bool		m_IsCrystalDeathRequest;							//クリスタルの死亡を要求しているか
	int			m_CrystalCount;										//現在出現しているクリスタルの数

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
	//回転攻撃
	void SideAttack();
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
	Boss();
	//デストラクタ
	~Boss();
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
	//クリスタルの出現を要求しているか
	bool	GetIsCrystalRequest() { return m_IsCrystalRequest; }
	//クリスタルの脂肪を要求しているか
	bool	GetIsCrystalDeathRequest() { return m_IsCrystalDeathRequest; }

	//Set
	//現在出現しているクリスタルの数
	void	SetCrystalCount(int _CrystalCount) { m_CrystalCount = _CrystalCount; }
};
