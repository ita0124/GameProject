#include "player.h"
#include "../data.h"
#include "../../lib/myMath/myMath.h"
#include <math.h>
#include "../common.h"
#include "../../lib/input/controllerManager.h"
#include"../../lib/input/keyInput.h"
#include "playerManager.h"
#include "../../lib/effekseer/effekseer.h"
#include "../system/effectData/effectData.h"
#include "../item/itemBase.h"
#include "../system/soundManager.h"

//定義関連---------------------------

//プレイヤー関連--------------------------------
constexpr char MODEL_PATH[] =
 "data/model/player/playerTransformTest.mv1" ;				//ロードするファイル名
constexpr VECTOR INIT_POS = { 0.0f,1.0f,0.0f };				//初期座標
constexpr float SHADOW_SIZE = 0.5f;							//丸影の大きさ
constexpr int MAX_HP = 200;									//体力
constexpr int ATK = 20;										//攻撃力
constexpr float MOVE_SPEED = 1.2f * 1.5f;					//移動スピード
constexpr float RADIUS = 10.0f;								//半径
constexpr float JUMP_SPEED = 3.0f;							//ジャンプスピード
constexpr int BLOWN_MAX = 100;								//吹き飛び最大値
constexpr VECTOR KNOCK_BACK_SPEED = { 0.0f,3.0f,-0.8f };	//吹き飛ぶスピード
constexpr int INIT_MONEY = 0;								//最初の所持金
constexpr float MONEY_DROP_RATE = 0.4f;						//落とすお金の割合
constexpr float MONEY_RESPAWN_RATE = 0.5f;					//復活するときに消費するお金の割合
constexpr float DIE_POS_Y = -100.0f;						//死ぬ高さ
constexpr float FALL_OUT_DAMAGER_RATE = 0.3f;				//ステージから落下したときの割合ダメージ
constexpr int GET_UP_MAX_TIME = 3 * 60;						//起き上がるまでの最大時間
//----------------------------------------------

//攻撃関連---------------------------
constexpr int ATTACKB_ATK = 100;							//攻撃Bの攻撃力
constexpr float ATTACK_MOVE_SPEED = 0.5f;					//攻撃時に前進する力
constexpr float FIGHT_LEN = 40.0f;							//戦う距離
constexpr float SHOT_SIZE = 10.0f;							//弾の大きさ
constexpr float SHOT_SPEED = 2.5f;							//弾の速度
constexpr int SHOT_LOST_TIME = 2 * 60;						//弾が消えるまでの時間
constexpr float HAMMER_ATTACK_AIR_FALL_SPEED = 4.0f;		//ハンマーの空中攻撃の落下スピード
constexpr float HAMMER_FALL_FRAME = 13.0f;					//ハンマーの落下するまでのフレーム
constexpr float HAMMER_FALL_MIN_LENGTH = 30.0f;				//ハンマーの落下攻撃ができる最小の高さ
//-----------------------------------

enum tagAttackNum
{
	ATTACK_NONE = -1,	//攻撃をしていない
	ATTACK_1,			//一段階目の攻撃
	ATTACK_2,			//二段階目の攻撃
	ATTACK_3,			//三段階目の攻撃
	ATTACK_AIR,			//空中の攻撃

	ATTACK_NUM,			//攻撃の数
};

