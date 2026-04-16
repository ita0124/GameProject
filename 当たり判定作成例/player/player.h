#pragma once
#include "../character/characterBase.h"
#include "../shot/shotManager.h"
#include"../../lib/input/controllerManager.h"
#include "playerData.h"

constexpr int TRANSFORM_TIME = 10 * 60;					//変身している時間

//武器のID
enum tagWeaponId
{
	WEAPON_ID_HAND,		//素手
	WEAPON_ID_HAMMER,	//ハンマー
	WEAPON_ID_AX,		//斧
	WEAPON_ID_GUN,		//銃

	WEAPON_ID_NUM,		//武器の種類
};

//アイテムの状態
enum tagHaveItemState
{
	ITEM_STATE_NONE = -1,		//アイテムを持っていない
	ITEM_STATE_PICK_UP,			//アイテムを取ろうとしている
	ITEM_STATE_PUT_DOWN,		//アイテムを下ろす
	ITEM_STATE_GET,				//アイテムを手に入れる
	ITEM_STATE_HAVE,			//アイテムを持っている
	ITEM_STATE_THROW,			//アイテムを投げる
	ITEM_STATE_DROP,			//アイテムを落とした
};

//プレイヤークラス
class CPlayer:public CCharacterBase
{
protected:
	int m_dropCoin;					//コインを落とす数
	bool m_isJump;					//ジャンプしているかフラグ
	int m_attackNum;				//攻撃の番号
	int m_weaponDurability;			//武器の耐久度
	int m_money;					//お金
	int m_attackId;					//攻撃のID
	int m_effectId;					//エフェクトのID
	tagPadName m_padName;			//コントローラーの名前
	tagWeaponId m_weaponId;			//武器のID
	tagPlayerName m_name;			//プレイヤーの名前
	tagHaveItemState m_itemState;	//アイテムの状態
	VECTOR* m_targetPos;			//相手の座標ポインタ
	bool m_isCpu;					//cpuかどうかフラグ
	int m_getUpTime;				//起き上がるまでの時間
	//アニメーション一覧---------------------------

