#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"

//動く足場
class MovingPlatform :public PlatformBase {
private:
	bool	m_IsEndPos;	//終端座標にたどり着いたか
	VECTOR	m_MoveDir;	//移動方向ベクトル
	float	m_PrevLen;	//前フレームの座標から終端座標までの距離を保存する
public:
	//コンストラクタ
	MovingPlatform();
	//デストラクタ
	~MovingPlatform();
	//初期化処理
	void Init();
	//データ読み込み処理
	void Load();
	//毎フレーム呼び出す処理
	void Step();

	//Get
	VECTOR GetMoveDir() { return m_MoveDir; }
};


