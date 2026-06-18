#pragma once
#include "Game/Base/Object/ObjectBase.h"

//剣クラス
class Sword :public ObjectBase {
public:
	//ボーン番号enum
	enum FrameNamber {
		ARMATURE,		//アーマチュア
		//ここからボーン
		POMMEL,			//柄頭
		GRIP,			//柄
		GRIP001,		//柄１
		GUARD,			//鍔
		BLADE,			//刃
		BLADE001,		//刃１
		BLADE002,		//刃２
		VLADEEND,		//刃先
		//これ以下は使うことがないと思うので省略

		FARAM_NUM
	};

	//コンストラクタ
	Sword();
	//デストラクタ
	~Sword();
	//初期化処理
	void Init(ObjectBase *_Object);
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
