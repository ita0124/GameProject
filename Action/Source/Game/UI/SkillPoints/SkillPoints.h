#pragma once
#include "Game/Base/Object2D/StatusDraw/StatusDrawBase.h"

//スキルポイントクラス
class SkillPoints :public StatusDrawBase {
public:
	//コンストラクタ
	SkillPoints();
	//デストラクタ
	~SkillPoints();
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

