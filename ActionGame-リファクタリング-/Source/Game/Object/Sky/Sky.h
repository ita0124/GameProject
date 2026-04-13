#pragma once
#include "Game/Base/Object/ObjectBase.h"

//天球クラス
class Sky :public ObjectBase {
public:
	//コンストラクタ
	Sky();
	//デストラクタ
	~Sky();
	//初期化処理
	void Init();
	//データ読み込み処理
	void Load();
	//毎フレーム呼び出す処理
	void Step();
};
