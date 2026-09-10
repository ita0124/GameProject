#pragma once
#include "Game/Base/Object2D/StatusDraw/StatusDrawBase.h"

//スタミナクラス
class Stamina :public StatusDrawBase {
public:
	//コンストラクタ
	Stamina();
	//デストラクタ
	~Stamina();
	//初期化処理
	void Init(VECTOR _Pos,float _MaxStatus);
	//データ読み込み処理
	void Load();
	//描画処理
	void Draw();
private:
	//初期化処理
	void Init();
};