constexpr float ATTACK_MAGNIFICATION[WEAPON_ID_NUM][ATTACK_NUM] =	//攻撃の倍率
{
	{0.5f,0.8f,1.0f,1.0f},
	{0.7f,1.0f,1.2f,1.0f},
	{1.0f,1.2f,1.5f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
};

constexpr int ATTACK_BLOWN[WEAPON_ID_NUM][ATTACK_NUM] =				//攻撃の吹き飛び度
{
	{40,40,80,100},
	{40,40,80,100},
	{100,100,100,100},
	{60,60,80,100},
};

constexpr float ATTACK_SIZE[WEAPON_ID_NUM][ATTACK_NUM] =			//攻撃の大きさ
{
	{12.0f,12.0f,12.0f,12.0f},
	{25.0f,25.0f,25.0f,25.0f},
	{28.0f,28.0f,28.0f,12.0f},
	{16.0f,16.0f,16.0f,12.0f},
};

constexpr float ATTACK_LENGTH[WEAPON_ID_NUM][ATTACK_NUM] =			//攻撃の長さ
{
	{15.0f,15.0f,15.0f,15.0f},
	{15.0f,15.0f,15.0f,0.0f},
	{15.0f,15.0f,15.0f,15.0f},
	{15.0f,15.0f,15.0f,15.0f},
};						

//---------------------------------------------------------

//-----------------------
//	コンストラクタ
//-----------------------
CPlayer::CPlayer()
{
	m_name = PLAYER_NONE;
	CCharacterBase::Init();
	m_dropCoin = 0;
	m_pos = ZERO;
	m_rad = 0.0f;
	m_maxHp = 0;
	m_hp = 0;
	m_atk = 0;
	m_isJump = false;
	m_attackNum = ATTACK_NONE;
	m_money = INIT_MONEY;
	m_attackId = -1;
	m_effectId = -1;
	m_padName = PAD_NONE;
	m_weaponId = WEAPON_ID_HAND;
	m_weaponDurability = 0;
	m_itemState = ITEM_STATE_NONE;
	m_targetPos = nullptr;
	m_isCpu = false;
	m_getUpTime = 0;
}

//-----------------------
//	デストラクタ
//-----------------------
CPlayer::~CPlayer()
{
	Exit();
}

//-----------------------
//		初期化
//-----------------------
void CPlayer::Init(tagPlayerName _name, tagPadName _padName)
{
	CCharacterBase::Init();

	m_dropCoin = 0;
	m_pos = ZERO;
	m_rad = RADIUS;
	m_maxHp = MAX_HP;
	m_hp = m_maxHp;
	m_atk = ATK;
	m_attackNum = ATTACK_NONE;
	m_weaponDurability = 0;
	m_money = INIT_MONEY;
	m_padName = _padName;
	m_weaponId = WEAPON_ID_HAND;
	m_name = _name;
	m_shadow.Init(m_pos, SHADOW_SIZE);
	m_objectName = OBJECT_PLAYER;
	m_isCpu = false;
	m_getUpTime = 0;
}

//-----------------------
//	モデルロード
//-----------------------
void CPlayer::Load(int _modelHndl)
{
	CObject::DuplicateModel(_modelHndl);
	m_shadow.Load();
}

//-----------------------
//毎フレームする処理
//-----------------------
void CPlayer::Step(float _rotY, VECTOR* _targetPos, CAttackManager* _attackManager, CShotManager* _shotManager)
{
	m_targetPos = _targetPos;

	if (m_isJump == true)
	{
		m_isJump = false;
		m_gravity = JUMP_SPEED;
		m_isFlying = true;
	}

	if (m_targetPos != nullptr && m_isFlying == false)
	{
		switch (m_state)
		{
		case BLOW_AWAY:
		case DOWN:
		case DOWN_IN:
		case GET_UP:
		case DIE:
			break;
		default:
			//プレイヤー同士の距離
			VECTOR vLen = VSub(m_pos, *m_targetPos);
			float fLen = VSize(vLen);

			//戦いの距離になったら互いの方向を向く
			if (fLen <= FIGHT_LEN)
			{
				float rotY1 = atan2f(m_pos.x - m_targetPos->x, m_pos.z - m_targetPos->z);

				m_rot.y = rotY1;
			}

			//プレイヤーの向きを変える
			if (m_state == ITEM_THROW_IN)
			{
				float rotY = atan2f(m_pos.x - m_targetPos->x, m_pos.z - m_targetPos->z);

				m_rot.y = rotY;
			}
			break;
		}
	}

	//丸影とプレイヤーの座標が離れていたら飛んでいる
	if (m_pos.y - m_shadow.GetPos().y > 1.0f)
	{
		m_isFlying = true;
	}

	//空中にいたら状態を空中に変える
	if (m_isFlying == true)
	{
		switch (m_state)
		{
		case ATTACK_IN:
		case ATTACK:
		case ATTACK_OUT:
		case BLOW_AWAY:
		case DIE:
			break;
		default:
			m_state = AIR;
			break;
		}
	}

	//指定した高度よりしたに落ちたら死んで復活する
	if (m_pos.y <= DIE_POS_Y)
	{
		m_isActive = false;
	}

	//武器の耐久度処理--------------------------------
	//素手以外の場合耐久度が0以下になったら武器が壊れる
	if (m_weaponId != WEAPON_ID_HAND)
	{
		if (m_weaponDurability <= 0)
		{
			m_weaponId = WEAPON_ID_HAND;
			m_weaponDurability = 0;
		}
	}

	//------------------------------------------------

	//攻撃の呼び出し
	if ((CheckHitKey(KEY_INPUT_J) != 0 ||
		CControllerManager::IsTrg(BUTTON_X, m_padName)) &&
		m_attackId == -1)
	{
		RequestAttack();
	}
	else
	{
		switch (m_state)
		{
		case WAIT:
		case WALK:
			m_attackNum = 0;
			break;
		}
	}

	switch (m_state)
	{
	case ATTACK_IN:
	case ATTACK:
	case ATTACK_OUT:
		break;
	default:
		if (m_attackNum != ATTACK_NONE)
		{
			m_attackNum = ATTACK_NONE;
		}
		break;
	}

	//移動処理
	Move(_rotY);

	//ジャンプ処理
	RequestJump();

	//回避移行処理
	RequestDodgeroll(_rotY);

	//アイテム使用処理
	Item();

	//アイテムを手に入れていたら持ち上げる
	if (m_itemState == ITEM_STATE_GET)
	{
		m_itemState = ITEM_STATE_HAVE;
		m_state = ITEM_LIFT_UP;
	}

	//アイテムを取ろうとしていたら持っていない状態に戻す
	if (m_itemState == ITEM_STATE_PICK_UP)
	{
		m_itemState = ITEM_STATE_NONE;
	}

	if (CheckHitKey(KEY_INPUT_I) != 0 ||
		CControllerManager::IsTrg(BUTTON_B, m_padName) == true)
	{
		//アイテムを拾う処理
		PickUpItem();
	}

	CCharacterBase::Step(_attackManager,_shotManager);

	//体力が増えすぎないように
	if (m_hp >= m_maxHp)
	{
		m_hp = m_maxHp;
	}

	//攻撃の当たり判定が消えたら攻撃のIDを初期化する
	if (_attackManager->GetActive(m_attackId) == false)
	{
		m_attackId = -1;
	}
	//攻撃中は当たり判定をプレイヤーの位置に設定する
	else
	{
		_attackManager->SetPos(m_attackId,m_pos);
	}

	//攻撃中ではない場合攻撃を消す
	if (m_state != ATTACK)
	{
		_attackManager->SetActive(m_attackId,false);
	}

	if (m_effectId != -1)
	{
		CEffekseerCtrl::SetPosition(m_effectId, GetCenter());

		if (CEffekseerCtrl::IsActive(m_effectId) == false)
		{
			m_effectId = -1;
		}
	}
}

//-----------------------
//		描写処理
//-----------------------
void CPlayer::Draw()
{
	CCharacterBase::Draw();

#ifdef DEBUG
	//当たり判定を表示
	DrawSphere3D(GetCenter(), m_rad, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);

	VECTOR attackPos;
	attackPos.x = -sinf(m_rot.y) * ATTACK_LENGTH[m_weaponId][m_attackId];
	attackPos.z = -cosf(m_rot.y) * ATTACK_LENGTH[m_weaponId][m_attackId];

	attackPos = VAdd(attackPos, m_pos);
	float attackSize = ATTACK_SIZE[m_weaponId][m_attackNum];

	attackPos.y = m_pos.y + attackSize * 0.5f;

	DrawSphere3D(attackPos, attackSize, 16, GetColor(0, 255, 0), GetColor(0, 255, 0), FALSE);

#endif // DEBUG

}

//-----------------------
//		更新処理
//-----------------------
void CPlayer::Update()
{
	CCharacterBase::Update();

	//速度のリセット
	ResetSpeed();

}

//終了処理
void CPlayer::Exit()
{
	CCharacterBase::Exit();
}

//復活処理
void CPlayer::Respawn(VECTOR _respawnPos)
{
	m_pos = _respawnPos;
	m_isActive = true;
	switch (m_state)
	{
	//死んでいた場合
	case DIE:
		m_hp = m_maxHp;
		m_weaponId = WEAPON_ID_HAND;
		m_weaponDurability = 0;
		m_state = WAIT;
		break;
	//死んでいない場合
	default:
		m_state = WAIT;
		m_hp -= static_cast<int>(m_maxHp * FALL_OUT_DAMAGER_RATE);
		break;
	}
}

//当たり判定後の処理
void CPlayer::HitCalc(CObject* _hitObject)
{
	//_hitObjectがnullだったら処理をしない
	if (_hitObject == nullptr)return;

	//攻撃の当たり判定の場合の処理-----------------------------------------
	if (_hitObject->GetObjectName() == OBJECT_ATTACK)
	{
		//ダウン状態と起き上がり中は判定をしない
		if (m_state == DOWN ||
			m_state == DOWN_IN ||
			m_state == GET_UP)
			return;

		//当たり判定保存用
		CAttackBase* attack = nullptr;

		attack = dynamic_cast<CAttackBase*>(_hitObject);

		//攻撃判定が消えている場合処理をしない
		if (attack->GetIsAttack() == false)return;

		//自分が出した攻撃の場合処理をしない
		if (attack->GetAttackName() == m_name)return;

		//ノックバックの方向
		float rot = atan2f(attack->GetPos().x - GetCenter().x,
			attack->GetPos().z - GetCenter().z);

		HitAttack(attack->GetAtk(),attack->GetBlown(), rot);

		//呼び出すエフェクトのID
		int effectId = CEffectData::GetId(EFFECT_ATTACK);

		//エフェクトを呼び出す
		CEffekseerCtrl::Request(effectId, GetCenter(), false);

		//アイテムを落とす
		m_itemState = ITEM_STATE_DROP;

	}
	//---------------------------------------------------------------------

	//アイテムの場合の処理-------------------------------------------------
	if (_hitObject->GetObjectName() == OBJECT_ITEM)
	{
		//アイテム保存用
		CItemBase* item = nullptr;

		item = dynamic_cast<CItemBase*>(_hitObject);

		//アイテムがオブジェクトタイプ以外の場合処理をしない
		if (item->GetItemType() == ITEM_TYPE_COIN)
		{
			if (m_effectId != -1)
			{
				CEffekseerCtrl::Stop(m_effectId);
				m_effectId = -1;
			}

			//呼び出すエフェクトのID
			int effectId = CEffectData::GetId(EFFECT_COIN_GET);

			//プレイヤーの位置にエフェクトを呼び出す
			m_effectId = CEffekseerCtrl::Request(effectId, GetCenter(), false);
		}

	}
	//---------------------------------------------------------------------

	//弾の場合の処理-------------------------------------------------------
	if (_hitObject->GetObjectName() == OBJECT_SHOT)
	{

		CShotBase* shot = nullptr;

		shot = dynamic_cast<CShotBase*>(_hitObject);

		//自分が出した攻撃の場合処理をしない
		if (shot->GetShotName() == m_name)return;

		//ノックバックの方向
		float rot = atan2f(shot->GetPos().x - GetCenter().x,
			shot->GetPos().z - GetCenter().z);

		CCharacterBase::HitAttack(shot->GetAtk(), 90, rot);

		shot->SetActive(false);

		//呼び出すエフェクトのID
		int effectId = CEffectData::GetId(EFFECT_ATTACK);

		//エフェクトを呼び出す
		CEffekseerCtrl::Request(effectId, GetCenter(), false);

		//アイテムを落とす
		m_itemState = ITEM_STATE_DROP;

	}

	//---------------------------------------------------------------------

}

//------------------------------
//	攻撃を食らった時にする処理
//------------------------------
void CPlayer::HitAttack(int _atk, int _blown, float _rotY)
{
	//死亡していたら処理をしない
	if (m_state == DIE)return;

	CSoundManager::Play(CSoundManager::SE_HIT, DX_PLAYTYPE_BACK);

	m_blown += _blown;

	VECTOR knockBack = KNOCK_BACK_SPEED;

	if (m_blown >= BLOWN_MAX || m_state == AIR)
	{
		knockBack = VScale(knockBack, 10.0f);
		m_blown = 0;
		
		//コインを落とす量を求める
		m_dropCoin = static_cast<int>(m_money * MONEY_DROP_RATE);

		//落としたコイン量だけお金を減らす
		m_money -= m_dropCoin;

		//吹き飛ばされ状態
		m_state = BLOW_AWAY;
	}
	else
	{
		//怯み状態にする
		m_state = STAGGER;
	}

	//既に怯み状態なら処理をしない
	if (m_state == STAGGER)return;

	//ノックバックの速度を設定------------

	//プレイヤーが目の前にいる時に進む速度
	VECTOR defaultDir = knockBack;
	//上記を行列に変換
	MATRIX dir = CMyMath::GetTranslateMatrix(defaultDir);
	//Y軸回転行列
	MATRIX mRotY = CMyMath::GetYawMatrix(_rotY);
	//行列の合成
	MATRIX res = CMyMath::MatMult(mRotY, dir);

	//移動をスピードに代入
	m_speed.x = res.m[0][3];
	m_speed.y = res.m[1][3];
	m_speed.z = res.m[2][3];

	//------------------------------------

	//Hpを攻撃力分減らす
	m_hp -= _atk;
}

//重力リセット
void CPlayer::GravityReset()
{
	CObject::GravityReset();

	if (m_state == AIR)
	{
		m_state = LANDING;
	}
}

//お金を減らす
bool CPlayer::SubMoney(int _subMoney)
{
	//消費量が今のお金より少なかったら購入しない
	if (m_money < _subMoney)
	{
		return false;
	}

	m_money -= _subMoney;

	return true;
}

//-----------------------
//		待機処理
//-----------------------
void CPlayer::Wait()
{
	//アイテムを持っている場合待機のモーションが変わる
	if (m_itemState == ITEM_STATE_HAVE)
	{
		//物を持ち上げた状態の待機アニメーションを再生
		RequestAnim(ANIMID_WAIT_LIFTING_UP, 0.5f, true);
	}
	else
	{
		//待機アニメーションを再生
		RequestAnim(ANIMID_WAIT, 0.5f, true);
	}
	
}

//-----------------------
//		歩く処理
//-----------------------
void CPlayer::Walk()
{
	//アイテムを持っている場合歩きのモーションが変わる
	if (m_itemState == ITEM_STATE_HAVE)
	{
		//物を持ち上げた状態の歩くアニメーション
		RequestAnim(ANIMID_WALK_LIFTING_UP, 1.0f, true);
	}
	else
	{
		//歩くアニメーション
		RequestAnim(ANIMID_WALK, 1.0f, true);
	}


	//止まっていたら待機状態に移行
	if (m_speed.x == 0.0f &&
		m_speed.z == 0.0f)
	{
		m_state = WAIT;
	}

}

//-----------------------
//		ジャンプ
//-----------------------
void CPlayer::Jump()
{
	RequestAnim(ANIMID_JUMP, 2.0f);

	if (GetAnimEnd() == true)
	{
		m_isJump = true;
		m_state = AIR;
	}
}

//-----------------------
//			空中
//-----------------------
void CPlayer::Air()
{
	RequestAnim(ANIMID_AIR, 1.0f, true);

	if (m_isFlying == false)
	{
		m_state = WAIT;
	}
}

//-----------------------
//			着地
//-----------------------
void CPlayer::Landing()
{
	RequestAnim(ANIMID_LANDING, 1.0f);

	if (GetAnimEnd() == true)
	{
		m_isJump = false;

		m_state = WAIT;
	}
}

//-----------------------
//		  回避
//-----------------------
void CPlayer::Dodgeroll()
{
}

//-----------------------
//		攻撃前
//-----------------------
void CPlayer::AttackIn()
{
	if (m_isFlying == true)
	{
		m_attackNum = ATTACK_AIR;
	}

	if (m_weaponId != WEAPON_ID_GUN)
	{

		//進む速度
		VECTOR defaultDir = { 0.0f,0.0f,-ATTACK_MOVE_SPEED };
		//上記を行列に変換
		MATRIX dir = CMyMath::GetTranslateMatrix(defaultDir);
		//Y軸回転行列
		MATRIX mRotY = CMyMath::GetYawMatrix(m_rot.y);
		//行列の合成
		MATRIX res = CMyMath::MatMult(mRotY, dir);

		//移動をスピードに代入
		m_speed.x = res.m[0][3];
		m_speed.y = res.m[1][3];
		m_speed.z = res.m[2][3];
	}

	switch (m_weaponId)
	{
	//武器が素手の場合
	case WEAPON_ID_HAND:
		switch (m_attackNum)
		{
		case ATTACK_1:
			//攻撃前のアニメーション
			RequestAnim(ANIMID_ATTACK1_HAND_IN, 1.0f);
			break;
		case ATTACK_2:
			//攻撃前のアニメーション
			RequestAnim(ANIMID_ATTACK2_HAND_IN, 0.8f);
			break;
		case ATTACK_3:
			//攻撃前のアニメーション
			RequestAnim(ANIMID_ATTACK3_HAND_IN, 0.8f);
			break;
		case ATTACK_AIR:
			//空中の攻撃前アニメーション
			RequestAnim(ANIMID_AIR_ATTACK_HAND_IN, 1.0f);
			break;
		}

		break;
	//武器がハンマーの場合
	case WEAPON_ID_HAMMER:
		switch (m_attackNum)
		{
		case ATTACK_1:
			//攻撃前のアニメーション
			RequestAnim(ANIMID_ATTACK1_HAMMER_IN, 0.6f);
			break;
		case ATTACK_2:
			//攻撃前のアニメーション
			RequestAnim(ANIMID_ATTACK2_HAMMER_IN, 0.6f);
			break;
		case ATTACK_3:
			//攻撃前のアニメーション
			RequestAnim(ANIMID_ATTACK3_HAMMER_IN, 0.6f);
			break;
		case ATTACK_AIR:
			if (m_animData.m_id != ANIMID_AIR_ATTACK_HAMMER_IN)
			{
				float len = m_pos.y - m_shadow.GetPos().y;
				if (HAMMER_FALL_MIN_LENGTH >= len)
				{
					m_state = WAIT;
					return;
				}
			}
			//空中の攻撃前アニメーション
			RequestAnim(ANIMID_AIR_ATTACK_HAMMER_IN, 0.7f);
			if (m_animData.m_frame >= HAMMER_FALL_FRAME)
			{
				m_gravity = -HAMMER_ATTACK_AIR_FALL_SPEED;
			}
			else
			{
				m_gravity = 0;
			}
			break;
		}

		break;
	//武器が斧の場合
	case WEAPON_ID_AX:
		m_attackNum = 0;
		//攻撃前のアニメーション
		RequestAnim(ANIMID_ATTACK1_AX_IN, 0.5f);
		break;
	//武器が銃の場合
	case WEAPON_ID_GUN:
		//攻撃前のアニメーション
		RequestAnim(ANIMID_ATTACK1_GUN_IN, 0.5f);

		if (m_targetPos != nullptr)
		{
			float rotY = atan2f(m_pos.x - m_targetPos->x, m_pos.z - m_targetPos->z);
			m_rot.y = rotY;
		}

		break;
	}

	//ハンマーの空中攻撃は着地するまで続く
	if (m_weaponId == WEAPON_ID_HAMMER &&
		m_attackNum == ATTACK_AIR)
	{
		if (m_isFlying == false)
		{
			m_state = ATTACK;
		}
		return;
	}

	//アニメーションが終わったら攻撃中に移行
	if (GetAnimEnd() == true)
	{
		m_state = ATTACK;
	}

}

//-----------------------
//		攻撃中
//-----------------------
void CPlayer::Attack(CAttackManager* _attackManager, CShotManager* _shotManager)
{
	int attackTime = 180;
	int attackNum = attackTime / 10;

	//攻撃の座標
	VECTOR attackPos;
	attackPos.x = -sinf(m_rot.y) * ATTACK_LENGTH[m_weaponId][m_attackId];
	attackPos.z = -cosf(m_rot.y) * ATTACK_LENGTH[m_weaponId][m_attackId];

	attackPos = VAdd(attackPos, m_pos);

	attackPos.y = m_pos.y;

	//攻撃力を計算
	int atk = static_cast<int>(m_atk * ATTACK_MAGNIFICATION[m_weaponId][m_attackNum]);

	int blown = ATTACK_BLOWN[m_weaponId][m_attackNum];

	float attackSize = ATTACK_SIZE[m_weaponId][m_attackNum];

	switch (m_weaponId)
	{
	//武器が素手の場合
	case WEAPON_ID_HAND:
		CSoundManager::Play(CSoundManager::SE_HAND, DX_PLAYTYPE_BACK);

		switch (m_attackNum)
		{
		case ATTACK_1:
			//攻撃中のアニメーション
			if (RequestAnim(ANIMID_ATTACK1_HAND, 1.0f) == true)
			{
				_attackManager->Request(attackPos, attackSize, atk, blown, m_name);
			}
			break;
		case ATTACK_2:
			//攻撃中のアニメーション
			if (RequestAnim(ANIMID_ATTACK2_HAND, 1.0f) == true)
			{
				_attackManager->Request(attackPos, attackSize, atk, blown, m_name);
			}
			break;
		case ATTACK_3:
			//攻撃中のアニメーション
			if(RequestAnim(ANIMID_ATTACK3_HAND, 1.0f) == true)
			{
				_attackManager->Request(attackPos, attackSize, atk, blown, m_name);
			}
			break;
		case ATTACK_AIR:
			//空中の攻撃中アニメーション
			if (RequestAnim(ANIMID_AIR_ATTACK_HAND, 1.2f) == true)
			{
				_attackManager->Request(attackPos, attackSize, atk, blown, m_name);
						}
			break;
		}

		break;
	//武器がハンマーの場合
	case WEAPON_ID_HAMMER:
		CSoundManager::Play(CSoundManager::SE_SWORD, DX_PLAYTYPE_BACK);

		switch (m_attackNum)
		{
		case ATTACK_1:
			//攻撃中のアニメーション
			if (RequestAnim(ANIMID_ATTACK1_HAMMER, 0.6f) == true)
			{
				_attackManager->Request(attackPos, attackSize, atk, blown, m_name);
			}
			break;
		case ATTACK_2:
			//攻撃中のアニメーション
			if (RequestAnim(ANIMID_ATTACK2_HAMMER, 0.6f) == true)
			{
				_attackManager->Request(attackPos, attackSize, atk, blown, m_name);
			}
			break;
		case ATTACK_3:
			//攻撃中のアニメーション
			if (RequestAnim(ANIMID_ATTACK3_HAMMER, 0.6f) == true)
			{
				_attackManager->Request(attackPos, attackSize, atk, blown, m_name);
			}
			break;
		case ATTACK_AIR:
			//空中の攻撃中アニメーション
			if (RequestAnim(ANIMID_AIR_ATTACK_HAMMER, 0.6f) == true)
			{
				_attackManager->Request(attackPos, attackSize, atk, blown, m_name);
				
				int effectId = CEffectData::GetId(EFFECT_SHOCK_WAVE);
				
				CEffekseerCtrl::Request(effectId, attackPos, false);
			};
			break;
		}
		break;
	//武器が斧の場合
	case WEAPON_ID_AX:
		CSoundManager::Play(CSoundManager::SE_AX, DX_PLAYTYPE_BACK);
		//攻撃中のアニメーション
		if (RequestAnim(ANIMID_ATTACK1_AX, 1.0f,true) == true)
		{
			m_attackId = _attackManager->Request(attackPos, attackSize, atk, blown, m_name,attackNum,attackTime);
		}
		break;
	//武器が銃の場合
	case WEAPON_ID_GUN:
		CSoundManager::Play(CSoundManager::SE_GUN, DX_PLAYTYPE_BACK);

		if (RequestAnim(ANIMID_ATTACK1_GUN, 1.0f) == true)
		{
			VECTOR shotPos = MV1GetFramePosition(m_hndl, 11);
			_shotManager->Request(shotPos,m_rot,attackSize,SHOT_SPEED,atk,SHOT_LOST_TIME,m_name);
		}
		break;
	}

	//アニメーションが終わったら待機状態に戻す
	if (GetAnimEnd() == true || _attackManager->GetActive(m_attackId) == false)
	{
		//武器の耐久度が減る
		if (m_weaponId != WEAPON_ID_HAND)
		{
			m_weaponDurability--;
		}

		m_state = ATTACK_OUT;
	}

}

//-----------------------
//		攻撃後
//-----------------------
void CPlayer::AttackOut()
{
	switch (m_weaponId)
	{
	//武器が素手の場合
	case WEAPON_ID_HAND:
		switch (m_attackNum)
		{
		case ATTACK_1:
			//攻撃後のアニメーション
			RequestAnim(ANIMID_ATTACK1_HAND_OUT, 1.0f);
			break;
		case ATTACK_2:
			//攻撃後のアニメーション
			RequestAnim(ANIMID_ATTACK2_HAND_OUT, 0.8f);
			break;
		case ATTACK_3:
			//攻撃後のアニメーション
			RequestAnim(ANIMID_ATTACK3_HAND_OUT, 0.3f);
			break;
		case ATTACK_AIR:
			//空中の攻撃後アニメーション
			RequestAnim(ANIMID_AIR_ATTACK_HAND_OUT, 1.2f);
			break;
		}
	
		break;
	//武器がハンマーの場合
	case WEAPON_ID_HAMMER:
		switch (m_attackNum)
		{
		case ATTACK_1:
			//攻撃後のアニメーション
			RequestAnim(ANIMID_ATTACK1_HAMMER_OUT, 0.6f);
			break;
		case ATTACK_2:
			//攻撃後のアニメーション
			RequestAnim(ANIMID_ATTACK2_HAMMER_OUT, 0.6f);
			break;
		case ATTACK_3:
			//攻撃後のアニメーション
			RequestAnim(ANIMID_ATTACK3_HAMMER_OUT, 0.6f);
			break;
		case ATTACK_AIR:
			//空中の攻撃後アニメーション
			RequestAnim(ANIMID_AIR_ATTACK_HAMMER_OUT, 0.6f);
			break;
		}
	
		break;
	//武器が斧の場合
	case WEAPON_ID_AX:
		//攻撃後のアニメーション
		RequestAnim(ANIMID_ATTACK1_AX_OUT, 0.5f);
		break;
	//武器が銃の場合
	case WEAPON_ID_GUN:
		//攻撃後のアニメーション
		RequestAnim(ANIMID_ATTACK1_GUN_OUT, 1.0f);
		break;
	}

	//アニメーションが終わったら待機状態に戻す
	if (GetAnimEnd() == true)
	{
		m_attackNum = ATTACK_NONE;
		m_state = WAIT;
	}

}

//-----------------------
//	   攻撃チャージ前
//-----------------------
void CPlayer::AttackChargeIn()
{
	//アイテム使用前のアニメーション
	RequestAnim(ANIMID_CHARGE_IN, 0.3f);

	//アニメーションが終わったらアイテム使用中に移行
	if (GetAnimEnd() == true)
	{
		m_state = ATTACK_CHARGE;
	}

}

//-----------------------
//	   攻撃チャージ
//-----------------------
void CPlayer::AttackCharge()
{
	//アイテム使用前のアニメーション
	RequestAnim(ANIMID_CHARGE, 0.3f);

	//アニメーションが終わったらアイテム使用中に移行
	if (GetAnimEnd() == true)
	{
		m_state = SKILL_IN;
	}

}

//-----------------------
//		スキル使用前
//-----------------------
void CPlayer::SkillIn()
{
	switch (m_weaponId)
	{
	case WEAPON_ID_HAND:
		//攻撃前のアニメーション
		RequestAnim(ANIMID_SKILLA_IN, 1.2f);
		break;
	case WEAPON_ID_HAMMER:
		//攻撃前のアニメーション
		RequestAnim(ANIMID_SKILLB_IN, 1.2f);

		//カメラの角度がオールゼロの時に進む速度
		VECTOR defaultDir = { 0.0f,0.0f,-4.0f };
		//上記を行列に変換
		MATRIX dir = CMyMath::GetTranslateMatrix(defaultDir);
		//Y軸回転行列
		MATRIX mRotY = CMyMath::GetYawMatrix(m_rot.y);
		//行列の合成
		MATRIX res = CMyMath::MatMult(mRotY, dir);

		//移動をスピードに代入
		m_speed.x = res.m[0][3];
		m_speed.y = res.m[1][3];
		m_speed.z = res.m[2][3];
		break;
	}

	//アニメーションが終わったら攻撃中に移行
	if (GetAnimEnd() == true)
	{
		m_state = SKILL;
	}

}

//-----------------------
//		スキル使用
//-----------------------
void CPlayer::Skill()
{
	//アニメーションが終わったら待機状態に戻す
	if (GetAnimEnd() == true)
	{
		m_state = SKILL_OUT;
	}

}

//-----------------------
//		スキル使用後
//-----------------------
void CPlayer::SkillOut()
{
	switch (m_weaponId)
	{
	case WEAPON_ID_HAND:
		//攻撃後のアニメーション
		RequestAnim(ANIMID_SKILLA_OUT, 0.3f);
		break;
	case WEAPON_ID_HAMMER:
		//攻撃後のアニメーション
		RequestAnim(ANIMID_SKILLB_OUT, 0.5f);
		break;
	}

	//アニメーションが終わったら待機状態に戻す
	if (GetAnimEnd() == true)
	{
		m_state = WAIT;
	}

}

//-----------------------
//	  アイテム使用前
//-----------------------
void CPlayer::ItemUseIn()
{
	//アイテム使用前のアニメーション
	RequestAnim(ANIMID_ITEM_USE_IN, 1.0f);

	//アニメーションが終わったらアイテム使用中に移行
	if (GetAnimEnd() == true)
	{
		m_state = ITEM_USE;
	}

}

//-----------------------
//	 アイテム使用中
//-----------------------
void CPlayer::ItemUse()
{
	//アイテム使用中のアニメーション
	if (RequestAnim(ANIMID_ITEM_USE, 1.0f))
	{

	}

	//アニメーションが終わったら待機状態に戻す
	if (GetAnimEnd() == true)
	{
		m_state = ITEM_USE_OUT;
	}

}

//-----------------------
//	 アイテム使用後
//-----------------------
void CPlayer::ItemUseOut()
{
	//アイテム使用後のアニメーション
	RequestAnim(ANIMID_ITEM_USE_OUT, 1.0f);

	//アニメーションが終わったら待機状態に戻す
	if (GetAnimEnd() == true)
	{
		m_state = WAIT;
	}

}

//-----------------------
//	アイテムを持ち上げる
//-----------------------
void CPlayer::ItemLiftUp()
{
	//アイテムを持ち上げるアニメーション
	RequestAnim(ANIMID_LIFT_UP, 1.0f);

	//アニメーションが終わったら待機状態に戻す
	if (GetAnimEnd() == true)
	{
		m_state = WAIT;
	}
}

//-----------------------
//	 アイテムを下ろす
//-----------------------
void CPlayer::ItemPutDown()
{
	//アイテムを下ろすアニメーション
	RequestAnim(ANIMID_PUT_DOWN, 1.0f);

	//アニメーションが終わったら待機状態に戻す
	if (GetAnimEnd() == true)
	{
		m_itemState = ITEM_STATE_NONE;
		m_state = WAIT;
	}

}

//-----------------------
//  アイテムを投げる前
//-----------------------
void CPlayer::ItemThrowIn()
{
	//アイテムを投げる前のアニメーション
	RequestAnim(ANIMID_THROW_IN, 1.0f);

	//アニメーションが終わったらアイテムを投げている状態にする
	if (GetAnimEnd() == true)
	{
		m_state = ITEM_THROW;
	}

}

//-----------------------
//    アイテムを投げる
//-----------------------
void CPlayer::ItemThrow()
{
	//アイテムを投げるアニメーション
	RequestAnim(ANIMID_THROW, 1.0f);

	//アニメーションが終わったらアイテムを投げた後状態にする
	if (GetAnimEnd() == true)
	{
		m_itemState = ITEM_STATE_THROW;
		m_state = ITEM_THROW_OUT;
	}

}

//-----------------------
//  アイテムを投げた後
//-----------------------
void CPlayer::ItemThrowOut()
{
	//アイテムを投げた後のアニメーション
	RequestAnim(ANIMID_THROW_OUT, 1.0f);

	//アニメーションが終わったら待機状態に戻す
	if (GetAnimEnd() == true)
	{
		m_itemState = ITEM_STATE_NONE;
		m_state = WAIT;
	}

}

//-----------------------
//		怯み
//-----------------------
void CPlayer::Stagger()
{
	//被弾のアニメーション
	RequestAnim(ANIMID_HIT, 1.4f);

	//被弾のアニメーションが終わったら戻す
	if (GetAnimEnd() == true)
	{
		m_state = WAIT;
	}
}

//-----------------------
//		吹き飛んだ
//-----------------------
void CPlayer::BlowAway()
{
	//吹き飛んだアニメーション
	RequestAnim(ANIMID_BLOW_AWAY, 1.0f);

	//移動している反対方向を向く
	if (m_speed.x != 0 || m_speed.z != 0)
		m_rot.y = atan2f(m_speed.x, m_speed.z);

	//着地したらダウン状態に移行する
	if(GetAnimEnd() == true &&
		m_isFlying == false)
	{
		m_state = DOWN;
	}
}

//-----------------------
//		 ダウン前
//-----------------------
void CPlayer::DownIn()
{
	//ダウン前のアニメーション
	RequestAnim(ANIMID_DOWN_IN, 1.0f);

	if (GetAnimEnd() == true)
	{
		m_state = DOWN;
	}
}

//-----------------------
//		  ダウン
//-----------------------
void CPlayer::Down()
{
	//ダウンのアニメーション
	RequestAnim(ANIMID_DOWN, 1.0f, true);

	m_getUpTime++;

	//移動の入力をされたら起き上がりに移行する
	if ((CControllerManager::GetLX(m_padName) != 0.0f ||
		CControllerManager::GetLY(m_padName) != 0.0f) ||
		m_getUpTime >= GET_UP_MAX_TIME)
	{
		m_getUpTime = 0;
		m_state = GET_UP;
	}
}

//-----------------------
//		起き上がり
//-----------------------
void CPlayer::GetUp()
{
	//起き上がりのアニメーション
	RequestAnim(ANIMID_GET_UP,0.8f);

	//アニメーションが終わったら待機状態に戻す
	if (GetAnimEnd() == true)
	{
		m_state = WAIT;
	}
}

//-----------------------
//		死亡
//-----------------------
void CPlayer::Die()
{
	//死亡のアニメーション
	if (RequestAnim(ANIMID_DIE, 0.5f) == true)
	{
		m_dropCoin = static_cast<int>(m_money * MONEY_RESPAWN_RATE);
		m_money -= m_dropCoin;
	}

	//死亡アニメーションが終わったら消える
	if (GetAnimEnd() == true)
	{
		m_isActive = false;
	}
}

//-----------------------
//		  準備完了
//-----------------------
void CPlayer::Ready()
{
	//準備完了のアニメーション
	RequestAnim(ANIMID_READY, 1.0f);
}

//-----------------------
//	  準備完了を解除
//-----------------------
void CPlayer::ReadyOut()
{
	//準備完了をやめるアニメーション
	RequestAnim(ANIMID_READY_OUT, 1.0f);

	//アニメーションが終わったら待機状態に戻す
	if (GetAnimEnd() == true)
	{
		m_state = WAIT;
	}
}

//-----------------------
//		   拍手
//-----------------------
void CPlayer::Clap()
{
	//拍手のアニメーション
	RequestAnim(ANIMID_CLAP, 0.8f,true);
}

//-----------------------
//	   勝利モーション
//-----------------------
void CPlayer::Winner()
{
	//勝利のアニメーション
	RequestAnim(ANIMID_WINNER, 0.8f,true);
}

//-----------------------
//		移動処理
//-----------------------
void CPlayer::Move(float _rotY)
{
	//待機状態と移動状態以外は移動を出来ないようにする
	switch (m_state)
	{
	case WAIT:
	case WALK:
	case AIR:
		break;
	case ATTACK:
		if (m_weaponId != WEAPON_ID_AX)return;
		break;
	default:
		return;
	}

	//コントローラーを使っているか
	bool isController = false;

	if (CControllerManager::GetLY(m_padName) != 0 ||
		CControllerManager::GetLX(m_padName) != 0)
	{
		isController = true;
	}

	float moveSpeed = MOVE_SPEED;

	//移動ベクトル
	VECTOR speed = { 0.0f,0.0f,0.0f };
	//コントローラー用前進後退
	if (isController == true)
	{
		speed.z = CControllerManager::GetLY(m_padName) * moveSpeed;
	}
	//キーボード用前進
	else if (CheckHitKey(KEY_INPUT_W) != 0)
	{
		speed.z = -moveSpeed;
	}
	//キーボード用後退
	else if (CheckHitKey(KEY_INPUT_S) != 0)
	{
		speed.z = moveSpeed;
	}

	//左右にどれだけ移動するか
	//コントローラー用左右移動
	if (isController == true)
	{
		speed.x = -CControllerManager::GetLX(m_padName) * moveSpeed;
	}
	//キーボード用左移動
	else if (CheckHitKey(KEY_INPUT_A) != 0)
	{
		speed.x = moveSpeed;
	}
	//キーボード用右移動
	else if (CheckHitKey(KEY_INPUT_D) != 0)
	{
		speed.x = -moveSpeed;
	}

	////カメラの角度がオールゼロの時に進む速度
	VECTOR defaultDir = { speed.x,0.0f,speed.z };
	//上記を行列に変換
	MATRIX dir = CMyMath::GetTranslateMatrix(defaultDir);
	//Y軸回転行列
	MATRIX mRotY = CMyMath::GetYawMatrix(_rotY);
	//行列の合成
	MATRIX res = CMyMath::MatMult(mRotY, dir);

	//移動をスピードに代入
	m_speed.x = res.m[0][3];
	m_speed.y = res.m[1][3];
	m_speed.z = res.m[2][3];

	if (m_state == ATTACK)return;

	//移動していたら歩きモーションに以降
	if ((m_speed.x != 0.0f ||
		m_speed.z != 0.0f) &&
		m_state != AIR)
	{
		m_state = WALK;
	}

	//移動方向を向く
	if (m_speed.x != 0 || m_speed.z != 0)
		m_rot.y = atan2f(-m_speed.x,-m_speed.z);

}

//-----------------------
//攻撃を呼び出す処理
//-----------------------
void CPlayer::RequestAttack()
{
	if (m_attackNum >= ATTACK_3)return;

	switch (m_state)
	{
	case AIR:
	case WAIT:
	case WALK:
	case ATTACK_OUT:
		break;
	default:
		return;
	}

	//アイテムを持ち上げている状態ではアイテムを投げる
	if (m_itemState == ITEM_STATE_HAVE)
	{
		//攻撃ボタンを押したら投げる
		m_state = ITEM_THROW_IN;

		return;
	}

	//攻撃中なら次に移行する
	if (m_attackNum >= ATTACK_1)
	{
		//最後の攻撃以外は攻撃を進める
		m_attackNum++;
		m_state = ATTACK_IN;

	}
	//攻撃してない時に攻撃前に移行する
	else if(m_attackNum == ATTACK_NONE)
	{
		m_attackNum++;
		m_state = ATTACK_IN;
	}

}

//-----------------------
//ジャンプの呼び出し処理
//-----------------------
void CPlayer::RequestJump()
{
	switch (m_state)
	{
	case WAIT:
	case WALK:
		break;
	default:
		return;
	}

	if ((CControllerManager::IsTrg(BUTTON_A, m_padName) && !m_isFlying) ||
		(CheckHitKey(KEY_INPUT_SPACE) && !m_isFlying))
	{
		m_state = JUMP;

	}

}

//-----------------------
//	回避に移行する処理
//-----------------------
void CPlayer::RequestDodgeroll(float _rotY)
{
	return;

	//アイテムを持ち上げている状態は処理をしない
	if (m_itemState == ITEM_STATE_HAVE)return;

	//待機状態と歩いてる状態以外は処理をしない
	switch (m_state)
	{
	case WAIT:
	case WALK:
	case ATTACK_IN:
	case ATTACK:
	case ATTACK_OUT:
	case ATTACK_CHARGE_IN:

		break;
	default:
		return;
	}

	//コントローラーを使っているか
	bool isController = false;

	if (CControllerManager::GetLY(m_padName) != 0 ||
		CControllerManager::GetLX(m_padName) != 0)
	{
		isController = true;
	}

	//入力方向ベクトル
	VECTOR vec = { 0.0f,0.0f,0.0f };
	//コントローラー用前進後退
	if (isController == true)
	{
		vec.z = CControllerManager::GetLY(m_padName);
	}

	//左右にどれだけ移動するか
	//コントローラー用左右移動
	if (isController == true)
	{
		vec.x = -CControllerManager::GetLX(m_padName);
	}

}

//-----------------------
//	   アイテム処理
//-----------------------
void CPlayer::Item()
{

	//アイテムを持っていなかったら処理をしない
	if (m_itemState != ITEM_STATE_HAVE)return;

	//待機状態と歩いてる状態以外は処理をしない
	switch (m_state)
	{
	case WAIT:
	case WALK:
		break;
	default:
		return;
	}

	//ボタンを押されたらアイテム使用前状態に移行
	if (CheckHitKey(KEY_INPUT_K) != 0 ||
		CControllerManager::IsTrg(BUTTON_A,m_padName) == true)
	{
		m_state = ITEM_USE_IN;
	}

}

//-----------------------
//	  アイテムを拾う
//-----------------------
void CPlayer::PickUpItem()
{
	//空中いるときは攻撃を出せない
	if (m_isFlying == true)return;

	switch (m_state)
	{
	case WAIT:
	case WALK:
		break;
	default:
		return;
	}

	//アイテムを取得・下ろす

	//アイテムをすでに持っている場合はアイテムを下ろす
	if (m_itemState == ITEM_STATE_HAVE)
	{
		m_state = ITEM_PUT_DOWN;
		m_itemState = ITEM_STATE_PUT_DOWN;
	}
	//アイテムを持っていない場合アイテムを取得する
	else
	{
		m_itemState = ITEM_STATE_PICK_UP;
	}

}

//-----------------------
//持っているアイテムの座標を取得
//-----------------------
VECTOR CPlayer::GetItemHavePos()
{
	VECTOR itemPos = MV1GetFramePosition(m_hndl, 18);

	return itemPos;
}

//-----------------------
//	 武器の座標を取得
//-----------------------
VECTOR CPlayer::GetWeaponPos()
{
	VECTOR weaponPos = MV1GetFramePosition(m_hndl, 11);

	return weaponPos;
}

