#pragma once
#include "Game/Base/Object/ObjectBase.h"

//アクターベースクラス
class ActorBase :public ObjectBase {
protected:
	//アニメーション関連変数構造体
	typedef struct {
		float	Frame;		//再生中のアニメのフレーム
		float	EndFrame;		//再生中のアニメの最終フレーム
		float	Speed;		//アニメの再生速度
		int		Hndl;			//アニメーション用ハンドル
		int		ID;			//再生中のアニメ番号
		int		State;		//ループorストップ
		bool	EndFlg;		//再生中のアニメ終了フラグ
	}ANIME_DATA;

	//変数定義
	ANIME_DATA m_AnimeData;

	//再生アニメ種類
	enum {
		ANIME_LOOP,
		ANIME_ENDLOOP
	};

	VECTOR	m_MoveVec;		//移動方向

	bool	m_IsCollision;	//当たり判定実行フラグ

	//アニメーション更新
	void AnimeUpdate();

public:
	//コンストラクタ
	ActorBase();
	//デストラクタ
	virtual ~ActorBase();
	//初期化処理
	virtual void Init();
	//モデル更新処理
	virtual void Update();

	//継承先で使う
	//データ読み込み処理
	virtual void Load() = 0;
	//毎フレーム呼び出す処理
	virtual void Step() = 0;

	//アニメリクエスト
	//アニメーションループ再生
	//_ID		:どのアニメーションを再生するか
	//_Speed	:アニメーションの再生速度
	void RequestLoop(int _ID, float _Speed = 1.0f);

	//アニメーション再生(最終フレームで停止)
	//_ID		:どのアニメーションを再生するか
	//_Speed	:アニメーションの再生速度
	void RequestEndLoop(int _ID, float _Speed = 1.0f);

	//Get
	//移動方向
	VECTOR	GetMoveVec()	const { return m_MoveVec; }
	///Y軸を考慮しない座標Aから座標Bへの方向ベクトルを取得する
	//_VectorA		:基準となる座標
	//_VectorB		:目標になる座標
	//_IsNorm		:FALSE	＝	正規化しない	
	//				:TRUE	＝	正規化する
	//return		:VECTOR型の数値
	VECTOR GetDirectionNotY(VECTOR _VectorA, VECTOR _VectorB, bool _IsNorm = FALSE);

	//Set
	//移動方向
	void	SetMoveVec(VECTOR _MoveVec) { m_MoveVec = _MoveVec; }
	//アニメーションのスピード変化
	void	SetAnimeSpeed(float _AnimeSpeed) { m_AnimeData.Speed = _AnimeSpeed; }
};