	enum tagAnim {
		ANIMID_AIR,						//空中にいるときのアニメーション
		ANIMID_AIR_ATTACK_HAMMER,		//空中でハンマー攻撃中アニメーション
		ANIMID_AIR_ATTACK_HAMMER_IN,	//空中でハンマー攻撃前アニメーション
		ANIMID_AIR_ATTACK_HAMMER_OUT,	//空中でハンマー攻撃後アニメーション
		ANIMID_AIR_ATTACK_HAND,			//空中で素手攻撃中アニメーション
		ANIMID_AIR_ATTACK_HAND_IN,		//空中で素手攻撃前アニメーション
		ANIMID_AIR_ATTACK_HAND_OUT,		//空中で素手攻撃後アニメーション
		ANIMID_ATTACK1_AX,				//斧攻撃1中アニメーション
		ANIMID_ATTACK1_AX_IN,			//斧攻撃1前アニメーション
		ANIMID_ATTACK1_AX_OUT,			//斧攻撃1後アニメーション
		ANIMID_ATTACK2_AX,				//斧攻撃2中アニメーション
		ANIMID_ATTACK2_AX_IN,			//斧攻撃2前アニメーション
		ANIMID_ATTACK2_AX_OUT,			//斧攻撃2後アニメーション
		ANIMID_ATTACK3_AX,				//斧攻撃3中アニメーション
		ANIMID_ATTACK3_AX_IN,			//斧攻撃3前アニメーション
		ANIMID_ATTACK3_AX_OUT,			//斧攻撃3後アニメーション
		ANIMID_ATTACK1_GUN,				//銃攻撃1中アニメーション
		ANIMID_ATTACK1_GUN_IN,			//銃攻撃1前アニメーション
		ANIMID_ATTACK1_GUN_OUT,			//銃攻撃1後アニメーション
		ANIMID_ATTACK1_HAMMER,			//ハンマー攻撃1中アニメーション
		ANIMID_ATTACK1_HAMMER_IN,		//ハンマー攻撃1前アニメーション
		ANIMID_ATTACK1_HAMMER_OUT,		//ハンマー攻撃1後アニメーション
		ANIMID_ATTACK2_HAMMER,			//ハンマー攻撃2中アニメーション
		ANIMID_ATTACK2_HAMMER_IN,		//ハンマー攻撃2前アニメーション
		ANIMID_ATTACK2_HAMMER_OUT,		//ハンマー攻撃2後アニメーション
		ANIMID_ATTACK3_HAMMER,			//ハンマー攻撃3中アニメーション
		ANIMID_ATTACK3_HAMMER_IN,		//ハンマー攻撃3前アニメーション
		ANIMID_ATTACK3_HAMMER_OUT,		//ハンマー攻撃3後アニメーション
		ANIMID_ATTACK1_HAND,			//素手攻撃1中アニメーション
		ANIMID_ATTACK1_HAND_IN,			//素手攻撃1前アニメーション
		ANIMID_ATTACK1_HAND_OUT,		//素手攻撃1後アニメーション
		ANIMID_ATTACK2_HAND,			//素手攻撃2中アニメーション
		ANIMID_ATTACK2_HAND_IN,			//素手攻撃2前アニメーション
		ANIMID_ATTACK2_HAND_OUT,		//素手攻撃2後アニメーション
		ANIMID_ATTACK3_HAND,			//素手攻撃3中アニメーション
		ANIMID_ATTACK3_HAND_IN,			//素手攻撃3前アニメーション
		ANIMID_ATTACK3_HAND_OUT,		//素手攻撃3後アニメーション
		ANIMID_BLOW_AWAY,				//吹き飛んでいるアニメーション
		ANIMID_CHARGE,					//チャージ中のアニメーション
		ANIMID_CHARGE_IN,				//チャージ前のアニメーション
		ANIMID_CLAP,					//拍手のアニメーション
		ANIMID_DEFAULT,					//デフォルトのアニメーション
		ANIMID_DIE,						//死亡時のアニメーション
		ANIMID_DODGEROLL,				//回避のアニメーション
		ANIMID_DOWN,					//ダウン中のアニメーション
		ANIMID_DOWN_IN,					//ダウン前のアニメーション
		ANIMID_GET_UP,					//起き上がりのアニメーション
		ANIMID_GUARD,					//ガード中アニメーション
		ANIMID_GUARD_IN,				//ガード前アニメーション
		ANIMID_GUARD_OUT,				//ガード後アニメーション
		ANIMID_HIT,						//被弾のアニメーション
		ANIMID_ITEM_USE,				//アイテムを使用中のアニメーション
		ANIMID_ITEM_USE_IN,				//アイテムを使用する前のアニメーション
		ANIMID_ITEM_USE_OUT,			//アイテムを使用した後のアニメーション
		ANIMID_JUMP,					//ジャンプするアニメーション
		ANIMID_LANDING,					//着地するアニメーション
		ANIMID_LIFT_UP,					//物を持ち上げるアニメーション
		ANIMID_PUT_DOWN,				//物を下ろすアニメーション
		ANIMID_READY,					//準備完了のアニメーション
		ANIMID_READY_OUT,				//準備完了をやめるアニメーション
		ANIMID_SKILLA,					//スキルA使用中のアニメーション
		ANIMID_SKILLA_IN,				//スキルA使用前のアニメーション
		ANIMID_SKILLA_OUT,				//スキルA使用後のアニメーション
		ANIMID_SKILLB,					//スキルB使用中のアニメーション
		ANIMID_SKILLB_IN,				//スキルB使用前のアニメーション
		ANIMID_SKILLB_OUT,				//スキルB使用後のアニメーション
		ANIMID_THROW,					//物を投げる中のアニメーション
		ANIMID_THROW_IN,				//物を投げる前のアニメーション
		ANIMID_THROW_OUT,				//物を投げる後のアニメーション
		ANIMID_WAIT,					//待機状態のアニメーション
		ANIMID_WAIT_LIFTING_UP,			//物を持ち上げている待機状態のアニメーション
		ANIMID_WALK,					//歩きのアニメーション
		ANIMID_WALK_LIFTING_UP,			//物を持ち上げている歩きのアニメーション
		ANIMID_WINNER,					//勝者のアニメーション

	};

	//---------------------------------------------

public:
	//コンストラクタ・デストラクタ
	CPlayer();
	virtual ~CPlayer();

	//初期化
	virtual void Init(tagPlayerName _name, tagPadName _padName);

	//モデルロード
	virtual void Load(int _modelHndl);

