#pragma once
#include "Game/Base/Object/ObjectBase.h"

//ステージベースクラス
class StageBase :public ObjectBase {
public:
	//コンストラクタ
	StageBase();
	//デストラクタ
	virtual ~StageBase();
	//リクエスト
	//_Pos	:座標指定
	//_Rot	:角度設定(360°の方)
	virtual bool Request(const VECTOR&_Pos,const VECTOR &_Rot);
};
