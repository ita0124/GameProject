#pragma once
#include "Game/Base/Object/PlatformBase/PlatformBase.h"
#include "Lib/LoadMaterial/LoadMaterial.h"

//リスポーン足場
class RespawnPlatform :public PlatformBase {
private:
	ObjectBase *m_Object;
	bool		m_IsHit;
public:
	//コンストラクタ
	RespawnPlatform();
	//デストラクタ
	~RespawnPlatform();
	//初期化処理
	void Init();
	//毎フレーム呼び出す処理
	void Step();
	//当たり判定後の処理(当たっている場合)
	void HitCalc(ObjectBase* _Object);
};


