#pragma once
#include "Game/Base/Object2D/Object2DBase.h"

//スキルポイントUIクラス
class SkillPoints :public Object2DBase {
private:
	int m_SkillPoints;				//取得したスキルポイントを保存する
public:
	//画像
	enum TagGraphicsNum {
		SKILL_POINTS,	//スキルポイント画像
		GRAY,			//灰色画像

		GRAPHICS_NUM
	};

	//コンストラクタ
	SkillPoints();
	//デストラクタ
	~SkillPoints();
	//初期化処理
	void Init();
	//データ読み込み処理
	void Load();
	//数値更新
	void Step();
	//描画処理
	void Draw();
	//セット
	//スキルポイントを取得
	void SetSkillPoints(int _SkillPoints) { m_SkillPoints = _SkillPoints; }
};
