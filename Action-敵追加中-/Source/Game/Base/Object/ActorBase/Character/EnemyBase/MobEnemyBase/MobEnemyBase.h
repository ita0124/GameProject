#pragma once
#include "Game/Base/Object/ActorBase/Character/EnemyBase/EnemyBase.h"

//モブ敵ベースクラス
class MobEnemyBase :public EnemyBase {
public:
	//モブ敵オブジェクト種類
	enum TagMobEnemyKinds {
		BOAR,
		WOLF,

		PLATFORM_NUM
	};
protected:
	//リクエスト時に必要なデータ構造体
	typedef struct {
		VECTOR	Pos;		//座標
		VECTOR	Rot;		//回転率
		VECTOR	Scale;		//拡縮
		VECTOR	Size;		//サイズ
	}REQUEST_DATA;

	REQUEST_DATA			m_NormalRequestData;			//リクエスト時に最低限必要なデータ構造体

	TagMobEnemyKinds		m_MobEnemyKinds;				//足場オブジェクト種類
	//重力処理
	virtual void GravityManager();
public:
	//コンストラクタ
	MobEnemyBase();
	//デストラクタ
	virtual ~MobEnemyBase();
	//初期化処理
	virtual void Init();
	//リクエスト
	void Request(const REQUEST_DATA& _NormalRequestData);
	//Get
	//基本データ構造体
	REQUEST_DATA	GetNormalRequestData() const { return m_NormalRequestData; }
	//足場オブジェクト種類
	TagMobEnemyKinds	GetPlatformKinds()	const { return m_MobEnemyKinds; }

	//Set
	//基本データ構造体 座標
	void SetNormalRequestDataPosX(const float& _PosX) { m_NormalRequestData.Pos.x = _PosX; }
	void SetNormalRequestDataPosY(const float& _PosY) { m_NormalRequestData.Pos.y = _PosY; }
	void SetNormalRequestDataPosZ(const float& _PosZ) { m_NormalRequestData.Pos.z = _PosZ; }
	//基本データ構造体 回転値
	void SetNormalRequestDataRotX(const float& _RotX) { m_NormalRequestData.Rot.x = _RotX; }
	void SetNormalRequestDataRotY(const float& _RotY) { m_NormalRequestData.Rot.y = _RotY; }
	void SetNormalRequestDataRotZ(const float& _RotZ) { m_NormalRequestData.Rot.z = _RotZ; }
	//基本データ構造体 拡縮
	void SetNormalRequestDataScaleX(const float& _ScaleX) { m_NormalRequestData.Scale.x = _ScaleX; }
	void SetNormalRequestDataScaleY(const float& _ScaleY) { m_NormalRequestData.Scale.y = _ScaleY; }
	void SetNormalRequestDataScaleZ(const float& _ScaleZ) { m_NormalRequestData.Scale.z = _ScaleZ; }
	//基本データ構造体 サイズ
	void SetNormalRequestDataSizeX(const float& _SizeX) { m_NormalRequestData.Size.x = _SizeX; }
	void SetNormalRequestDataSizeY(const float& _SizeY) { m_NormalRequestData.Size.y = _SizeY; }
	void SetNormalRequestDataSizeZ(const float& _SizeZ) { m_NormalRequestData.Size.z = _SizeZ; }
};
