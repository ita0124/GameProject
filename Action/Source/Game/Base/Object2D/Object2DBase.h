#pragma once
#include "Comon.h"
#include <vector>

//2Dオブジェクトベースクラス
class Object2DBase {
protected:
	VECTOR					m_Pos;		//座標
	float					m_Rot;		//回転率
	VECTOR					m_Scale;	//拡縮
	VECTOR					m_Size;		//サイズ
	std::vector<std::vector<int>>		m_Hndl;		//画像ハンドル
	float					m_Rad;		//半径
	bool					m_IsActive;	//生存フラグ
	bool					m_IsTurn;	//画像反転をさせるか

	typedef struct {
		int		Frame;						//アニメの現在のフレーム
		int		Time;						//アニメの時間
		int		ChangeTime;					//アニメの再生速度
		int		ID;							//現在のアニメID
		bool	IsLoop;						//アニメループフラグ
		bool	IsPlay;						//アニメ再生フラグ
	}ANIM_DATA;

	ANIM_DATA				m_AnimeData;	//アニメ再生関連データ

	// データ分割読み込み処理
	//_FilePath		:画像のファイルパス
	//_HandleBuf	:ハンドルを保存するint型の配列へのポインタ
	//_AllNum		:画像の分割総数
	//_XNum			:横に対する分割数
	//_YNum			:縦に対する分割数
	//_XSize		:分割された画像の横向き一つの大きさ
	//_YSize		:分割された画像の縦向き一つの大きさ
	void Load(const char* _FilePath, int _AllNum, int _XNum, int _YNum, int _XSize = 32, int _YSize = 32);
	//画像のアニメーションを呼び出し
	//_ID			:再生させるアニメID
	//_ChangeTime	:何フレームで画像を進めるか
	//_IsLoop		:TRUE	=	ループ再生
	//				:FALSE	=	終端再生
	void RequestAnime(int _ID, int _ChangeTime, bool _IsLoop = true);
	//アニメーション再生
	void AnimeUpdate();
	//アニメーション停止
	void AnimeStop();
public:
	//コンストラクタ
	Object2DBase();
	//デストラクタ
	virtual ~Object2DBase();
	//初期化処理
	virtual void Init();
	//データ破棄処理
	virtual void Exit();
	//継承先で使う
	//データ読み込み処理
	virtual void Load() = 0;
	//毎フレーム呼び出す処理
	virtual void Step() = 0;
	//描画処理
	virtual void Draw();
};
