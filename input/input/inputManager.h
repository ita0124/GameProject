#pragma once
#include <vector>
#include <iterator>
#include "padInput.h"

class CInputManager
{
public:
	enum {
		INPUT_MOVE_L,		//左移動
		INPUT_MOVE_R,		//右移動
		INPUT_MOVE_UP,		//上移動
		INPUT_MOVE_DOWN,	//下移動
		INPUT_JUMP,			//ジャンプ
		INPUT_DASH,			//ダッシュ
		INPUT_ATTACK,		//攻撃

		INPUT_NUM
	};
private:
	static std::vector<int>m_keyButton;		//キー入力用
	static std::vector<tagPadButton>m_pad;	//コントローラー用
public:
	//初期化
	static void Init();
	
	//更新処理
	static void Update();

	//キー入力情報取得(通常判定)
	//_key	:	チェックするキー
	//return:	キー入力したか？
	static bool IsPushKeyRep(int _key);

	//キー入力情報取得(トリガー判定)
	//_key	:	チェックするキー
	//return:	キー入力したか？
	static bool IsPushKeyTrg(int _key);

	//移動縦入力取得
	static float GetMoveX();

	//移動横入力取得
	static float GetMoveY();
};

