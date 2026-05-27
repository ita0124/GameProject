#include "Object2DBase.h"

//コンストラクタ
Object2DBase::Object2DBase() {
	Init();
}
//デストラクタ
Object2DBase::~Object2DBase() {
	Exit();
}
//初期化処理
void Object2DBase::Init() {
	m_Pos = VZERO;								//座標
	m_Rot = 0.0f;								//回転率
	m_Scale = VONE;								//拡縮
	m_Size = VZERO;								//サイズ
	m_Rad = FZERO;								//半径
	m_IsActive = true;							//生存フラグ
	m_IsTurn = false;							//画像反転させるか

	m_AnimeData.Frame = 0;						//アニメの現在のフレーム
	m_AnimeData.Time = 0;						//アニメの時間
	m_AnimeData.ChangeTime = 0;					//アニメの再生速度
	m_AnimeData.ID = 0;							//現在のアニメID
	m_AnimeData.IsLoop = true;					//アニメループフラグ
	m_AnimeData.IsPlay = true;					//アニメ再生フラグ
}
//データ破棄処理
void Object2DBase::Exit() {
	for (int AnimNum_Index = 0; AnimNum_Index < m_Hndl.size(); AnimNum_Index++)
	{
		for (int Hndl_Index = 0; Hndl_Index < m_Hndl[AnimNum_Index].size(); Hndl_Index++)
		{
			if (m_Hndl[AnimNum_Index][Hndl_Index] != -1)
			{
				DeleteGraph(m_Hndl[AnimNum_Index][Hndl_Index]);
				m_Hndl[AnimNum_Index][Hndl_Index] = -1;
			}
		}
	}

	//画像を全て消去
	m_Hndl.clear();
}
//データ読み込み処理
void Object2DBase::Load(const char* _FilePath, int _AllNum, int _XNum, int _YNum, int _XSize, int _YSize) {
	//ハンドルが空なら処理をしない
	if (_FilePath == nullptr)return;

	vector<int> Hndl;

	//一枚以上ある場合
	if (_AllNum == 1)
	{
		//画像を一枚ロードする
		Hndl.push_back(LoadGraph(_FilePath));
	}
	//複数ある場合は分けてロードする
	else
	{
		//分割分ハンドルを作成する
		for (int Hndl_Index = 0; Hndl_Index < _AllNum; Hndl_Index++)
		{
			Hndl.push_back(-1);
		}

		//画像をロードする
		LoadDivGraph(_FilePath, _AllNum, _XNum, _YNum, _XSize, _YSize, &Hndl[0]);
	}
	m_Hndl.push_back(Hndl);
}
//画像のアニメーションを呼び出し
void Object2DBase::RequestAnime(int _ID, int _ChangeTime, bool _IsLoop) {
	//現在のアニメーションに存在しない場合は処理をしない
	if (m_Hndl.size() < _ID)return;

	//再生中のアニメと同じIDなら処理を行わない
	if (m_AnimeData.ID == _ID)return;

	//アニメーションを設定
	m_AnimeData.Frame = 0;
	m_AnimeData.Time = 0;
	m_AnimeData.ChangeTime = _ChangeTime;
	m_AnimeData.ID = _ID;
	m_AnimeData.IsLoop = _IsLoop;
	m_AnimeData.IsPlay = true;
}
//アニメーション再生
void Object2DBase::AnimeUpdate() {
	//画像が無い場合はアニメーションをしない
	if (m_Hndl.size() == 0)return;

	//最後まで到達していなかったらアニメーションを進める
	if (m_Hndl[m_AnimeData.ID].size() - 1 > m_AnimeData.Frame)
	{
		m_AnimeData.Time++;

		//アニメ速度をタイムが超えたらアニメーションを進める
		if (m_AnimeData.ChangeTime <= m_AnimeData.Time)
		{
			m_AnimeData.Frame++;
			m_AnimeData.Time = 0;
		}
	}
	//ループ再生するなら最初に戻す
	else if (m_AnimeData.IsLoop)
	{
		m_AnimeData.Frame = 0;
	}
}
//アニメーション停止
void Object2DBase::AnimeStop() {
	//アニメーションがすでに止まっている場合は処理をしない
	if (!m_AnimeData.IsPlay)return;

	m_AnimeData.IsPlay = false;
}
//描画処理
void Object2DBase::Draw() {
	if (!m_IsActive || m_Hndl.size() == 0)return;
	DrawRotaGraph3((int)m_Pos.x, (int)m_Pos.y, 0, 0, m_Scale.x, m_Scale.y, m_Rot, m_Hndl[m_AnimeData.ID][m_AnimeData.Frame], TRUE, m_IsTurn);
}
