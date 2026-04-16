#pragma once
#include<DxLib.h>

//オブジェクトの名前
enum tagObjectName
{
	OBJECT_PLAYER,			//プレイヤー
	OBJECT_ITEM,			//アイテム
	OBJECT_ATTACK,			//攻撃の当たり判定
	OBJECT_SHOT,			//弾
	OBJECT_FOV,				//CPUの視界の当たり判定
	
	OBJECT_NAME_NUM,		//名前の数
};

//オブジェクトのタイプ
enum tagObjectType
{
	OBJECT_TYPE_BOX,		//ボックスタイプ
	OBJECT_TYPE_SPHERE,		//球タイプ

	OBJECT_TYPE_NUM,		//タイプの数
};

//3Dオブジェクトの基底クラス
class CObject{
protected:
	VECTOR			m_pos;			//座標
	VECTOR			m_speed;		//速度
	VECTOR			m_rot;			//回転角度
	VECTOR			m_scale;		//拡大縮小率
	VECTOR			m_size;			//大きさ
	int				m_hndl;			//オブジェクトのハンドル
	float			m_rad;			//半径
	float			m_gravity;		//重力
	bool			m_isActive;		//生存フラグ
	bool			m_isGravity;	//重力処理をするかどうかフラグ
	bool			m_isFlying;		//空中にいるかどうかフラグ
	bool			m_isPushed;		//押し出し処理をするかどうかフラグ
	tagObjectName	m_objectName;	//オブジェクトの名前
	tagObjectType	m_objectType;	//オブジェクトのタイプ
	CObject*		m_owner;		//オーナーオブジェクト

public:
	//コンストラクタ・デストラクタ
	CObject();
	virtual ~CObject();

	//初期化
	virtual void Init();
	//終了処理
	virtual void Exit();
	//毎フレームする処理
	virtual void Step();
	//数値の更新
	virtual void Update();
	//オブジェクトの描写
	virtual void Draw();
	//オブジェクトのロード
	virtual void Load();

	//モデルデータロード
	//_filePath		:読み込みたいモデルデータパス
	//return	true:ロード成功		false:ロード失敗
	bool LoadModel(const char* _filePath);

	//モデルコピーロード
	//_originHndl	:コピー元のモデルのハンドル
	//return	true:ロード成功		false:ロード失敗
	bool DuplicateModel(int _originHndl);
	
	//モデルデータ削除
	void DeleteModel();

	//当たり判定後の処理
	virtual void HitCalc(CObject* _hitObject);

	//マップに触れたとき
	virtual void HitMapCalc();

	//重力リセット
	virtual void GravityReset();

	//取得・設定---------------------
	//座標を取得
	VECTOR GetPos() { return m_pos; }
	//座標を設定
	void SetPos(VECTOR _pos) { m_pos = _pos; }

	//移動後の座標を取得
	VECTOR GetMovePos() { return VAdd(GetCenter(), m_speed); }

	//速度を取得
	VECTOR GetSpeed() { return m_speed; }
	//速度を設定(floot)
	void SetSpeed(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) 
	{
		m_speed.x = _x;
		m_speed.y = _y;
		m_speed.z = _z;
	}
	//速度を設定(VECTOR)
	void SetSpeed(VECTOR _speed) { m_speed = _speed; }

	//回転角度を取得
	VECTOR GetRot() { return m_rot; }
	//回転角度を設定(float)
	void SetRot(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
	{
		m_rot.x = _x;
		m_rot.y = _y;
		m_rot.z = _z;
	}
	//回転角度を設定(VECTOR)
	void SetRot(VECTOR _rot) { m_rot = _rot; }

	//大きさを取得
	VECTOR GetSize() { return m_size; }
	//大きさを設定
	void SetSize(VECTOR _size) { m_size = _size; }

	//半径を取得
	float GetRad() { return m_rad; }
	//半径を設定
	void SetRad(float _rad) { m_rad = _rad; }

	//中心座標を取得
	virtual VECTOR GetCenter();

	//生存フラグを取得
	bool GetActive() { return m_isActive; }
	//生存フラグを設定
	void SetActive(bool _isActive) { m_isActive = _isActive; }

	//押し出し処理をするかどうかを取得
	bool GetIsPushed() { return m_isPushed; }

	//ハンドルを取得
	int GetHndl() { return m_hndl; }

	//オブジェクトの名前を取得
	tagObjectName GetObjectName() { return m_objectName; }

	//オブジェクトのタイプを取得
	tagObjectType GetObjectType() { return m_objectType; }
	//オブジェクトのタイプを設定
	void SetObjectType(tagObjectType _type) { m_objectType = _type; }

	//-------------------------------

protected:
	
	//速度のリセット
	void ResetSpeed();

	//重力処理
	void Gravity();

};

