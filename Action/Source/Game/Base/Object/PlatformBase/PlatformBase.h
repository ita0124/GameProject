#pragma once
#include "Game/Base/Object/ObjectBase.h"

//ステージベースクラス
class PlatformBase :public ObjectBase {
public:
	//足場オブジェクト種類
	enum TagPlatformKinds {
		NORMAL,			//通常足場
		MOVING,			//動く足場
		FALLING,		//落ちる足場
		WALL,			//壁
		RESPAWN,		//リスポーン
		GOAL,			//ゴール
		BARRIER,		//バリア
		FLOWERS,		//花
		GRASS,			//草
		FENCE,			//塀
		TREE,			//木
		ARROW,			//矢印
		MUSHROOMS,		//きのこ
		FENCE_BROKEN,	//壊れた塀
		COIN_GOLD,		//金コイン

		PLATFORM_NUM
	};
protected:
	//リクエスト時に最低限必要なデータ構造体
	typedef struct {
		VECTOR	Pos;			//座標
		VECTOR	Rot;			//回転率
		VECTOR	Scale;			//拡縮
		VECTOR	Size;			//サイズ
		int		GimmickType;	//ギミックタイプ

	}NORMAL_REQUEST_DATA;

	//動く床をリクエストする時必要なデータ構造体
	typedef struct {
		VECTOR	FirstPos;	//初期座標
		VECTOR	EndPos;		//終端座標
		float	MoveSpeed;	//移動速度
	}MOVINGPLATFORM_REQUEST_DATA;

	NORMAL_REQUEST_DATA			m_NormalRequestData;			//リクエスト時に最低限必要なデータ構造体
	MOVINGPLATFORM_REQUEST_DATA	m_MovingPlatformRequestData;	//動く床をリクエストする時必要なデータ構造体

	TagPlatformKinds			m_PlatformKinds;				//足場オブジェクト種類

	int							m_GimmickType;					//作用するギミックタイプ

	bool						m_ObjectPush;					//オブジェクトを押し戻すか
	bool						m_PlayerPush;					//プレイヤーを押し戻すか
	bool						m_EnemyPush;					//敵を押し戻すか
	bool						m_IsItem;						//アイテム系か

public:
	//コンストラクタ
	PlatformBase();
	//デストラクタ
	virtual ~PlatformBase();
	//初期化処理
	virtual void Init();
	// データ読み込み処理
	void Load(const int _Hndl);
	//リクエスト
	void Request(const NORMAL_REQUEST_DATA& _NormalRequestData);
	//Get
	//基本データ構造体
	NORMAL_REQUEST_DATA			GetNormalRequestData() const { return m_NormalRequestData; }
	//動く床データ構造体
	MOVINGPLATFORM_REQUEST_DATA GetMovingPlatformRequestData() const { return m_MovingPlatformRequestData; }
	//足場オブジェクト種類
	TagPlatformKinds			GetPlatformKinds()	const { return m_PlatformKinds; }
	//作用するギミックタイプ
	int							GetGimmickType()	const { return m_GimmickType; }
	//オブジェクトを押し戻すか
	bool						GetObjectPush()const { return m_ObjectPush; }
	//プレイヤーを押し戻すか
	bool						GetPlayerPush()const { return m_PlayerPush; }
	//敵を押し戻すか
	bool						GetEnemyPush()const { return m_EnemyPush; }
	//アイテム系か
	bool						GetIsItem()const { return m_IsItem; };

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
	//データ構造体 ギミックタイプ
	void SetNormalRequestDataGimmickType(const int& _GimmickType) { m_NormalRequestData.GimmickType = _GimmickType; }
	//動く床データ構造体 初期座標
	void SetMovingPlatformRequestDataFirstPosX(const float& _FirstPosX) { m_MovingPlatformRequestData.FirstPos.x = _FirstPosX; }
	void SetMovingPlatformRequestDataFirstPosY(const float& _FirstPosY) { m_MovingPlatformRequestData.FirstPos.y = _FirstPosY; }
	void SetMovingPlatformRequestDataFirstPosZ(const float& _FirstPosZ) { m_MovingPlatformRequestData.FirstPos.z = _FirstPosZ; }
	//動く床データ構造体 終端座標
	void SetMovingPlatformRequestDataEndPosX(const float& _EndPosX) { m_MovingPlatformRequestData.EndPos.x = _EndPosX; }
	void SetMovingPlatformRequestDataEndPosY(const float& _EndPosY) { m_MovingPlatformRequestData.EndPos.y = _EndPosY; }
	void SetMovingPlatformRequestDataEndPosZ(const float& _EndPosZ) { m_MovingPlatformRequestData.EndPos.z = _EndPosZ; }
	//動く床データ構造体 移動速度
	void SetMovingPlatformRequestDataMoveSpeed(const float& _MoveSpeed) { m_MovingPlatformRequestData.MoveSpeed = _MoveSpeed; }
	//オブジェクトを押し戻すか
	void SetObjectPush(const bool& _ObjectPush) { m_ObjectPush = _ObjectPush; }
	//プレイヤーを押し戻すか
	void SetPlayerPush(const bool& _PlayerPush) { m_PlayerPush = _PlayerPush; }
	//敵を押し戻すか
	void SetEnemyPush(const bool& _EnemyPush) { m_EnemyPush = _EnemyPush; }
};
