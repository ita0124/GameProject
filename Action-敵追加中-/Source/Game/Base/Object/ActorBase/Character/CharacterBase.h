#pragma once
#include "Game/Base/Object/ActorBase/ActorBase.h"

//キャラクタークラスベース
class CharacterBase :public ActorBase {
protected:
	//ボーン情報関連変数構造体
	typedef struct {
		VECTOR	Pos;			//ボーン座標
		float	Rad;			//ボーン半径
		bool	IsCollision;	//ボーン当たり判定
		bool	IsAttackFlg;	//ボーン攻撃判定
	}FRAME_DATA;				//子クラスで各々が設定

	float	m_Power;			//攻撃力
	float	m_HitPoints;		//体力
	float   m_MaxHitPoints;		//最大体力
	float	m_Stamina;			//スタミナ
	float	m_MaxStamina;		//最大スタミナ
	int		m_SkillPoints;		//スキルポイント

	bool	m_IsStaminaRecover;	//スタミナを回復させてよいか

	VECTOR		m_KnockBackStartPos;						//ノックバック開始時の敵座標
	float		m_KnockBackDistance;						//現在のノックバック量
	float		m_KnockBackMaxDistance;						//最大ノックバック量
	float		m_KnockBackSub;								//1フレーム毎のノックバック力減衰量
	int			m_KnockBackDuration;						//ノックバック継続時間
	bool		m_IsKnockBackCalcStart;						//ノックバック計算を始めるフラグ
	bool		m_IsKnockBack;								//ノックバック中フラグ

	//回転値更新
	void UpdateRotation(VECTOR _MoveVec, float _RotSpeed);
	//ノックバック
	void KnockBackManager();
	//ノックバックデータ数値代入
	void SetKnockBackData(float _Power, VECTOR _Pos) {};
public:
	//コンストラクタ
	CharacterBase();
	//デストラクタ
	virtual ~CharacterBase();
	//初期化処理
	virtual void Init();

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
