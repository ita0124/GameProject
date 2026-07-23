#pragma once
#include "Game/Base/Object2D/Object2DBase.h"

//スタミナUIクラス
class Stamina :public Object2DBase {
private:
	int m_Stamina;				//取得したスタミナを保存する
public:
	//画像
	enum TagGraphicsNum {
		STAMINA,		//スタミナ画像
		GRAY,			//灰色画像

		GRAPHICS_NUM
	};

	//コンストラクタ
	Stamina();
	//デストラクタ
	~Stamina();
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
	void SetStamina(int _Stamina) { m_Stamina = _Stamina; }
};

