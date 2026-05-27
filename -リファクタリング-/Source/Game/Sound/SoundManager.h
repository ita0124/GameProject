#pragma once
#include "Comon.h"

class SoundManager {
public:
	enum TagID {
		SE_ATK,
		SE_SWORDMISS,
		SE_FLASHLIGHT,
		SE_GUARD,
		SE_PARRY,
		SE_WEAKATK,
		SE_MEDIUMATK,
		SE_STRONGATK,
		SE_LUNGES,
		SE_ELEPHANTCRY,
		SE_SPECIALMOVE,
		BGM_TITLE,
		BGM_SELECT,
		BGM_GAMEOVER,
		BGM_GAMECLEAR,

		NUM_MAX
	};

	enum TagThreeDimensionID {
		SE_ROT,

		THIRD_NUM_MAX
	};

private:
	static vector<int>m_Hndl;	//音楽ハンドル
	static vector<int>m_ThreeDimensionHndl;
public:

	//コンストラクタ・デストラクタ
	SoundManager();
	~SoundManager();

	//初期化処理
	static void Init();

	//データ読み込み処理
	static void Load();

	//音量調節
	static void Volume();

	//データ破棄
	static void Exit();

	//音楽再生
	//_ID		:再生する音楽
	//_Type		:DX_PLAYTYPE_BACK	=	1回再生
	//			:DX_PLAYTYPE_LOOP	=	繰り返し再生
	//_IsStart	:TRUE	=	最初から再生
	//			:FALSE	=	途中から
	//return	:TRUE	=	再生成功
	//			:FALSE	=	再生失敗
	static bool Play(TagID _ID, int _Type = DX_PLAYTYPE_BACK, bool _IsStart = TRUE);

	//指定音楽停止
	static void Stop(TagID _ID);

	//全音楽停止
	static void AllStop();

	//音楽を再生中か
	static bool IsPlay(TagID _ID);

	//音楽再生
	//_ID		:再生する音楽
	//_Rad		:3Dサウンドを再生する時の音が聞こえる範囲
	//_Type		:DX_PLAYTYPE_BACK	=	1回再生
	//			:DX_PLAYTYPE_LOOP	=	繰り返し再生
	//_IsStart	:TRUE	=	最初から再生
	//			:FALSE	=	途中から
	//return	:TRUE	=	再生成功
	//			:FALSE	=	再生失敗
	static bool ThreeDimension_Play(TagThreeDimensionID _ID,float _Rad,VECTOR _Pos, int _Type = DX_PLAYTYPE_BACK, bool _IsStart = TRUE);

	//指定音楽停止
	static void ThreeDimension_Stop(TagThreeDimensionID _ID);

	//音楽を再生中か
	static bool ThreeDimension_IsPlay(TagThreeDimensionID _ID);

	//3Dサウンドの再生座標を設定する
	static void SetThreeDimension_MemPos(VECTOR _Pos, TagThreeDimensionID _ID);
};
