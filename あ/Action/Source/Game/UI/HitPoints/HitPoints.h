#pragma once
#include "Game/Base/Object2D/Object2DBase.h"

//体力UIクラス
class HitPoints :public Object2DBase {
private:
	int m_HitPoints;				//取得した体力を保存する
public:
	//画像
	enum TagGraphicsNum {
		HIT_POINTS,		//体力画像
		GRAY,			//灰色画像

		GRAPHICS_NUM
	};

	//コンストラクタ
	HitPoints();
	//デストラクタ
	~HitPoints();
	//初期化処理
	void Init();
	//データ読み込み処理
	void Load();
	//数値更新
	void Step();
	//描画処理
	void Draw();
	//セット
	//体力を取得
	void SetHitPoints(int _HitPoints) { m_HitPoints = _HitPoints; }
};

