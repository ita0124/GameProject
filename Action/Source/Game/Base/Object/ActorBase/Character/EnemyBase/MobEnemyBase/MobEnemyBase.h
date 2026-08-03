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
		VECTOR	Pos;			//座標
		VECTOR	Rot;			//回転率
		VECTOR	Scale;			//拡縮
		VECTOR	Size;			//サイズ
		int		GimmickType;	//ギミックタイプ
	}REQUEST_DATA;

	REQUEST_DATA			m_RequestData;			//リクエスト時に最低限必要なデータ構造体

	TagMobEnemyKinds		m_MobEnemyKinds;		//足場オブジェクト種類

	int						m_GimmickType;			//作用するギミックタイプ
	//重力処理
	virtual void GravityManager();
	//ノックバックデータ数値代入
	void SetKnockBackData(float _Power, VECTOR _Pos);
public:
	//コンストラクタ
	MobEnemyBase();
	//デストラクタ
	virtual ~MobEnemyBase();
	//初期化処理
	virtual void Init();
	//リクエスト
	void Request(const REQUEST_DATA& _RequestData);
	//Get
	//基本データ構造体
	REQUEST_DATA		GetRequestData() const { return m_RequestData; }
	//敵種類
	TagMobEnemyKinds	GetMobEnemyKinds()	const { return m_MobEnemyKinds; }
	//作用するギミックタイプ
	int					GetGimmickType()	const { return m_GimmickType; }

	//Set
	//データ構造体 座標
	void SetRequestDataPosX(const float& _PosX) { m_RequestData.Pos.x = _PosX; }
	void SetRequestDataPosY(const float& _PosY) { m_RequestData.Pos.y = _PosY; }
	void SetRequestDataPosZ(const float& _PosZ) { m_RequestData.Pos.z = _PosZ; }
	//データ構造体 回転値
	void SetRequestDataRotX(const float& _RotX) { m_RequestData.Rot.x = _RotX; }
	void SetRequestDataRotY(const float& _RotY) { m_RequestData.Rot.y = _RotY; }
	void SetRequestDataRotZ(const float& _RotZ) { m_RequestData.Rot.z = _RotZ; }
	//データ構造体 拡縮
	void SetRequestDataScaleX(const float& _ScaleX) { m_RequestData.Scale.x = _ScaleX; }
	void SetRequestDataScaleY(const float& _ScaleY) { m_RequestData.Scale.y = _ScaleY; }
	void SetRequestDataScaleZ(const float& _ScaleZ) { m_RequestData.Scale.z = _ScaleZ; }
	//データ構造体 サイズ
	void SetRequestDataSizeX(const float& _SizeX) { m_RequestData.Size.x = _SizeX; }
	void SetRequestDataSizeY(const float& _SizeY) { m_RequestData.Size.y = _SizeY; }
	void SetRequestDataSizeZ(const float& _SizeZ) { m_RequestData.Size.z = _SizeZ; }
	//データ構造体 ギミックタイプ
	void SetRequestDataGimmickType(const int& _GimmickType) { m_RequestData.GimmickType = _GimmickType; }
};