	//毎フレームする処理
	virtual void Step(float _rotY,VECTOR* _targetPos, CAttackManager* _attackManager, CShotManager* _shotManager);

	//描写処理
	virtual void Draw();

	//更新処理
	virtual void Update();

	//終了処理
	virtual void Exit();

	//復活処理
	void Respawn(VECTOR _respawnPos);

	//当たり判定後の処理
	virtual void HitCalc(CObject* _hitObject);

	//攻撃を食らった時にする処理
	void HitAttack(int _atk, int _blown, float _rotY = 0.0f);

	//重力リセット
	void GravityReset();

	//コインを落としているかを取得
	int GetDropCoin() { return m_dropCoin; }
	//コインを落としているかを設定
	void SetDropCoin(int _dropCoin) { m_dropCoin = _dropCoin; }

	//コントローラーの名前を取得
	tagPadName GetPadName() { return m_padName; }

	//武器のIDを取得
	tagWeaponId GetWeaponId() { return m_weaponId; }
	//武器のIDを設定
	void SetWeaponId(tagWeaponId _weaponId) { m_weaponId = _weaponId; }

	//武器の耐久度をセットする
	void SetWeaponDurability(int _durability) { m_weaponDurability = _durability; }

	//名前を取得
	tagPlayerName GetPlayerName() { return m_name; }

	//アイテムの状態を取得
	tagHaveItemState GetItemState() { return m_itemState; }

	//アイテムの状態を設定
	void SetItemState(tagHaveItemState _itemState) { m_itemState = _itemState; }

	//体力を増やす
	void AddHp(int _addNum) { m_hp += _addNum; }

	//体力の最大値を取得
	int GetHpMax() { return m_maxHp; }

	//お金を取得
	int GetMoney() { return m_money; }

	//お金を設定
	void SetMoney(int _money) { m_money = _money; }

	//お金を増やす
	void AddMoney(int _addMoney) { m_money += _addMoney; }

	//お金を減らす
	bool SubMoney(int _subMoney);

	//持っているアイテムの座標を取得
	VECTOR GetItemHavePos();

	//武器の座標を取得
	VECTOR GetWeaponPos();

	//座標をアドレスを取得
	VECTOR* GetPosPoint() { return &m_pos; }

	//cpuかどうかを取得
	bool GetIsCpu() { return m_isCpu; }

	//状態を設定
	void SetState(tagState _state) { m_state = _state; }

protected:
	//待機状態処理
	void Wait();

	//歩く状態処理
	void Walk();

	//ジャンプ状態処理
	void Jump();

	//空中状態処理
	void Air();

	//着地状態処理
	void Landing();

	//回避状態処理
	void Dodgeroll();

	//攻撃前処理
	void AttackIn();

	//攻撃状態処理
	void Attack(CAttackManager* _attackManager, CShotManager* _shotManager);

	//攻撃後処理
	void AttackOut();

	//攻撃チャージ前
	void AttackChargeIn();

	//攻撃チャージ
	void AttackCharge();

	//スキル使用前
	void SkillIn();

	//スキル使用
	void Skill();

	//スキル使用後
	void SkillOut();

	//アイテム使用前
	void ItemUseIn();

	//アイテム使用中
	void ItemUse();

	//アイテム使用後
	void ItemUseOut();

	//アイテムを持ち上げる
	void ItemLiftUp();

	//アイテムを下ろす
	void ItemPutDown();

	//アイテムを投げる前
	void ItemThrowIn();

	//アイテムを投げる
	void ItemThrow();

	//アイテムを投げた後
	void ItemThrowOut();

	//怯み状態処理
	void Stagger();

	//吹き飛んだ
	void BlowAway();

	//ダウン前
	void DownIn();

	//ダウン
	void Down();

	//起き上がり
	void GetUp();

	//死亡
	void Die();

	//準備完了
	void Ready();

	//準備完了を解除
	void ReadyOut();

	//拍手
	void Clap();

	//勝利モーション
	void Winner();

	//移動処理
	virtual void Move(float _rotY);

	//攻撃を呼び出す処理
	virtual void RequestAttack();

	//ジャンプの呼び出し処理
	virtual void RequestJump();

	//回避に移行する処理
	void RequestDodgeroll(float _rotY);

	//アイテム処理
	void Item();

	//アイテムを拾う
	void PickUpItem();

};

