#pragma once
#include "Game/Base/Object/ActorBase/Character/EnemyBase/MobEnemyBase/MobEnemyBase.h"

//イノシシクラス
class Boar :public MobEnemyBase {
public:
	//ボス１の状態を管理するタグ
	enum TagState {
		IDEL,					//待機
		WALK,					//歩き
		ATTACK_IDEL,			//攻撃待機
		ATTACK,					//攻撃
		DOWN,					//ダウン
		DEATH,					//死亡
		DAMAGE,					//ダメージ

		STATE_NUM
	};

	//ボスのアニメーションを管理するタグ
	enum TagAnime {
		ANIME_IDEL,				//待機
		ANIME_WALK,				//歩き
		ANIME_ATTACK_IDEL,		//攻撃待機
		ANIME_ATTACK,			//攻撃
		ANIME_DOWN,				//ダウン
		ANIME_DEATH,			//死亡
		ANIME_DAMAGE,			//ダメージ

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
		HEAD,					//頭
		NOSE,					//鼻
		NOSE_END,				//鼻先
		FANG_BASE_LEFT,			//左牙の根元
		FANG001_LEFT,			//左牙１
		FANG002_LEFT,			//左牙２
		FANG002_END_LEFT,		//左牙先
		FANG_BASE_RIGHT,		//右牙の根元
		FANG001_RIGHT,			//右牙１
		FANG002_RIGHT,			//右牙２
		FANG002_END_RIGHT,		//右牙先
		SHOULDER_LEFT,			//左肩
		FRONT_UPPER_LEG_LEFT,	//左前脚上部
		FRONT_LOWER_LEG_LEFT,	//左前脚下部
		FRONT_PAW_LEFT,			//左前足
		FRONT_PAW_END_LEFT,		//左前足先
		SHOULDER_RIGHT,			//右肩
		FRONT_UPPER_LEG_RIGHT,	//右前脚上部
		FRONT_LOWER_LEG_RIGHT,	//右前脚下部
		FRONT_PAW_RIGHT,		//右前足
		FRONT_PAW_END_RIGHT,	//右前足先
		PELVIS_LEFT,			//左骨盤
		BACK_UPPER_LEG_LEFT,	//左後脚上部
		BACK_LOWER_LEG_LEFT,	//左後脚下部
		BACK_PAW_LEFT,			//左後足
		BACK_PAW_END_LEFT,		//左後足先
		PELVIS_RIGHT,			//右骨盤
		BACK_UPPER_LEG_RIGHT,	//右後脚上部
		BACK_LOWER_LEG_RIGHT,	//右後脚下部
		BACK_PAW_RIGHT,			//右後足
		BACK_PAW_END_RIGHT,		//右後足先
		TAIL_BASE,				//尻尾の根元
		TAIL001,				//尻尾１
		TAIL002,				//尻尾２
		TAIL003,				//尻尾３
		TAIL004,				//尻尾４
		TAIL005,				//尻尾５
		TAIL005_END,			//尻尾先
		//これ以下は使うことがないと思うので省略

		FRAME_NUM
	};

	//マテリアルenum
	enum Material {
		BODY,					//体
		FANG,					//牙

		MATERIAL_NUM
	};
private:
	TagState	m_State;											//ボス状態変数
	TagState	m_PrevState;										//１フレーム前の状態

	int			m_DamageTime;										//ダメージ処理の継続時間

	int			m_AttackTime;										//攻撃継続時間

	int			m_DeathIdelTime;									//死亡待機時間

	//待機
	void Idel();
	//歩き
	void Walk();
	//攻撃待機
	void AttackIdel();
	//攻撃
	void Attack();
	//ダウン
	void Down();
	//死亡
	void Death();
	//ダメージ
	void Damage();
	//行動管理
	void ActionManager();
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
