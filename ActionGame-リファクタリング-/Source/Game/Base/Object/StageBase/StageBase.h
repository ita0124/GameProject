#pragma once
#include "Game/Base/Object/ObjectBase.h"

//ステージベースクラス
class StageBase :public ObjectBase {
protected:

	typedef struct {
		VECTOR	Pos;		//座標
		VECTOR	Rot;		//回転率
		VECTOR	Scale;		//拡縮
		VECTOR	Size;		//サイズ
	}NORMAL_REQUEST_DATA;

	NORMAL_REQUEST_DATA	m_NormalRequestData;	//リクエスト時に最低限必要なデータ構造体
public:
	//コンストラクタ
	StageBase();
	//デストラクタ
	virtual ~StageBase();
	//初期化処理
	virtual void Init();
	//リクエスト
	bool Request(NORMAL_REQUEST_DATA _NormalRequestData);
	//Get
	//基本データ構造体
	NORMAL_REQUEST_DATA	GetNormalRequestData() { return m_NormalRequestData; }
};
