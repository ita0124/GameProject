#pragma once
#include "Game/Base/UIBase/PlayerStatusUIBase/PlayerStatusUIBase.h"

//スキルポイントUIクラス
class SkillPoints :public PlayerStatusUIBase {
public:
	//コンストラクタ
	SkillPoints();

	//デストラクタ
	~SkillPoints();

	//初期化処理
	void Init();

	//データ破棄処理
	void Exit();

	//データ読み込み処理
	void Load();

	//数値更新
	void Update(int _Num);

	//描画処理
	void Draw();
};
