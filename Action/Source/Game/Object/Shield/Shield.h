#pragma once
#include "Game/Base/Object/ObjectBase.h"

//盾クラス
class Shield :public ObjectBase {
public:
	//コンストラクタ
	Shield();
	//デストラクタ
	~Shield();
	//初期化処理
	void Init(ObjectBase* _Object);
	//データ破棄処理
	void Exit()override;
	//データ読み込み処理
	void Load()override;
	//毎フレーム呼び出す処理
	void Step()override;
private:
	//初期化処理
	void Init()override;
};
