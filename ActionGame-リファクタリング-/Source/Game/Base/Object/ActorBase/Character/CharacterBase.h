#pragma once
#include "Game/Base/Object/ActorBase/ActorBase.h"

//キャラクタークラスベース
class CharacterBase :public ActorBase {
protected:
	//ボーン情報関連変数構造体
	typedef struct {
		VECTOR	Pos;			//ボーン座標
		float	Rad;			//ボーン半径
		bool	IsHitFlg;		//ボーン当たり判定
		bool	IsAttackFlg;	//ボーン攻撃判定
	}FRAME_DATA;				//子クラスで各々が設定

	float	m_Power;			//攻撃力
	float	m_HitPoints;		//体力
	float	m_Stamina;			//スタミナ
	int		m_SkillPoints;		//スキルポイント

	bool	m_IsStaminaRecover;	//スタミナを回復させてよいか



public:
	//コンストラクタ
	CharacterBase();
	//デストラクタ
	virtual ~CharacterBase();
	//初期化処理
	virtual void Init();

	//継承先で使う
	//データ読み込み処理
	virtual void Load() = 0;
	//毎フレーム呼び出す処理
	virtual void Step() = 0;

	//Get
	//攻撃力
	float	GetPower()	const { return m_Power; }
	//体力
	float	GetHitPoints()	const { return m_HitPoints; }
	//スタミナ
	float	GetStamina()	const { return m_Stamina; }
	//スキルポイント
	int	GetSkillPoints()	const { return m_SkillPoints; }

	//Set
	//攻撃力
	void	SetPower(float _Power) { m_Power = _Power; }
	//体力
	void	SetHitPoints(float _HitPoints) { m_HitPoints = _HitPoints; }
	//スタミナ
	void	SetStamina(float _Stamina) { m_Stamina = _Stamina; }
	//スキルポイント
	void	SkillPoints(int _SkillPoints) { m_SkillPoints = _SkillPoints; }
};
