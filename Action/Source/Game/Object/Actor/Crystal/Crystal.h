#pragma once
#include "Game/Base/Object/ActorBase/Character/CharacterBase.h"
#include "Lib/LoadMaterial/LoadMaterial.h"

class Crystal :public CharacterBase {
public:
	enum TagState {
		IDEL,				//待機
		ROTATION,			//回転

		STATE_MAX
	};

	enum TagAnimeState {
		ANIME_IDEL,			//待機
		ANIME_ROTATION,		//回転

		ANIME_MAX
	};
private:
	TagState	m_State;		//状態変数

	int			m_DamageTime;	//ダメージ処理の継続時間

	float		m_FloatCount;	//浮遊モーション用カウント

	//待機
	void Idel();
	//回転
	void Rotarion();
public:
	//コンストラクタ
	Crystal();
	//デストラクタ
	~Crystal();
	//初期化処理
	void Init();
	//データ読み込み処理
	void Load();
	//毎フレーム呼び出す処理
	void Step();
	//当たり判定後の処理(当たっている場合)
	void HitCalc(ObjectBase* _Object);
	//リクエスト
	bool Request(const VECTOR& _Pos);
};
