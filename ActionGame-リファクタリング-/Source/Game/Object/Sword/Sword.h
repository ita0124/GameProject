#pragma once
#include "Game/Base/Object/ObjectBase.h"

//剣クラス
class Sword :public ObjectBase {
public:
	//コンストラクタ
	Sword();
	//デストラクタ
	~Sword();
	//初期化処理
	void Init(ObjectBase _Object);
	//データ読み込み処理
	void Load()override;
	//毎フレーム呼び出す処理
	void Step()override;
private:
	//初期化処理
	void Init()override;
};
