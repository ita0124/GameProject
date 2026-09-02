#pragma once
#include "effekseer.h"

class MyEffeckseer {
public:
	enum EFFECTID {
		EFFEKSEER01_HIT,			//被弾
		EFFEKSEER01_ENEMY_DEATH,	//雑魚敵死亡
		TKTK01_BLOW5,				//衝撃波
		TKTK01_BLOW12_DOWN,			//振り下ろし
		TKTK01_BLOW12_UP,			//振り上げ
		TKTK01_SWORD17,				//剣必殺
		TKTK02_BLOW2,				//敵必殺
		TKTK02_CLAW1,				//オオカミ噛みつき
		TKTK02_GUN3,				//突進
		TKTK02_SWORD4_HORIZONTAL,	//斬撃横
		TKTK02_SWORD4_VERTICAL,		//斬撃縦

		EFFECT_NUM
	};

private:

	typedef struct {
		int Hndl;
		VECTOR Pos;
		VECTOR Rot;
		VECTOR Scale;
	}EFFECT_DATA;

	static EFFECT_DATA m_Effect[EFFECT_NUM];

public:
	//コンストラクタ
	MyEffeckseer();
	//デストラクタ
	~MyEffeckseer();

	//エフェクト読み込み
	static void Load();

	//
	// effeckseerクラスのやつそのまま呼び出す
	//

	//	初期化
	//	_EffNum			:	エフェクト総数
	//	_ParticleNum	:	パーティクル総数
	static void Init(int _EffNum, int _ParticleNumcleNum) { EffekseerManager::Init(_EffNum, _ParticleNumcleNum); }
	//破棄
	static void Exit() { EffekseerManager::Exit(); }
	//	更新処理
	//	@memo	: カメラ設定も自動(なるべく使わない)
	//			  ProjectionMtx,CameraMtxの設定も不要
	static void UpdateAutoCamera() { EffekseerManager::UpdateAutoCamera(); }
	//	描画処理
	static void Draw() { EffekseerManager::Draw(); }
	//	エフェクト呼び出し
	//	ID		:	エフェクトID
	//	pos		:	初期位置
	//	isLoop	:	エフェクトがループするか
	static int Request(int _ID, VECTOR _Pos, bool _IsLoop) { return EffekseerManager::Request(_ID, _Pos, _IsLoop); }
	//	指定エフェクト停止
	static void Stop(int _Hndl) { EffekseerManager::Stop(_Hndl); }
	//	座標変更
	static void SetPosition(int _Hndl, VECTOR _Pos) { EffekseerManager::SetPosition(_Hndl, _Pos); }
	//	拡大縮小率変更
	static void SetScale(int _Hndl, VECTOR _Scale) { EffekseerManager::SetScale(_Hndl, _Scale); }
	//	回転角度変更
	static void SetRot(int _Hndl, VECTOR _Rot) { EffekseerManager::SetRot(_Hndl, _Rot); }
};
