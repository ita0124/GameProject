#include "ActorBase.h"

//コンストラクタ
ActorBase::ActorBase() {
	Init();
}
//デストラクタ
ActorBase::~ActorBase() {
	Exit();
}
//初期化処理
void ActorBase::Init() {
	ObjectBase::Init();
	m_Kinds = Actor;							//種類設定

	memset(&m_AnimeData, 0, sizeof(ANIME_DATA));	//アニメーション関連変数構造体
	m_AnimeData.Hndl = -1;					//ハンドル
	m_AnimeData.EndFlg = true;				//アニメ終了フラグ

	m_MoveVec = VZERO;							//移動方向
}
//モデル更新処理
void ActorBase::Update() {
	//モデル情報を先に更新
	ObjectBase::Update();

	//アニメーションの更新
	AnimeUpdate();
}

//アニメーションループ再生	
void ActorBase::RequestLoop(int _ID, float _Speed) {
	if (m_AnimeData.ID == _ID)return;

	//再生中のアニメーションがあればいったん消しておく
	if (m_AnimeData.Hndl != -1) {
		//アニメーションをデタッチ
		MV1DetachAnim(m_Hndl, m_AnimeData.Hndl);
		//変数を初期化
		m_AnimeData.Hndl = -1;
	}

	//アニメーションをアタッチ
	m_AnimeData.Hndl = MV1AttachAnim(m_Hndl, _ID);
	//何番目のアニメを再生しているかも一応保存
	m_AnimeData.ID = _ID;
	//初期フレームは0フレーム目に設定
	m_AnimeData.Frame = 1.0f;
	//このアニメの最終フレーム数を保存
	m_AnimeData.EndFrame = MV1GetAnimTotalTime(m_Hndl, _ID);
	//アニメーションの再生速度保存
	m_AnimeData.Speed = _Speed;
	//ループ再生であることを保存しておく
	m_AnimeData.State = ANIME_LOOP;
}
//アニメーション再生(最終フレームで停止)
void ActorBase::RequestEndLoop(int _ID, float _Speed) {
	if (m_AnimeData.ID == _ID)return;

	//基本的にループ再生と同じ
	RequestLoop(_ID, _Speed);
	//ステータスだけ変更
	m_AnimeData.State = ANIME_ENDLOOP;
	//フラグをアニメーション再生へ
	m_AnimeData.EndFlg = false;
}

//アニメーション更新
void ActorBase::AnimeUpdate() {
	if (m_AnimeData.Hndl == -1)return;

	//アニメーションの時間を進める
	m_AnimeData.Frame += m_AnimeData.Speed;

	//最終フレームまで到達したか確認
	if (m_AnimeData.Frame >= m_AnimeData.EndFrame) {
		switch (m_AnimeData.State) {
		case ANIME_LOOP:
			//ループ再生なら最初に戻す
			m_AnimeData.Frame = 1.0f;
			break;
		case ANIME_ENDLOOP:
			//最終フレームで止めるなら値を固定
			m_AnimeData.Frame = m_AnimeData.EndFrame;
			//フラグをアニメーション終了へ
			m_AnimeData.EndFlg = true;
			break;
		}
	}

	//アニメーションの時間を更新
	MV1SetAttachAnimTime(m_Hndl, m_AnimeData.Hndl, m_AnimeData.Frame);
}
//指定した座標Aから指定した座標Bへの方向ベクトルを取得する
VECTOR ActorBase::GetDirectionNotY(VECTOR _VectorA, VECTOR _VectorB, bool _IsNorm) {
	VECTOR Direction = VSub(_VectorB, _VectorA);
	Direction.y = 0.0f;
	if (_IsNorm) {
		Direction = VNorm(Direction);
	}
	return Direction;
}
