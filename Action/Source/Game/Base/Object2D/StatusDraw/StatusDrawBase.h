#pragma once
#include "Game/Base/Object2D/Object2DBase.h"

//ステータス描画ベース
class StatusDrawBase :public Object2DBase {
protected:
	float	m_Status;				//取得したステータスを保存する
	float	m_MaxStatus;			//最大ステータス量を保存
	int		m_ImageSpacing;			// 次の画像との間隔
public:
	//画像
	enum TagGraphicsNum {
		STATUS,			//ステータス
		MAX_STATUS,		//最大ステータス
		ICON,			//アイコン

		GRAPHICS_NUM
	};

	//コンストラクタ
	StatusDrawBase();
	//デストラクタ
	~StatusDrawBase();
	//初期化処理
	virtual void Init();
	//描画処理
	virtual void Draw();
	//セット
	//ステータスを取得
	void SetStatus(float _Status) { m_Status = _Status; }
};

