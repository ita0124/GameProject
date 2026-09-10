#pragma once
#include "Game/Base/Object/ObjectBase.h"
#include "Game/Manager/MobEnemy/MobEnemyManager.h"
#include "Game/Manager/Platform/PlatformManager.h"

//ステージベースクラス
class GimmickBase :public ObjectBase {
public:
	//ギミックオブジェクト種類
	enum TagGimmickKinds {
		BARRIERACTIVATOR,

		GIMMICK_NUM
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

	REQUEST_DATA				m_RequestData;					//リクエスト時に必要なデータ構造体

	TagGimmickKinds				m_GimmickKinds;					//ギミックオブジェクト種類

	int							m_GimmickType;					//作用させるギミックタイプ
public:
	//コンストラクタ
	GimmickBase();
	//デストラクタ
	virtual ~GimmickBase();
	//初期化処理
	virtual void Init();
	//毎フレーム呼び出す処理
	virtual void Step() {};
	//毎フレーム呼び出す処理
	virtual void Step(MobEnemyManager _MobEnemyManager, PlatformManager _PlatformManager) {};
	//リクエスト
	void Request(const REQUEST_DATA& _RequestData);
	//Get
	//基本データ構造体
	REQUEST_DATA	GetRequestData()	const { return m_RequestData; }
	//ギミックオブジェクト種類
	TagGimmickKinds GetGimmickKinds()	const { return m_GimmickKinds; }

	//Set
	//基本データ構造体 座標
	void SetRequestDataPosX(const float& _PosX) { m_RequestData.Pos.x = _PosX; }
	void SetRequestDataPosY(const float& _PosY) { m_RequestData.Pos.y = _PosY; }
	void SetRequestDataPosZ(const float& _PosZ) { m_RequestData.Pos.z = _PosZ; }
	//基本データ構造体 回転値
	void SetRequestDataRotX(const float& _RotX) { m_RequestData.Rot.x = _RotX; }
	void SetRequestDataRotY(const float& _RotY) { m_RequestData.Rot.y = _RotY; }
	void SetRequestDataRotZ(const float& _RotZ) { m_RequestData.Rot.z = _RotZ; }
	//基本データ構造体 拡縮
	void SetRequestDataScaleX(const float& _ScaleX) { m_RequestData.Scale.x = _ScaleX; }
	void SetRequestDataScaleY(const float& _ScaleY) { m_RequestData.Scale.y = _ScaleY; }
	void SetRequestDataScaleZ(const float& _ScaleZ) { m_RequestData.Scale.z = _ScaleZ; }
	//基本データ構造体 サイズ
	void SetRequestDataSizeX(const float& _SizeX) { m_RequestData.Size.x = _SizeX; }
	void SetRequestDataSizeY(const float& _SizeY) { m_RequestData.Size.y = _SizeY; }
	void SetRequestDataSizeZ(const float& _SizeZ) { m_RequestData.Size.z = _SizeZ; }
	//データ構造体 ギミックタイプ
	void SetRequestDataGimmickType(const int& _GimmickType) { m_RequestData.GimmickType = _GimmickType; }
};
