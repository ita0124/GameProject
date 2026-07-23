#pragma once
#include "Game/Base/Object/ActorBase/Character/CharacterBase.h"
#include "Lib/LoadMaterial/LoadMaterial.h"
#include <vector>
#include <iterator>

//エネミーベースクラス
class EnemyBase :public CharacterBase {
protected:
	VECTOR	m_PlayerPos;					//プレイヤー座標

	int		m_NextActionTime;				//次の行動までの待機時間
	int		m_DownTime;						//ダウン状態継続時間

	int		m_FrameNumber;					//最大ボーン数を保存

	std::vector<FRAME_DATA>m_FrameData;		//ボーン分の情報を格納
public:
	//コンストラクタ
	EnemyBase();
	//デストラクタ
	~EnemyBase();
	//初期化処理
	virtual void Init();

	//Get
	//最大ボーン数を保存
	int		GetFrameNumber() { return m_FrameNumber; }
	//指定のボーン座標を取得する
	VECTOR	GetFrameDataPos(int _Num) { return m_FrameData[_Num].Pos; }
	//指定のボーン半径を取得する
	float	GetFrameDataRad(int _Num) { return m_FrameData[_Num].Rad; }
	//指定のボーン当たり判定を取得する
	bool	GetFrameDataIsHitFlg(int _Num) { return m_FrameData[_Num].IsCollision; }
	//指定のボーン攻撃判定を取得する
	bool	GetFrameDataIsAttackFlg(int _Num) { return m_FrameData[_Num].IsAttackFlg; }
	//Set
	//プレイヤー座標
	void SetPlayerPos(VECTOR _PlayerPos) { m_PlayerPos = _PlayerPos; }
};
