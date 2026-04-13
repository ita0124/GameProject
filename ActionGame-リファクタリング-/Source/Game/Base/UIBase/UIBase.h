#pragma once
#include "Comon.h"

class UIBase {
protected:
	VECTOR m_Pos;			//座標

	vector<int> m_Hndl;		//画像ハンドル
public:
	//コンストラクタ
	UIBase();

	//デストラクタ
	virtual ~UIBase();

	//初期化処理
	virtual void Init();

	//データ破棄処理
	virtual void Exit();

	//データ読み込み処理
	virtual void Load(const char* FilePath);

	//描画処理
	virtual void Draw() = 0;
};
