#pragma once
#include "Comon.h"
#include <vector>
#include <iterator>

class SoundManager {
public:
	enum TagID {
		SE_SWORD,
		SE_SHIELD,
		SE_HIT,
		SE_GUARD,
		SE_JUMP,
		SE_SKILL_ATTACK,
		SE_ELEPHANT_NORMAL_ATTACK,
		SE_ELEPHANT_STRONG_ATTACK,
		SE_ELEPHANT_LUNGES,
		SE_ELEPHANT_CRY,
		SE_WOLF_ATTACK,
		SE_MOB_WOLF_DEATH,
		SE_BOSS_WOLF_DEATH,
		SE_BOAR_LUNGES,
		SE_MOB_BOAR_DEATH,
		SE_BOSS_BOAR_DEATH,
		SE_COIN,
		BGM_TITLE,
		BGM_NORMAL_MAP,
		BGM_MAIN,
		BGM_GAME_CLEAR,
		BGM_GAME_OVER,

		NUM_MAX
	};

private:
	static std::vector<int>m_Hndl;	//音楽ハンドル
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
};
