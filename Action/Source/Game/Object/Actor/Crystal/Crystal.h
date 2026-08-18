#pragma once
#include "../../Character.h"
#include "../../../../../Lib/LoadMaterial/LoadMaterial.h"

class EnergyPoint :public Character {
private:
	enum TagState {
		WAIT,
		SPIN,
		DEATH,

		STATE_MAX
	};

	TagState m_State;

	enum TagAnimeState {
		ANIME_WAIT,
		ANIME_SPAWN,
		ANIME_DEATH,

		ANIME_MAX
	};
public:
	//コンストラクタ・デストラクタ
	EnergyPoint();
	~EnergyPoint();

	//初期化処理
	void Init();

	//データ破棄処理
	void Exit();

	//モデル更新処理
	void Update();

	//描画処理
	void Draw();

	//データ読み込み処理
	void Load() {};

	//データ読み込み処理
	void Load(int _Hndl);

	//毎フレーム呼び出す処理
	void Step();

	//当たり判定の処理
	void HitCalc(float _Power);

	//リクエスト
	bool Request(const VECTOR& _Pos);
};
