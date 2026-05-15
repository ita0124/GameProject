#pragma once
#include "Comon.h"
#include "Game/Sound/SoundManager.h"
#include "Effekseer/MyEffeckseer.h"

//オブジェクトベースクラス
class ObjectBase {
public:
	//ゲームオブジェクト種類
	enum TagKinds {
		Object,		//オブジェクト	//3Dモデルを動かすための必要最低限のものは入っている
		Actor,		//アクタ		//アニメーションを行うものはここ
		Character,	//キャラクター	//体力などのステータスがある
		Enemy,		//エネミー		//プレイヤーに攻撃を与えてくるものはここ
	};

	//オブジェクトの形状
	enum TagShape {
		BALL,
		BOX,

		SHAPE_NUM
	};
protected:
	TagKinds	m_Kinds;		//オブジェクト種類

	VECTOR		m_Pos;			//座標
	VECTOR		m_Rot;			//回転率
	VECTOR		m_Scale;		//拡縮
	VECTOR		m_Size;			//サイズ
	VECTOR		m_PrevPos;		//前フレーム座標
	VECTOR		m_PlatformVec;	//足場の動く移動方向
	int			m_Hndl;			//画像ハンドル
	float		m_Rad;			//半径
	bool		m_IsActive;		//生存フラグ
	bool		m_IsCollision;	//当たり判定実行フラグ
	bool		m_IsPush;		//押し出し判定を行うか
	float		m_Gravity;		//重力
	bool		m_IsGravity;	//重力処理をするか
	ObjectBase* m_Owner;		//オーナーオブジェクト

	int			m_EffectHndl;	//エフェクトハンドル
	bool		m_IsEffect;		//エフェクト出現判定
	//重力処理
	virtual void GravityManager();
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
	//当たり判定後の処理(当たっている場合)
	virtual void HitCalc() {}
	//当たり判定後の処理(当たっていない場合)
	virtual void NotHitCalc() {}

	//Get
	//ゲームオブジェクト種類
	TagKinds	GetKinds()			const { return m_Kinds; }
	//座標
	VECTOR		GetPos()			const { return m_Pos; }
	//回転率	
	VECTOR		GetRot()			const { return m_Rot; }
	//拡縮
	VECTOR		GetScale()			const { return m_Scale; }
	//サイズ
	VECTOR		GetSize()			const { return m_Size; }
	//前フレーム座標
	VECTOR		GetPrevPos()		const { return m_PrevPos; }
	//ハンドル
	int			GetHndl()			const { return m_Hndl; }
	//半径
	float		GetRad()			const { return m_Rad; }
	//生存フラグ
	bool		GetIsActive()		const { return m_IsActive; }
	//重力処理フラグ
	bool		GetIsGravity()		const { return m_IsGravity; }
	//当たり判定実行フラグ
	bool		GetIsCollision()	const { return m_IsCollision; }
	//押し出し判定を行うか
	bool		GetIsPush()			const { return m_IsPush; }
	//指定したフレームの座標を渡す
	VECTOR		GetFramePos(int _Hndl, int _FramwNum)	const { return MV1GetFramePosition(_Hndl, _FramwNum); }
	//モデルの中心を取る
	VECTOR		GetCenter(TagShape _Shape);
	//モデルの中心を取る
	VECTOR		GetPrevCenter(TagShape _Shape);

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
	//重力処理フラグ
	void		SetIsGravity(bool _IsGravity) { m_IsGravity = _IsGravity; }
	//足場の動く移動方向
	void		SetPlatformVec(VECTOR _PlatformVec) { m_PlatformVec = _PlatformVec; }

	//Add
	//座標加算
	void		AddPos(VECTOR _Pos) { m_Pos = VAdd(m_Pos, _Pos); }

	//Reset
	//重力リセット
	virtual void GravityReset();
};
