#pragma once
#include "Comon.h"
#include "Game/Sound/SoundManager.h"
#include "Effekseer/MyEffeckseer.h"

//オブジェクトベースクラス
class ObjectBase {
protected:
	//ゲームオブジェクト種類
	enum TagKinds {
		Object,		//オブジェクト	//3Dモデルを動かすための必要最低限のものは入っている
		Actor,		//アクタ		//アニメーションを行うものはここ
		Character,	//キャラクター	//体力などのステータスがある
		Enemy,		//エネミー		//プレイヤーに攻撃を与えてくるものはここ
	};

	TagKinds	m_Kinds;		//オブジェクト種類

	VECTOR		m_Pos;			//座標
	VECTOR		m_Rot;			//回転率
	VECTOR		m_Scale;		//拡縮
	VECTOR		m_Size;			//サイズ
	int			m_Hndl;			//画像ハンドル
	float		m_Rad;			//半径
	bool		m_IsActive;		//生存フラグ
	float		m_Gravity;		//重力
	bool		m_IsFlying;		//空中判定
	bool		m_IsGravity;	//重力処理をするか
	ObjectBase* m_Owner;		//オーナーオブジェクト

	int			m_EffectHndl;	//エフェクトハンドル
	bool		m_IsEffect;		//エフェクト出現判定

	//重力処理
	void Gravity();
public:
	//コンストラクタ
	ObjectBase();
	//デストラクタ
	virtual ~ObjectBase();
	//初期化処理
	virtual void Init();
	//データ破棄処理
	virtual void Exit();
	// データ読み込み処理
	void Load(const char* FilePath);
	//モデル更新処理
	virtual void Update();
	//描画処理
	virtual void Draw();
	//継承先で使う
	//データ読み込み処理
	virtual void Load() = 0;
	//毎フレーム呼び出す処理
	virtual void Step() = 0;

	//Get
	//ゲームオブジェクト種類
	TagKinds	GetKinds()	const { return m_Kinds; }
	//座標
	VECTOR		GetPos()	const { return m_Pos; }
	//回転率	
	VECTOR		GetRot()	const { return m_Rot; }
	//拡縮
	VECTOR		GetScale()	const { return m_Scale; }
	//サイズ
	VECTOR		GetSize()	const { return m_Size; }
	//ハンドル
	int			GetHndl()	const { return m_Hndl; }
	//半径
	float		GetRad()	const { return m_Rad; }
	//生存フラグ
	bool		GetIsActive()	const { return m_IsActive; }
	//指定したフレームの座標を渡す
	VECTOR		GetFramePos(int _Hndl, int _FramwNum)	const { return MV1GetFramePosition(_Hndl, _FramwNum); }
	//モデルの中心を取る
	VECTOR		GetCenter();

	//Set
	//座標
	void		SetPos(VECTOR _Pos) { m_Pos = _Pos; }
	//回転率
	void		SetRot(VECTOR _Rot) { m_Rot = _Rot; }
	//拡縮
	void		SetScale(VECTOR _Scale) { m_Scale = _Scale; }
	//サイズ
	void		SetSize(VECTOR _Size) { m_Size = _Size; }
	//半径
	void		SetRad(float _Rad) { m_Rad = _Rad; }
	//生存フラグ
	void		SetIsActive(bool _IsActive) { m_IsActive = _IsActive; }

	//Add
	//座標加算
	void		AddPos(VECTOR _Pos) { m_Pos = VAdd(m_Pos, _Pos); }

	//Reset
	//重力リセット
	virtual void GravityReset();
};
