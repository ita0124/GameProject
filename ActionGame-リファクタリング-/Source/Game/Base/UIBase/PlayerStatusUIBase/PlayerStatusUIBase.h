#pragma once
#include "Game/Base/UIBase/UIBase.h"

class PlayerStatusUIBase :public UIBase {
protected:
	int m_Num;	//プレイヤーの数値を代入する
public:
	//コンストラクタ
	PlayerStatusUIBase();

	//デストラクタ
	virtual ~PlayerStatusUIBase();

	//初期化処理
	virtual void Init();

	//データ破棄処理
	virtual void Exit();

	//データ読み込み処理
	virtual void Load(const char* FilePath);

	//数値更新
	void Update(int _Num);

	//描画処理
	virtual void Draw() = 0;
};
