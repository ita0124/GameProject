#pragma once
#include "effekseer.h"

class MyEffeckseer {
public:
	enum EFFECTID {

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
	static void Init(int _EffNum, int _ParticleNumcleNum) { CEffekseerCtrl::Init(_EffNum, _ParticleNumcleNum); }
	//破棄
	static void Exit() { CEffekseerCtrl::Exit(); }
	//	更新処理
	//	@memo	: カメラ設定も自動(なるべく使わない)
	//			  ProjectionMtx,CameraMtxの設定も不要
	static void UpdateAutoCamera() { CEffekseerCtrl::UpdateAutoCamera(); }
	//	描画処理
	static void Draw() { CEffekseerCtrl::Draw(); }
	//	エフェクト呼び出し
	//	ID		:	エフェクトID
	//	pos		:	初期位置
	//	isLoop	:	エフェクトがループするか
	static int Request(int _ID, VECTOR _Pos, bool _IsLoop) { return CEffekseerCtrl::Request(_ID, _Pos, _IsLoop); }
	//	指定エフェクト停止
	static void Stop(int _Hndl) { CEffekseerCtrl::Stop(_Hndl); }
	//	座標変更
	static void SetPosition(int _Hndl, VECTOR _Pos) { CEffekseerCtrl::SetPosition(_Hndl, _Pos); }
	//	拡大縮小率変更
	static void SetScale(int _Hndl, VECTOR _Scale) { CEffekseerCtrl::SetScale(_Hndl, _Scale); }
	//	回転角度変更
	static void SetRot(int _Hndl, VECTOR _Rot) { CEffekseerCtrl::SetRot(_Hndl, _Rot); }
};
